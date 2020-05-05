#define RELAY_0_PIN 23 // OK
#define RELAY_1_PIN 22 // OK
#define RELAY_2_PIN 21 // OK
#define RELAY_3_PIN 19 // OK
#define RELAY_4_PIN 18 // OK
#define RELAY_5_PIN 17 // OK

#define TRANSISTOR_0_PIN 25 // DAC1 OK
#define TRANSISTOR_1_PIN 26 // DAC2 OK
#define TRANSISTOR_2_PIN 27 // OK
#define TRANSISTOR_3_PIN 32 // OK
#define TRANSISTOR_4_PIN 33 // OK

#define THERMOMETER_0_PIN 16 // OK
#define ONE_WIRE_BUS THERMOMETER_0_PIN

#define UART_TX 1 // TX!
#define UART_RX 3 // RX!

#define WiFi_SSID "SSID"
#define WiFi_password "password"


const int PWM_FREQUENCY = 5000; //Hz
const int PWM_RESOLUTION = 10; //bits


/* V10-20 Relays
 * V20-30 Transistors
 * V0 - temperature
 */

#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>



char auth[] = "authtoken";
char ssid[] = WiFi_SSID;
char pass[] = WiFi_password;


OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);



BLYNK_CONNECTED() {
    Blynk.syncAll();
}

BLYNK_WRITE(V10){
    int pinValue = param.asInt();
    digitalWrite(RELAY_0_PIN,pinValue);
}
BLYNK_WRITE(V11){
    int pinValue = param.asInt();
    digitalWrite(RELAY_1_PIN,pinValue);
}
BLYNK_WRITE(V12){
    int pinValue = param.asInt();
    digitalWrite(RELAY_2_PIN,pinValue);
}
BLYNK_WRITE(V13){
    int pinValue = param.asInt();
    digitalWrite(RELAY_3_PIN,pinValue);
}
BLYNK_WRITE(V14){
    int pinValue = param.asInt();
    digitalWrite(RELAY_4_PIN,pinValue);
}
BLYNK_WRITE(V15){
    int pinValue = param.asInt();
    digitalWrite(RELAY_5_PIN,pinValue);
}

BLYNK_WRITE(V20){
    int pinValue = param.asInt();
    ledcWrite(0, pinValue);
}
BLYNK_WRITE(V21){
    int pinValue = param.asInt();
    ledcWrite(1, pinValue);
}
BLYNK_WRITE(V22){
    int pinValue = param.asInt();
    ledcWrite(2, pinValue);
}
BLYNK_WRITE(V23){
    int pinValue = param.asInt();
    ledcWrite(3, pinValue);
}
BLYNK_WRITE(V24){
    int pinValue = param.asInt();
    ledcWrite(4, pinValue);
}

BlynkTimer timer;

void requestSensor()
{
   sensors.requestTemperatures();
}

void recieveSensor()
{
   float temp = sensors.getTempCByIndex(0);
   if (isnan(temp)){
      Serial.println("błąd komunikacji");}
   else if (temp == 127){
       Serial.println("Problem z podłączeniem sensora");
   }
   else if ((temp>127)||(temp<-55)){
       Serial.println("problem z odczytem");
   }else{
    Blynk.virtualWrite(V0,temp);
   }
}


void setup()
{
  Serial.begin(115200);
  ledcSetup(0, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcSetup(1, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcSetup(2, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcSetup(3, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcSetup(4, PWM_FREQUENCY, PWM_RESOLUTION);

  ledcAttachPin(TRANSISTOR_0_PIN, 0);
  ledcAttachPin(TRANSISTOR_1_PIN, 1);
  ledcAttachPin(TRANSISTOR_2_PIN, 2);
  ledcAttachPin(TRANSISTOR_3_PIN, 3);
  ledcAttachPin(TRANSISTOR_4_PIN, 4);
  sensors.begin(); 
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  timer.setInterval(15000L, requestSensor);
  delay(300);
  timer.setInterval(15000L, recieveSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
