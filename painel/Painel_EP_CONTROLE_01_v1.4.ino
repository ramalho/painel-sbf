// Painel "Controle" SBF
// Escrito para Arduino nano
// Ernesto s. Junior
// 24/11/2024

#define led_habilita 13
#define rele_habilita 10
#define led_externo A4
#define rele_externo 9
#define led_analogico A5
#define rele_analogico 8
#define led_digital 2
#define rele_digital 7
#define led_wifi A0
#define rele_wifi 6

int botao_habilita = A1;
int botao_externo = 5;
int botao_analogico = 4;
int botao_digital = 3;
int botao_wifi = A3;
int pino_curto = 11;
int pino_geral = 12;
int leitura_geral;
int leitura_curto;
int leitura_espera;

void espera();
void curto();
void externo();
void analogico();
void digital();
void wifi();

void setup() {
  pinMode(rele_habilita, OUTPUT);
  pinMode(led_habilita, OUTPUT);
  pinMode(botao_habilita, INPUT_PULLUP);
  pinMode(rele_externo, OUTPUT);
  pinMode(led_externo, OUTPUT);
  pinMode(botao_externo, INPUT_PULLUP);
  pinMode(rele_analogico, OUTPUT);
  pinMode(led_analogico, OUTPUT);
  pinMode(botao_analogico, INPUT_PULLUP);
  pinMode(rele_digital, OUTPUT);
  pinMode(led_digital, OUTPUT);
  pinMode(botao_digital, INPUT_PULLUP);
  pinMode(rele_wifi, OUTPUT);
  pinMode(led_wifi, OUTPUT);
  pinMode(botao_wifi, INPUT_PULLUP);
  pinMode(pino_geral, INPUT_PULLUP);
  pinMode(pino_curto, INPUT_PULLUP);
  digitalWrite(rele_habilita, HIGH);
  digitalWrite(led_habilita, HIGH);
  digitalWrite(rele_externo, HIGH);
  digitalWrite(led_externo, HIGH);
  digitalWrite(rele_analogico, HIGH);
  digitalWrite(led_analogico, HIGH);
  digitalWrite(rele_digital, HIGH);
  digitalWrite(led_digital, HIGH);
  digitalWrite(rele_wifi, HIGH);
  digitalWrite(led_wifi, HIGH);
}

void loop() {
  leitura_geral = digitalRead(pino_geral); 
  if (leitura_geral != 0) {
    digitalWrite(rele_habilita, HIGH);
    digitalWrite(led_habilita, HIGH);
    digitalWrite(rele_externo, HIGH);
    digitalWrite(led_externo, HIGH);
    digitalWrite(rele_analogico, HIGH);
    digitalWrite(led_analogico, HIGH);
    digitalWrite(rele_digital, HIGH);
    digitalWrite(led_digital, HIGH);
    digitalWrite(rele_wifi, HIGH);
    digitalWrite(led_wifi, HIGH);
  }
  if (leitura_geral != 1) {
//    externo();
//    analogico();
//    digital();
    wifi();
  }
}

void espera() {
  digitalWrite(rele_habilita, HIGH);
  digitalWrite(rele_externo, HIGH);
  digitalWrite(rele_analogico, HIGH);
  digitalWrite(rele_digital, HIGH);
  digitalWrite(rele_wifi, HIGH);
  digitalWrite(led_habilita, HIGH);

  digitalWrite(led_externo, LOW);
  digitalWrite(led_analogico, LOW);
  digitalWrite(led_digital, LOW);
  delay(1000);
  digitalWrite(led_externo, HIGH);
  digitalWrite(led_analogico, HIGH);
  digitalWrite(led_digital, HIGH);
  delay(1000);
}

void curto() {
  leitura_curto = digitalRead(pino_curto);
  if (leitura_curto != 1) {
    digitalWrite(rele_habilita, HIGH);
    digitalWrite(led_habilita, LOW);
    delay(100);
    digitalWrite(led_habilita, HIGH);
    delay(100);
  } else {
    byte leitura_habilita;         //Armazena informações sobre a leitura do botão
    static byte leitura_habilita_ant;
    static boolean estado_rele_habilita;  //Armazena o estado do relé (ligado/desligado)
    leitura_habilita = digitalRead(botao_habilita);
    if (leitura_habilita != leitura_habilita_ant) {
      if (leitura_habilita) {
        estado_rele_habilita = !estado_rele_habilita;
        digitalWrite(rele_habilita, estado_rele_habilita);
        digitalWrite(led_habilita, estado_rele_habilita);
      }
    }  
    leitura_habilita_ant = leitura_habilita;
    delay(10);
  }
}

void externo() {
  byte leitura_externo;         //Armazena informações sobre a leitura do botão
  static byte leitura_externo_ant;
  static boolean estado_rele_externo;  //Armazena o estado do relé (ligado/desligado)
  leitura_externo = digitalRead(botao_externo);
  if (leitura_externo != leitura_externo_ant) {
    digitalWrite(rele_analogico, HIGH);
    digitalWrite(led_analogico, HIGH);
    digitalWrite(rele_digital, HIGH);
    digitalWrite(led_digital, HIGH);
    digitalWrite(rele_wifi, HIGH);
    digitalWrite(led_wifi, HIGH);
    if (leitura_externo) {
      estado_rele_externo = !estado_rele_externo;
      digitalWrite(rele_externo, estado_rele_externo);
      digitalWrite(led_externo, estado_rele_externo);
    }
  }
  leitura_externo_ant = leitura_externo;
  delay(10);
}

void analogico() {
  byte leitura_analogico;         //Armazena informações sobre a leitura do botão
  static byte leitura_analogico_ant;
  static boolean estado_rele_analogico;  //Armazena o estado do relé (ligado/desligado)
  leitura_analogico = digitalRead(botao_analogico);
  if (leitura_analogico != leitura_analogico_ant) {
    digitalWrite(rele_externo, HIGH);
    digitalWrite(led_externo, HIGH);
    digitalWrite(rele_digital, HIGH);
    digitalWrite(led_digital, HIGH);
    digitalWrite(rele_wifi, HIGH);
    digitalWrite(led_wifi, HIGH); 
    if (leitura_analogico) {
      estado_rele_analogico = !estado_rele_analogico;
      digitalWrite(rele_analogico, estado_rele_analogico);
      digitalWrite(led_analogico, estado_rele_analogico);      
    }
  }
  leitura_analogico_ant = leitura_analogico;
  delay(10);
}

void digital() {
  byte leitura_digital;         //Armazena informações sobre a leitura do botão
  static byte leitura_digital_ant;
  static boolean estado_rele_digital;  //Armazena o estado do relé (ligado/desligado)
  leitura_digital = digitalRead(botao_digital);
  if (leitura_digital != 0) {
    digitalWrite(rele_externo, HIGH);
    digitalWrite(led_externo, HIGH);
    digitalWrite(rele_analogico, HIGH);
    digitalWrite(led_analogico, HIGH);
    digitalWrite(rele_wifi, HIGH);
    digitalWrite(led_wifi, HIGH);
  }
  if (leitura_digital != leitura_digital_ant) {
    if (leitura_digital) {
      estado_rele_digital = !estado_rele_digital;
      digitalWrite(rele_digital, estado_rele_digital);
      digitalWrite(led_digital, estado_rele_digital);      
    }
  }
  leitura_digital_ant = leitura_digital;
  delay(10);
}

void wifi() {
  byte leitura_wifi;         //Armazena informações sobre a leitura do botão 
  static byte leitura_wifi_ant;
  static boolean estado_rele_wifi;  //Armazena o estado do relé (ligado/desligado)
  leitura_wifi = digitalRead(botao_wifi);
  if (leitura_wifi != 0) {
  digitalWrite(rele_externo, HIGH);
  digitalWrite(led_externo, HIGH);
  digitalWrite(rele_analogico, HIGH);
  digitalWrite(led_analogico, HIGH);
  digitalWrite(rele_digital, HIGH);
  digitalWrite(led_digital, HIGH);
  }  
  if (leitura_wifi != leitura_wifi_ant) {
    if (leitura_wifi) {
      estado_rele_wifi = !estado_rele_wifi;
      digitalWrite(rele_wifi, estado_rele_wifi);
      digitalWrite(led_wifi, estado_rele_wifi);
      if (estado_rele_wifi = true) {
        curto();
      }
    }
  }
  leitura_wifi_ant = leitura_wifi;
  delay(10);
}