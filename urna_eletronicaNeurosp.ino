#include <Keypad.h>
#include <Wire.h>              // Biblioteca necessária para usar comunicação I2C (usada pelo LCD)
#include <LiquidCrystal_I2C.h> // Biblioteca para LCD com módulo I2C (menos fios e mais simples)

//// ----------------- DECLARANDO VARIAVEIS --------------------

// Pinos dos botões reorganizados para A0–A3 por serem entradas analógicas livres.
// O INPUT_PULLUP será usado depois, então os botões vão para o GND.
const int btn_voto1 = A0;
const int btn_voto2 = A1;
const int btn_voto3 = A2;
const int btn_votoNulo = A3;

// Botões de ação (confirmar/cancelar)
const int btn_confirmar = 4;
const int btn_cancelar = 5;

// Contadores independentes para cada tipo de voto.
// São variáveis persistentes usadas para armazenar o resultado.
int votos1 = 0;
int votos2 = 0;
int votos3 = 0;
int votosNulos = 0;

// Configuração do display LCD baseado em I2C.
// Endereço 0x27 é o mais comum; 16 colunas e 2 linhas.
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Senha do administrador (modo secreta).
// "adm" indica se o sistema está no modo administrador.
// "senhaDigitada" guarda os números conforme o usuário digita.
String senha = "61821202116924"; 	 
bool adm = false; 		 
String senhaDigitada = ""; 	

//// ---------------------- TECLADO 4X4 -------------------------

// Matriz representando as teclas físicas do keypad.
const byte LINHAS = 4;
const byte COLUNAS = 4;

// Mapeamento exato da matriz física do teclado 4x4.
char teclas[LINHAS][COLUNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Pinos usados pelo keypad (linhas e colunas).
// Esses pinos precisam bater com a fiação usada no teclado.
byte pinosLinhas[LINHAS] = {9, 8, 7, 6};
byte pinosColunas[COLUNAS] = {13, 12, 11, 10};

// Inicializa o teclado com o mapa e os pinos definidos.
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

//// ---------------------- CÓDIGO PRINCIPAL -------------------

void setup() {
  lcd.init();      // Inicializa o módulo LCD
  lcd.backlight(); // Liga a luz do LCD
  lcd.clear();

  // Ativa resistores PULLUP internos nos botões.
  // Com isso, o botão NÃO apertado fica em HIGH e apertado vira LOW.
  // Isso evita "ruído" (botão apertando sozinho).
  pinMode(btn_voto1, INPUT_PULLUP);
  pinMode(btn_voto2, INPUT_PULLUP);
  pinMode(btn_voto3, INPUT_PULLUP);
  pinMode(btn_votoNulo, INPUT_PULLUP);
  pinMode(btn_cancelar, INPUT_PULLUP);
  pinMode(btn_confirmar, INPUT_PULLUP);
}

void loop() {

  // ---------- MODO VOTANTE ----------
  if (!adm) { 	
    lcd.setCursor(0, 0);
    lcd.print("Ola!");
    lcd.setCursor(0, 1);
    lcd.print("escolha seu voto");

    // Verifica se algum botão físico de voto foi apertado
    votar();

    // Tecla secreta 'D' abre modo administrador
    char tecla = teclado.getKey();
    if (tecla == 'D') {
      adm = true;
      senhaDigitada = "";   // limpa senha antes de começar a digitar
      lcd.clear();
      lcd.print("Senha Admin: ");
      lcd.setCursor(0, 1);
    }
  }

  // ---------- MODO ADMINISTRADOR (DIGITAÇÃO DA SENHA) ----------
  else { 	
    char tecla = teclado.getKey();
    if (tecla) {

      // Se for um número, adiciona ao buffer da senha
      if (tecla >= '0' && tecla <= '9') {
        senhaDigitada += tecla;
        lcd.setCursor(0, 1);
        lcd.print(senhaDigitada);
      }

      // Tecla "#" confirma a senha digitada
      else if (tecla == '#') {
        if (senhaDigitada == senha) {
          TelaAdm(); 	// senha correta → abre menu admin
        } else {
          // Senha errada → mensagem e volta pro modo votação
          lcd.clear();
          lcd.print("Senha incorreta");
          delay(1500);
          adm = false;
          lcd.clear();
        }
      }

      // "*" funciona como apagar (backspace) ou sair do admin se vazio
      else if (tecla == '*') {

        // Apaga último dígito se houver texto
        if (senhaDigitada.length() > 0) {
          senhaDigitada.remove(senhaDigitada.length() - 1);
          lcd.setCursor(0, 1);
          lcd.print(senhaDigitada + " "); // o espaço extra serve para apagar o caractere antigo
        }

        // Se nada digitado, sai do modo admin
        else {
          adm = false; 	
          lcd.clear();
        }
      }
    }
  }
}

//// ---------------------- FUNÇÃO VOTAR -------------------------

void votar() {
  // A leitura usa LOW = botão pressionado, porque usa INPUT_PULLUP

  // Para cada botão, faz uma pequena espera (debouncing)
  // e chama a função confirmarVoto() correspondente.

  if (digitalRead(btn_voto1) == LOW) {
    delay(50); 
    if (digitalRead(btn_voto1) == LOW) confirmarVoto("No candidato 1?", &votos1);
  }
  else if (digitalRead(btn_voto2) == LOW) {
    delay(50);
    if (digitalRead(btn_voto2) == LOW) confirmarVoto("No candidato 2?", &votos2);
  }
  else if (digitalRead(btn_voto3) == LOW) {
    delay(50);
    if (digitalRead(btn_voto3) == LOW) confirmarVoto("No candidato 3?", &votos3);
  }
  else if (digitalRead(btn_votoNulo) == LOW) {
    delay(50);
    if (digitalRead(btn_votoNulo) == LOW) confirmarVoto("Voto NULO?", &votosNulos);
  }
}

//// ---------------------- FUNÇÃO CONFIRMAR VOTO ---------------------

void confirmarVoto(String mensagem, int* candidato) {
  
  // Mostra mensagem perguntando se deseja confirmar
  lcd.clear();
  lcd.print("Confirmar voto");
  lcd.setCursor(0, 1);
  lcd.print(mensagem);

  // Flags para detectar início e fim do pressionamento
  bool confirmarPressionado = false;
  bool cancelarPressionado = false;
  bool votoConcluido = false; 

  // Loop fica esperando confirmação ou cancelamento
  while (true) {
    
    // Detecta quando o botão COMEÇA a ser pressionado
    if (digitalRead(btn_confirmar) == LOW) {
      confirmarPressionado = true;
    } 
    else if (digitalRead(btn_cancelar) == LOW) {
      cancelarPressionado = true;
    }

    // Detecta quando o botão foi SOLTO → ação válida
    if (confirmarPressionado && digitalRead(btn_confirmar) == HIGH) {

      // SOLTO → confirma o voto e incrementa o contador
      (*candidato)++;
      lcd.clear();
      lcd.print("Voto registrado!");
      delay(1500);
      lcd.clear();
      votoConcluido = true;
      break;
    } 
    else if (cancelarPressionado && digitalRead(btn_cancelar) == HIGH) {

      // Cancelado
      lcd.clear();
      lcd.print("Voto cancelado");
      delay(1500);
      lcd.clear();
      votoConcluido = true;
      break;
    }
  }
  
  // Correção importante: aguarda o botão de voto ser SOLTO
  if (votoConcluido) {
    while (digitalRead(btn_voto1) == LOW || 
           digitalRead(btn_voto2) == LOW ||
           digitalRead(btn_voto3) == LOW || 
           digitalRead(btn_votoNulo) == LOW) {
      delay(10); 
    }
  }
}

//// ---------------------- FUNÇÃO RESULTADOS --------------------------

void mostrarResultados() {
  // Mostra todos os votos divididos em duas linhas
  lcd.clear();
  lcd.print("1:");
  lcd.print(votos1);
  lcd.setCursor(8, 0);
  lcd.print("2:");
  lcd.print(votos2);

  lcd.setCursor(0, 1);
  lcd.print("3:");
  lcd.print(votos3);
  lcd.setCursor(8, 1);
  lcd.print("N:");
  lcd.print(votosNulos);

  // Admin sai pressionando '*'
  while (true) {
    char tecla = teclado.getKey();
    if (tecla == '*') {
      break;
    }
  }
  lcd.clear();
}

//// ---------------------- MENU DO ADMINISTRADOR ---------------------

void TelaAdm() {

  // Mostra menu com 3 opções
  lcd.clear();
  lcd.print("1-Resultado ");
  lcd.setCursor(0, 1);
  lcd.print("2-Zerar 3-Sair");

  while (true) {
    char tecla = teclado.getKey();
    if (tecla) {

      // Opção 1 → ver resultados
      if (tecla == '1') {
        mostrarResultados();
        break;
      }

      // Opção 2 → zerar todos os contadores
      else if (tecla == '2') {
        votos1 = votos2 = votos3 = votosNulos = 0;
        lcd.clear();
        lcd.print("Votos zerados");
        delay(1500);
        lcd.clear();
        break;
      }

      // Opção 3 → sair do menu admin
      else if (tecla == '3') {
        lcd.clear();
        break;
      }
    }
  }

  adm = false; // Sai do modo admin e volta para votação
}
