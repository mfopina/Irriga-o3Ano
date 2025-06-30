const int pinoRele = 12;
const int pinoBotao = 8;

const unsigned long tempoLigado = 30000;
const unsigned long tempoDesligado = 2000;

bool releLigado = false;
unsigned long tempoAnterior = 0;

bool estadoAnteriorBotao = HIGH;  // estado anterior do botão

void setup() {
  pinMode(pinoRele, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP);
  digitalWrite(pinoRele, LOW);
}

void loop() {
  unsigned long tempoAtual = millis();
  bool estadoAtualBotao = digitalRead(pinoBotao);
  bool botaoApertado = (estadoAnteriorBotao == HIGH && estadoAtualBotao == LOW); // Detecta borda de descida

  estadoAnteriorBotao = estadoAtualBotao; // Atualiza estado do botão para o próximo loop

  if (releLigado) {
    // Se o tempo acabar ou botão for pressionado, desliga
    if (botaoApertado || (tempoAtual - tempoAnterior >= tempoLigado)) {
      digitalWrite(pinoRele, LOW);
      releLigado = false;
      tempoAnterior = tempoAtual;
    }
  } else {
    // Se botão for pressionado, liga manualmente
    if (botaoApertado) {
      digitalWrite(pinoRele, HIGH);
      releLigado = true;
      tempoAnterior = tempoAtual;
    }
    // Caso contrário, liga automaticamente após tempo
    else if (tempoAtual - tempoAnterior >= tempoDesligado) {
      digitalWrite(pinoRele, HIGH);
      releLigado = true;
      tempoAnterior = tempoAtual;
    }
  }
}
