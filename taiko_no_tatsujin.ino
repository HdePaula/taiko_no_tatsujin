#include <Keyboard.h>

// -----------------------------------------------------------------------------------------------PIEZOS
const int NUM_PADS = 4;
const int pinosPiezo[NUM_PADS] = {A0, A1, A2, A3}; // Certifique-se de que o 4º piezo está no A3
const char teclasPads[NUM_PADS] = {'a', 's', 'd', 'f'};

// -----------------------------------------------------------------------------------------------BOTOES
const int pinoBotao1 = 2;
const int pinoBotao2 = 3;

const char teclaBotao1 = 'q';
const char teclaBotao2 = 'w'; 

bool estadoAnteriorB1 = false;
bool estadoAnteriorB2 = false;

// -----------------------------------------------------------------------------------------------CONFIG
// Aumente este valor se a vibração da mesa estiver disparando os pads
const int limiarMinimo = 140;
const unsigned long tempoBloqueio = 50;   // Tempo em ms para evitar duplo disparo

unsigned long ultimaBatida[NUM_PADS] = {0, 0, 0, 0};
bool padAtivo[NUM_PADS] = {false, false, false, false};

void setup() {
  pinMode(pinoBotao1, INPUT_PULLUP);
  pinMode(pinoBotao2, INPUT_PULLUP);

  // Inicia a emulação de teclado USB
  Keyboard.begin();
}

// Função especial que limpa o canal analógico do Arduino antes de ler
int lerPiezoLimpo(int pino) {
  analogRead(pino); // Primeira leitura descarta a carga residual do pino anterior
  delayMicroseconds(50);
  return analogRead(pino); // Segunda leitura pega o valor real
}

void loop() {
  unsigned long tempoAtual = millis(); // Captura o tempo atual do relógio interno

  // -----------------------------------------------------------------------------------------------Leitura dos Botoes
  bool botao1Pressionado = (digitalRead(pinoBotao1) == LOW);
  if (botao1Pressionado != estadoAnteriorB1) {
    if (botao1Pressionado) {
      Keyboard.press(teclaBotao1);
    } else {
      Keyboard.release(teclaBotao1);
    }
    estadoAnteriorB1 = botao1Pressionado;
  }

  bool botao2Pressionado = (digitalRead(pinoBotao2) == LOW);
  if (botao2Pressionado != estadoAnteriorB2) {
    if (botao2Pressionado) {
      Keyboard.press(teclaBotao2);
    } else {
      Keyboard.release(teclaBotao2);
    }
    estadoAnteriorB2 = botao2Pressionado;
  }

  // -----------------------------------------------------------------------------------------------Leitura dos Piezos
  for (int i = 0; i < NUM_PADS; i++) {
    int leitura = lerPiezoLimpo(pinosPiezo[i]);

    // Detecta a batida e pressiona a tecla correspondente
    if (leitura > limiarMinimo && !padAtivo[i]) {
      Keyboard.press(teclasPads[i]);
      padAtivo[i] = true;
      ultimaBatida[i] = tempoAtual;
    }

    // Solta a tecla após o tempo de bloqueio
    if (padAtivo[i] && (tempoAtual - ultimaBatida[i] >= tempoBloqueio)) {
      Keyboard.release(teclasPads[i]);
      padAtivo[i] = false;
    }
  }
}
