#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Stepper.h>

const char *ssid = "Casa";             // Enter SSID here
const char *password = "caixadeareia"; //Enter Password here
const int stepsPerRevolution = 2048;

Stepper Motor2 = Stepper(stepsPerRevolution, D5, D6, D7, D8);
const int subida = 3000;
const int descida = -3000;

String SendHTML(uint8_t receita1, uint8_t receita2,uint8_t receita3);
void handle_OnConnect();
void handle_PedirReceita1();
void handle_FazendoReceita1();
void handle_PedirReceita2();
void handle_FazendoReceita2();
void handle_PedirReceita3();
void handle_FazendoReceita3();
void handle_NotFound();
ESP8266WebServer server(80);

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

int sentdata = 0;
int recievedata = 0;

void setup()
{
  Serial.begin(9600); /* begin serial for debug */

  //inicializa motor
  Motor2.setSpeed(500);

  //incia pacto
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/PedirReceita1", handle_PedirReceita1);
  server.on("/FazendoReceita1", handle_FazendoReceita1);
  server.on("/PedirReceita2", handle_PedirReceita2);
  server.on("/FazendoReceita2", handle_FazendoReceita2);
  server.on("/PedirReceita3", handle_PedirReceita3);
  server.on("/FazendoReceita3", handle_FazendoReceita3);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{

  server.handleClient();
  if (LED1status)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, LOW);
  }

  if (LED2status)
  {
    digitalWrite(LED2pin, HIGH);
  }
  else
  {
    digitalWrite(LED2pin, LOW);
  }
  Wire.beginTransmission(8); /* begin with device address 8 */

  Wire.write(1233);       /* sends hello string */
  Wire.endTransmission(); /* stop transmitting */

  Wire.requestFrom(8, 1); /* request & read data of size 1 from slave */
  while (Wire.available())
  {
    int c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
  delay(10000);
}

void handle_PedirReceita1()
{
  sentdata = 1;

  Wire.beginTransmission(8); /* begin with device address 8 */
  Wire.write(sentdata); /* sends hello string */
  Wire.endTransmission();

  LED1status = HIGH;
  Serial.println("Pediu a receita 1");
  server.send(200, "text/html", SendHTML(true, false, false));
}

void handle_FazendoReceita1()
{

  Serial.println("Fazendo receita1");
  server.send(200, "text/html", SendHTML(false, false, false));
}

void handle_PedirReceita2()
{
  sentdata = 2;

  Wire.beginTransmission(8); /* begin with device address 8 */
  Wire.write(sentdata); /* sends hello string */
  Wire.endTransmission();

  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(false, true, false));
}

void handle_FazendoReceita2()
{
  sentdata = 3;
  Wire.beginTransmission(8); /* begin with device address 8 */
  Wire.write(sentdata); /* sends hello string */
  Wire.endTransmission();

  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(false, false, false));
}
void handle_PedirReceita3()
{
  LED2status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(false, false, true));
}

void handle_FazendoReceita3()
{

  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(false, false, false));
}
void handle_OnConnect()
{
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(false, false,false));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t receita1, uint8_t receita2, uint8_t receita3)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>CockTail Machine</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Cocktail Machine</h1>\n";
  ptr += "<h3>Conectado via WIFI</h3>\n";

  if (receita1)
  {
    ptr += "<p>Receita 1: </p><a class=\"button button-off\" href=\"/FazendoReceita1\">Fazendo</a>\n";
  }
  else
  {
    ptr += "<p>Receita 1: </p><a class=\"button button-on\" href=\"/PedirReceita1\">Pedir</a>\n";
  }

  if (receita2)
  {
    ptr += "<p>Receita 2: </p><a class=\"button button-off\" href=\"/led2off\">Fazendo</a>\n";
  }
  else
  {
    ptr += "<p>Receita 2: </p><a class=\"button button-on\" href=\"/led2on\">Pedir</a>\n";
  }
  if (receita3)
  {
    ptr += "<p>Receita 3: </p><a class=\"button button-off\" href=\"/led2off\">Em falta</a>\n";
  }
  else
  {
    ptr += "<p>Receita 3: </p><a class=\"button button-on\" href=\"/led2on\">Pedir</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}