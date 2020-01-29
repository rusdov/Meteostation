#define BLYNK_PRINT Serial
#include <Adafruit_BMP280.h>
#include "HTU21D.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h> 


SimpleTimer timer;

//Variables:
float hum;
float temp;
int setAirQ;
int airQ;
float tempbmp;
int pressure;
int alt;


HTU21D myHumidity;
Adafruit_BMP280 bmp; 

WidgetLED led2(V12);

//blynk auth
char auth[] = "----------------------";
char ssid[] = "----------------------";
char pass[] = "----------------------";

BLYNK_WRITE(5){
setAirQ = param.asInt();
}

void gy21(){
hum = myHumidity.readHumidity();
temp = myHumidity.readTemperature();
  
Serial.print(F("  GY-21:"));
Serial.print(F("Temperature:"));
Serial.print(temp, 1);
Serial.print("C");
Serial.print(F(" Humidity:"));
Serial.print(hum, 1);
Serial.println();

Blynk.virtualWrite(V9, hum);
Blynk.virtualWrite(V10, temp);
}

void GetAirQ(){
float airQ = analogRead(A0); 
Serial.print("value :");
Serial.println(airQ);

Blynk.virtualWrite(V11, airQ);
}

void bmp280(){
Serial.println(F("  BMP280:"));

if (!bmp.begin()) {
  Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
   while (1);
  }  
tempbmp = bmp.readTemperature(); 
pressure = bmp.readPressure();
alt = bmp.readAltitude(1013.25);

Serial.print(F("Temperature = "));
Serial.print(tempbmp, 1);
Serial.println(" *C");

Serial.print(F("Pressure = "));
Serial.print(pressure, 1);
Serial.println(" Pa");

Serial.print(F("Approx altitude = "));
Serial.print(alt, 1); /* Adjusted to local forecast! */
Serial.println(" m");

Serial.println(); 

Blynk.virtualWrite(V12, tempbmp);
Blynk.virtualWrite(V13, pressure);
Blynk.virtualWrite(V14, alt);

}

void AirAlarm(){
if (airQ > setAirQ){
  led2.on();  
}
else{
  led2.off();
}
}

void setup() {

Serial.begin(9600);
myHumidity.begin();   

Blynk.begin(auth, ssid, pass);

//timers
timer.setInterval(1000,GetAirQ);
timer.setInterval(1000,AirAlarm);
timer.setInterval(1000,gy21);
timer.setInterval(1000,bmp280);

//pimode
}
BLYNK_CONNECTED(){
  Blynk.syncAll();
 }


void loop() {
  
Blynk.run(); 
timer.run();  
}
