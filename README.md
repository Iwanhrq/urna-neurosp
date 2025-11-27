# 🗳️ Urna Eletrônica Arduino – Projeto Escolar

Este projeto implementa uma urna eletrônica funcional utilizando Arduino, com votação física por botões, confirmação, cancelamento, contagem de votos e um modo administrador com senha para acessar resultados ou zerar os votos.

Ideal para trabalhos escolares, feiras de tecnologia, demonstrações e projetos de sistemas embarcados.

---

## 🚀 Funcionalidades

- Votação via botões físicos  
- LCD I2C 16x2 para interface  
- Modo Administrador:  
  - Acesso via senha  
  - Visualizar votos  
  - Zerar votos  
- Confirmação e cancelamento de votos  
- Compatível com Arduino Uno, Nano e Mega  

---

## 📦 Bibliotecas Necessárias

### 1. Keypad (GitHub)
Baixar manualmente deste repositório:  
https://github.com/Chris--A/Keypad

Instalação:  
- Baixe o ZIP (`Code > Download ZIP`)  
- No Arduino IDE: *Sketch → Incluir Biblioteca → Adicionar Biblioteca .ZIP*

---

### 2. LiquidCrystal_I2C (Arduino IDE)
Instala diretamente pela IDE:

- *Sketch → Incluir Biblioteca → Gerenciar Bibliotecas*  
- Pesquisar por **LiquidCrystal I2C**  
- Instalar a biblioteca (ex: Frank de Brabander)

---

## 🛠️ Como Usar a Urna

### ✔️ Modo Votante
Ao ligar a urna, ela exibe a tela inicial.

Os botões realizam:

- Botão 1 → Candidato 1  
- Botão 2 → Candidato 2  
- Botão 3 → Candidato 3  
- Botão Nulo → Voto nulo  

Após apertar um botão, o sistema pedirá confirmação:

- Botão **CONFIRMAR** → registra o voto  
- Botão **CANCELAR** → descarta voto  

---

### 🔐 Modo Administrador

Para entrar:

1. Pressione a tecla **D** no teclado 4x4  
2. Digite a senha definida no código  
3. Pressione **#** para confirmar  

O menu exibirá:

- **1** → mostra os votos (pressione * para sair)  
- **2** → zera todos os votos  
- **3** → volta ao modo votação  

---

## 🔌 Circuito / Montagem

*(Adicione aqui seu diagrama depois)*

---

## 📁 Arquivos do Projeto

- `urna_eletronica.ino` – Código principal  
- `README.md` – Documentação  

---

## 👨‍💻 Autor

Projeto desenvolvido para fins educacionais na ETEC – Desenvolvimento de Sistemas.
