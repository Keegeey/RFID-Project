#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "credentials.h"

#include <Servo.h>
#define SERVO_PIN 9
Servo myServo;

#define LED_PIN 17

#define TOPIC "uark/csce5013/gbgoode"
#define USERNAME "gbgoode"
char ssid[] = SSID;
char password[] = PASSKEY;
// Status
int status = WL_IDLE_STATUS;
const char* mqtt_server = "broker.hivemq.com";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
byte mac[6];  
long lastMsg = 0;
char msg[100];
int value = 0;

char* Topic;
char buffer[20];
boolean Rflag=false;
int r_len;

void setup_wifi(){
  delay(1000);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.macAddress(mac);
  Serial.println();
  Serial.print("Mac address ");
  int i = 0;
  for(i = 0; i<6; i++){
    Serial.println(mac[i],HEX);
  }
  Serial.println();
  
  while (status != WL_CONNECTED) {
    //status = WiFi.begin(ssid, password);
    status = WiFi.begin(ssid);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Rflag = true; //will use in main loop
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int i = 0;
  for (i; i < length; i++) {
    buffer[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  }
  buffer[i]='\0'; //terminate string

  Serial.println("");
  if(strcmp(buffer, "open") == 0){
    Rflag = true;
    Serial.println("Open lock");
    //digitalWrite(LED_PIN,LOW);
  }
  else{
    //digitalWrite(LED_PIN,HIGH);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(USERNAME)) {
      Serial.println("connected");
      // Once connected, subscribe to input channel
      client.subscribe(TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning program...");
  
  myServo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  static int servoPosition = 180;
  static int servoLastChanged = 0;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(Rflag)
  {
    digitalWrite(LED_PIN, HIGH);
    myServo.write(0);
    delay(1000);
    Rflag = false;
  }
  else{
    digitalWrite(LED_PIN, LOW);
    myServo.write(180);
  }
  //
  //
  //delay(1000);
  //
  //myServo.write(180);
  delay(1000);
}
