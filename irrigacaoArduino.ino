// ========================================================================================================
// --- Bibliotecas Auxiliares ---
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ========================================================================================================
// --- Definições ---
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
 
#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

unsigned long tempoDesligado = 0;
unsigned long tempoBombaLigado = 0;
unsigned long tempoLigadoTela = 0;


// C++ code
//
extern volatile unsigned long timer0_millis;
unsigned long previousMillis = 0;
unsigned long tempoBomba = 0;
int bomba1 = 10;
int bomba2 = 9;

long tempoParaLigar = 30000;
long tempoLigado = 30000;

int estadoBomba = 0;          // Estado da bomba (0 = desligado, 1 = ligado)
int leituraBotao;              // Variável para armazenar a leitura do botão
const int pinoBotao = 7;
int estadoBotaoAnterior = HIGH;// Estado anterior do botão

void setup()
{
  Serial.begin(9600);
  pinMode(bomba1,OUTPUT);     //define o pino 10 como saída
  pinMode(bomba2,OUTPUT);    //define o pino 9 como saída
  pinMode(pinoBotao, INPUT_PULLUP);// Configura o pino do botão como entrada com pull-up interno

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Inicializa OLED com endereço I2C 0x3C (para 128x64)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,14);
  display.clearDisplay();
  display.print("CEEP IRRIGA");
  display.display(); 
  delay(2000);
  

}


void loop()
{
 unsigned long currentMillis = millis(); 
 leituraBotao = digitalRead(pinoBotao); 
  
  tempoDesligado = currentMillis;
  tempoBombaLigado = tempoBomba;
  
      previousMillis = currentMillis;
    // Executar a ação desejada
  	Serial.print("Estado da Bomba: " + String(estadoBomba));
    Serial.print(" - Tempo decorrido: " + String(currentMillis));
    Serial.println(" - Bomba Ligada: " + String(tempoBomba));
  
  if (leituraBotao == LOW && estadoBotaoAnterior == HIGH) {//Se estadoButton for igual a HIGH ou 1
    
    estadoBomba = !estadoBomba;
    delay(50); // Debounce para evitar múltiplas leituras  
    
    }
   // Atualiza o estado anterior do botão
  estadoBotaoAnterior = leituraBotao;
  
  if (estadoBomba == 1) {
    
     digitalWrite(bomba1 , HIGH);
    digitalWrite(bomba2 , HIGH);
  }else{
  		digitalWrite(bomba1 , LOW);
        digitalWrite(bomba2 , LOW);
  }  
  
  
  delay(250); // Aguarda 250 milissegundos antes de mudar a frequência
 
  if(currentMillis > tempoParaLigar){
    tempoBomba = currentMillis - tempoParaLigar;
    chamaBomba();
  }
  
    if(tempoBomba > tempoLigado){
    timer0_millis = 0;
    tempoBomba = 0;
    desligaBomba();
  }
  
  if(estadoBomba == 0){
  telaBombaDesligada();
  }
  
  if(estadoBomba == 1){
  telaBombaLigada();
  }
  
 }

void chamaBomba(){
    estadoBomba = 1;
    delay(50); // Debounce para evitar múltiplas leituras  
   // Atualiza o estado anterior do botão
  estadoBotaoAnterior = leituraBotao;
  
  if (estadoBomba == 1) {
    tempoLigadoTela = 0;
     digitalWrite(bomba1 , HIGH);
    digitalWrite(bomba2 , HIGH);
  }else{
  		digitalWrite(bomba1 , LOW);
        digitalWrite(bomba2 , LOW);
  }  
 
  
  delay(250); // Aguarda 250 milissegundos antes de mudar a frequência
 
}



void desligaBomba(){
    estadoBomba = 0;
    delay(50); // Debounce para evitar múltiplas leituras  
   // Atualiza o estado anterior do botão
  estadoBotaoAnterior = leituraBotao;
  
  		digitalWrite(bomba1 , LOW);
        digitalWrite(bomba2 , LOW);  
  
  delay(250); // Aguarda 250 milissegundos antes de mudar a frequência
 
}


void telaBombaDesligada(){
   unsigned long tempoDesligadoTela = tempoDesligado;
   tempoDesligadoTela = tempoDesligadoTela / 1000;
   int hours = tempoDesligadoTela / 3600;
int minutes = (tempoDesligadoTela % 3600) / 60;
int seconds = tempoDesligadoTela % 60;

char tempoFormatado[9]; // "HH:MM:SS" + null
sprintf(tempoFormatado, "%02d:%02d:%02d", hours, minutes, seconds);


  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,4);
  display.clearDisplay();
  display.print("Bomba Desligada");
  display.setCursor(0,20);
  display.print(tempoFormatado);
  display.display();
  delay(500);
  

}

void telaBombaLigada(){
  
  if(tempoBomba > 0){
  unsigned long tempoLigadoTela = tempoBombaLigado;
   tempoLigadoTela = tempoLigadoTela / 1000;
   int hours = tempoLigadoTela / 3600;
int minutes = (tempoLigadoTela % 3600) / 60;
int seconds = tempoLigadoTela % 60;

char tempoFormatado[9]; // "HH:MM:SS" + null
sprintf(tempoFormatado, "%02d:%02d:%02d", hours, minutes, seconds);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,4);
  display.clearDisplay();
  display.print("Bomba Ligada");
  display.setCursor(0,20);
  display.print(tempoFormatado);
  display.display();
  delay(500);
    
  }else{
    tempoLigadoTela = tempoLigadoTela + 1;
   tempoLigadoTela = tempoLigadoTela;
   int hours = tempoLigadoTela / 3600;
int minutes = (tempoLigadoTela % 3600) / 60;
int seconds = tempoLigadoTela % 60;

char tempoFormatado[9]; // "HH:MM:SS" + null
sprintf(tempoFormatado, "%02d:%02d:%02d", hours, minutes, seconds);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,4);
  display.clearDisplay();
  display.print("Bomba Ligada");
  display.setCursor(0,20);
  display.print(tempoFormatado);
  display.display();
  delay(500);
  
  
  }
  
  

}
