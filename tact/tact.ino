#include <ESP8266WiFi.h> // Wifi 라이브러리 추가
#include <PubSubClient.h> // MQTT client 라이브러리 

//----------------------------------------------------------------

const char* ssid = "wifi"; //사용하는 Wifi 이름
const char* password = "12345678"; // 비밀번호
const char* mqtt_server = "192.168.232.218"; // MQTT broker ip
const char* clientName = "ledController"; // client 이름

WiFiClient espClient; // 인터넷과 연결할 수 있는 client 생성
PubSubClient client(espClient); // 해당 client를 mqtt client로 사용할 수 있도록 설정

//-----------------------------------------------------------------

int ledState1 = LOW;
int previous1 = LOW;
int ledPin1 = 4;
int swPin1 = 0;

int ledState2 = LOW;
int previous2 = LOW;
int ledPin2 = 16;
int swPin2 = 5;

int ledState3 = LOW;
int previous3 = LOW;
int ledPin3 = 14;
int swPin3 = 12;

int ledState4 = LOW;
int previous4 = LOW;
int ledPin4 = 1;
int swPin4 = 15;

//----------------------------------------------------------------

void setup_wifi() {
   delay(10);
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   while(WiFi.status() != WL_CONNECTED)
   {
     delay(500);
     Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP()); 
}

//------------------------------------------------------------------

void ledSet(int ledPin, int swPin){
  pinMode(ledPin, OUTPUT); // LED 핀 번호 설정
  pinMode(swPin, INPUT_PULLUP); // Tact 스위치 핀 번호 설정
}

//--------------------------------------------------------------------

void reconnect() {
  //연결될 때까지 시도
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientName))
    {
      //연결 성공
      Serial.println("connected");
      client.subscribe("subLed1"); // led 토픽 구독
      client.subscribe("subLed2"); // led 토픽 구독
      client.subscribe("subLed3"); // led 토픽 구독
      client.subscribe("subLed4"); // led 토픽 구독
    } 
    else 
    {
      //연결실패하면 현재 상태 출력하고 5초 후 다시 시도
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//-------------------------------------------------------------------

void callback(char* topic, byte* payload, unsigned int uLen) {
  char pBuffer[uLen+1];
  int i;
  int ledTemp = -1;
  for(i = 0; i < uLen; i++)
  {
    pBuffer[i]=(char)payload[i];
  }
  if((String)topic == "subLed1"){
    ledTemp = ledPin1;
  }
  else if((String)topic == "subLed2"){
    ledTemp = ledPin2;
  }
  else if((String)topic == "subLed3"){
    ledTemp = ledPin3;
  }
  else if((String)topic == "subLed4"){
    ledTemp = ledPin4;
  }
  Serial.print(ledTemp);
  Serial.print("Subscribe ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(pBuffer); // 1 or 0
  if(pBuffer[0]=='1')
  {
    digitalWrite(ledTemp, HIGH); // 1이면 led 켜기
  }
  else if(pBuffer[0]=='0')
  {
    digitalWrite(ledTemp, LOW); // 0면 led 끄기
  } 
}

//------------------------------------------------------------------

int ledControl(int ledPin, int swPin, int previous, int ledState, char* ledNum){
  int reading = digitalRead(swPin);
  if(reading == HIGH && previous == LOW){//버튼 인식
    ledState = 1-ledState;
    char ledMessage[13] = ""; // 문자열을 위한 공간 마련
    sprintf(ledMessage, "{\"ledState\":%d}", ledState);
    Serial.print("Publish message: ");
    Serial.print(ledMessage);
    Serial.println(ledNum);
    client.publish(ledNum, ledMessage); // 만든 문자열을 mqtt 서버에 publish *토픽에 숫자 XXX
    }
  digitalWrite(ledPin, ledState);
  return reading;
}

//--------------------------------------------------------------------

void setup(){
  Serial.begin(115200);
  setup_wifi(); //wifi 연결
  
  ledSet(ledPin1, swPin1);
  ledSet(ledPin2, swPin2);
  ledSet(ledPin3, swPin3);
  ledSet(ledPin4, swPin4);
  
  client.setServer(mqtt_server, 1883); //mqtt 서버와 연결(ip, 1883)
  client.setCallback(callback); //callback 함수 세팅
}

//--------------------------------------------------------------------

void loop(){
  if (!client.connected()){reconnect();} //mqtt 연결이 안되어있다면 다시 연결
  client.loop(); //연결을 계속해서 유지하고 들어오는 메시지를 확인할 수 있도록 함
  previous1 = ledControl(ledPin1, swPin1, previous1, digitalRead(ledPin1), "led1");
  previous2 = ledControl(ledPin2, swPin2, previous2, digitalRead(ledPin2), "led2");
  previous3 = ledControl(ledPin3, swPin3, previous3, digitalRead(ledPin3), "led3");
  previous4 = ledControl(ledPin4, swPin4, previous4, digitalRead(ledPin4), "led4");
  delay(10);
}
