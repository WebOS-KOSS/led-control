int led_pin = 5;
int sw_pin = 16;
int led_state = LOW;

void setup(){
  Serial.begin(9600);
  pinMode(sw_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
}

void loop(){
  if(digitalRead(sw_pin)==0){
    if(led_state==LOW){
      led_state = HIGH;
      digitalWrite(led_pin, led_state);
    }
    else{
      led_state = LOW;
      digitalWrite(led_pin, led_state);
    }
    delay(1000);
  }
  digitalWrite(led_pin, led_state);
}
