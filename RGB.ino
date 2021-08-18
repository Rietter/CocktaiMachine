

int greenPin = 1;
int bluePin = 2;
int redPin = 3;


void setup() {

  // initialize the LED as an output:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);

// For LED

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

//-------------------------------

}


void loop(){ 
  
  Serial.println("The LED should be red.");
  setColor(255, 0, 0);  // red
  delay(1000);
  
  Serial.println("The LED should be blue.");
  setColor(0, 255, 0);  // blue
  delay(1000);
  
  Serial.println("The LED should be green.");
  setColor(0, 0, 255);  // green
  delay(1000);
}


void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
