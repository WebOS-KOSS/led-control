//-----------------------------------------------------------------

int ledState1 = LOW;
int previous1 = LOW;
int ledPin1 = 4;
int swPin1 = 0;

int ledState2 = LOW;
int previous2 = LOW;
int ledPin2 = 16;
int swPin2 = 5;

int ledState2 = LOW;
int previous2 = LOW;
int ledPin2 = 14;
int swPin2 = 13;

int ledState2 = LOW;
int previous2 = LOW;
int ledPin2 = 12;
int swPin2 = 15;

//------------------------------------------------------------------

void ledSet(int ledPin, int swPin){
  pinMode(ledPin, OUTPUT); // LED 핀 번호 설정
  pinMode(swPin, INPUT_PULLUP); // Tact 스위치 핀 번호 설정
}

//------------------------------------------------------------------

int ledControl(int ledPin, int swPin, int previous, int ledState){
  int reading = digitalRead(swPin);
  if(reading == HIGH && previous == LOW){//버튼 인식
    ledState = 1-ledState;  
    }
  digitalWrite(ledPin, ledState);
  Serial.print(digitalRead(ledPin));
  return reading;
}

//--------------------------------------------------------------------

void setup(){
  Serial.begin(115200);
  ledSet(ledPin1, swPin1);
  ledSet(ledPin2, swPin2);
  ledSet(ledPin3, swPin3);
  ledSet(ledPin4, swPin4);
}

//--------------------------------------------------------------------
void loop(){
  previous1 = ledControl(ledPin1, swPin1, previous1, digitalRead(ledPin1));
  previous2 = ledControl(ledPin2, swPin2, previous2, digitalRead(ledPin2));
  previous2 = ledControl(ledPin3, swPin3, previous3, digitalRead(ledPin3));
  previous2 = ledControl(ledPin4, swPin4, previous4, digitalRead(ledPin4));
  delay(1);
}
