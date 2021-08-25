#include <Arduino.h>
#include <Stepper.h>
#include <Wire.h>

void receiveEvent(int numBytes);
void LigaMixer();
void movimentaEsteira(const int passos);
void findRaft();
void requestEvent();

//operacional
int isOperational = false;
int dadoRecebido = 0;

//  botao
const int buttonPin = 2; // the pin that the pushbutton is attached to
int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button
int raftFound = false;

//motor de passo da esteira

int L4 = 4;
int L5 = 5;
int L6 = 6;
int L7 = 7;

// Motor de passo do Mixer
const int stepsPerRevolution = 2048;
int L8 = 8;
int L9 = 9;
int L10 = 10;
int L11 = 11;

// Motor Dc do mixer
int L12 = 12;
int L13 = 13;

Stepper PassoMixer = Stepper(stepsPerRevolution, L8, L10, L9, L11);

void setup()
{

  pinMode(L12, OUTPUT);
  pinMode(L13, OUTPUT);

  // VELOVIDADE DO  MOTOR DE PASSO DO MIXER 10 rpm:
  PassoMixer.setSpeed(10);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop()
{
  if (dadoRecebido != 0)
  {
    findRaft();



  }

  isOperational = false;
}

void movimentaEsteira(const int passos)
{
  Stepper PassoEsteira = Stepper(passos, L4, L5, L6, L7);
  PassoEsteira.setSpeed(500);
  PassoEsteira.step(2000);
}

void LigaMixer()
{

  PassoMixer.step(stepsPerRevolution);
  delay(500);
  digitalWrite(L12, HIGH);
  digitalWrite(L13, LOW);
  delay(3000);
  digitalWrite(L12, LOW);
  digitalWrite(L13, LOW);
  delay(500);

  PassoMixer.step(-stepsPerRevolution);
}

void receiveEvent(int numBytes)
{
  Serial.print("ESP nao mandou esse valor " + dadoRecebido);
  while (0 < Wire.available())
  {
    dadoRecebido = Wire.read();
    Serial.print("ESP mandou esse valor " + dadoRecebido);
    delay(2000);
  }
  
 
}
void requestEvent(int num)
{
  Wire.write(num); /*send string on request */
}
void findRaft()
{

  if (raftFound == false)
  {
    Serial.println("Raft location not known yet");

    delay(1000);
    Serial.print("Looking for the raft...");
    buttonState = digitalRead(buttonPin); // read the pushbutton input pin:
    Serial.println(buttonState);
    delay(1000);
    while (buttonState == LOW)
    {
      buttonState = digitalRead(buttonPin);
      movimentaEsteira(10);
    }
    raftFound = true;
    movimentaEsteira(-10);
    delay(5000);
    isOperational = true;
  }
}