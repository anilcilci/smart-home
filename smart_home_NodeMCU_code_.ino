#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <Servo.h>
Servo servoMotor;

SocketIOClient client;
const char* ssid     = "SSID";
const char* password = "**********";

char host[] = "IP ADDRESS";
int port = PORT;
extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
long interval = 1000;
unsigned long lastsend = 0;
String JSON;
int pirValue;
float tempValue;
int tempPin = 0;
int val;
int trigPin = 5; /* D1 Pin */
int echoPin = 4; /* D2 Pin */
int pirPin = 12; /* D6 Pin*/
const int alevPin= 2;/*D4*/
int ledPin=16;/*D0*/
int alevDeger;
long sure;
long uzaklik;
int motorPin=15;/*D8*/
int derece;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(alevPin, INPUT);
  servoMotor.attach(14);/*D5*/

  Serial.begin(9600);
  delay(10);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  if (client.connected()){
      client.send("connection", "message", "Connected !!!!");
    }
  }/*END SETUP*/

  void loop() {
    
      servo( mesafe() );
      client.send("receivedValue", "message", (String) receivedValue() );
     
    
      if (client.monitor()) {
        if(Rcontent=="aydinlat"){
          digitalWrite(ledPin, HIGH);
          }
        else if(Rcontent=="karart"){
          digitalWrite(ledPin, LOW);
          }
      } /* End CLIENT.MONITOR*/
      
    }/* End LOOP*/

  /*Sıcaklık Fonksiyonu*/
    float sicaklik(){
  val = analogRead(tempPin);
  float cel = (val/1.8)+32;
  if(cel>25){
    digitalWrite(motorPin,HIGH);}
    else if(cel<25){
      digitalWrite(motorPin,LOW);
      }
  return cel;
  }
  /*Hareket Fonksitonu*/
  int hareket(){
  pirValue=digitalRead(pirPin);
  delay(1000);
  return pirValue;
  }
  /*Mesafe Fonksiyonu*/
  int mesafe(){
    digitalWrite(trigPin, LOW); /* sensör pasif hale getirildi */
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */ 
    sure = pulseIn(echoPin, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
    uzaklik= sure /29.1/2; /* ölçülen sure uzaklığa çevriliyor */            
    delay(500); 
    return uzaklik;
  }
  /*Alev Fonksiyonu*/
  int alev(){
    alevDeger= digitalRead(alevPin); 
    return alevDeger; 
  }
  /*Servo Fonksiyonu*/
  void servo(int mesafe_deger){
  if(mesafe_deger>20 && derece!=90){
     servoMotor.write(97);
     derece=90;
  }
  else if(mesafe_deger<=20 && derece!=0){
     servoMotor.write(7);
     derece=0;
  }
}
/*JSON a gönderilen Fonksiyon*/
String receivedValue(){
  return (String)sicaklik() + "#" + (String)hareket() + "#" + (String)mesafe() + "#" + (String)alev() ;
  }
