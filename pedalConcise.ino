/*Control code for a guitar pedal switching board which changes audio signal path
  due to either transient peaks of an incomming signal, or the position of a 
  pot (also can use an expression pedal)
  features two bypass modes 
  1. bypasses all effects for clean signal
  2  passes through all effects
*/



const int buttonPin = 2;     // the number of the pushbutton pin
const int changePin = 4;     // the number of the pushbutton pin
const int aroundPin = 5;     // the number of the pushbutton pin
const int thruPin = 6;     // the number of the pushbutton pin

const int thruIndicator = 7;
const int aroundIndicator = 8;
const int aPath = 9;
const int bPath = 10;
const int cPath = 11;
const int dPath = 12;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int changeState = 0;         // variable for reading the pushbutton status
int aroundState = 0;         // variable for reading the pushbutton status
int thruState = 0;         // variable for reading the pushbutton status
int ranNum;
int ranDel;
int previousRanNum = 0;
int lastHigh = 0;
int progPin = 3;             //analog input for pot  





void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(thruIndicator, OUTPUT);  
  pinMode(aroundIndicator, OUTPUT);
  pinMode(aPath, OUTPUT); //9
  pinMode(bPath, OUTPUT); //10
  pinMode(cPath, OUTPUT); //11
  pinMode(dPath, OUTPUT); //12
  pinMode(buttonPin, INPUT); 
  pinMode(changePin, INPUT);
  pinMode(aroundPin, INPUT); 
  pinMode(thruPin, INPUT);
}

void loop() {
  aroundState = digitalRead(aroundPin);
  thruState = digitalRead(thruPin);
  changeState = digitalRead(changePin);
  buttonState = digitalRead(buttonPin); 
  if (aroundState == HIGH){
    bypassAround(); //no effects
  }
  if (thruState == HIGH){  // used to just be else
    bypassThru(); //all effects
  }
  if (thruState == LOW && aroundState == LOW){
    digitalWrite(aroundIndicator, LOW);
    digitalWrite(thruIndicator, LOW);
  }
  if (changeState == HIGH){                                                        
    do{             
      ranNum = random (aPath,dPath+1);
    } while (ranNum == lastHigh);
    while (buttonState == HIGH && aroundState == LOW && thruState == LOW){ 
      transientControl(); // audio peaks change effect path
    }
  } 
  if(aroundState == LOW && thruState == LOW && changeState == LOW) {
    potControl(); // potentiometer chnges effect path
  }
}


void transientControl(){
  changeState = digitalRead(changePin); // check which mode we are in
  aroundState = digitalRead(aroundPin);
  thruState = digitalRead(thruPin);  
  buttonState = digitalRead(buttonPin); //read the push button
  delay (10);
  digitalWrite(aPath, LOW);  
  digitalWrite(bPath, LOW);
  digitalWrite(cPath, LOW);
  digitalWrite(dPath, LOW);
  digitalWrite(ranNum, HIGH);
  lastHigh = ranNum; 
}


void potControl(){
  int progReading = map(analogRead(progPin), 0, 1023, aPath, dPath);      // put xPaths in an array for this part - generalises better
  digitalWrite(aPath, LOW);
  digitalWrite(bPath, LOW);
  digitalWrite(cPath, LOW); 
  digitalWrite(dPath, LOW);  
  digitalWrite(progReading, HIGH);
}

void bypassThru(){
  digitalWrite(aPath, LOW);  
  digitalWrite(bPath, LOW);
  digitalWrite(cPath, LOW);
  digitalWrite(dPath, LOW); 
  digitalWrite(aroundIndicator, LOW);
  digitalWrite(thruIndicator, HIGH);// thru indicator
}

void bypassAround(){
  digitalWrite(aPath, LOW);  
  digitalWrite(bPath, LOW);
  digitalWrite(cPath, LOW);
  digitalWrite(dPath, LOW); 
  digitalWrite(aroundIndicator, HIGH); // around indicator
  digitalWrite(thruIndicator, LOW);  
}




