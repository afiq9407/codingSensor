
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

char auth[] = "5f62dd009ce74b618cd761595f0452fb";

char ssid[] = "ai";
char pass[] = "satusampailapan";


#define DHTPIN 2
#define TRIGGER 4
#define ECHO 5
#define DHTTYPE DHT11

int pump = 14;
int moisture1 = 0;
//int moisture2 = 16;
int moisture1_value = 0;
//int moisture2_value = 0;
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float m1 = analogRead(moisture1);
  //float m2 = digitalRead(moisture2);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
/////////////////////////////////////////////////////////////// DHT11 sensor
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else {
    Serial.print("Humidity :");
    Serial.println(h);
    Serial.print("Temperature :");
    Serial.println(t);
  }

  Blynk.virtualWrite(V1, m1);
 // Blynk.virtualWrite(V0, m2);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  dht.begin();

  //pinMode(moisture2, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  ///////////////////////////////////////////////////////////////// ultrasonic sensor
  long duration, distance;
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance <= 20 ) {
    Blynk.notify("There's movement near your plants!");
  }
  /////////////////////////////////////////////////////////////////// moisture sensor
  moisture1_value = analogRead(moisture1);
 // moisture2_value = digitalRead(moisture2);
  moisture1_value = moisture1_value / 10;
 // moisture2_value = moisture1_value / 10;
  Serial.print("Moisture1 : ");
  Serial.println( moisture1_value);
//  Serial.print("Moisture2 : ");
//  Serial.println( moisture2_value);

  if ( moisture1_value == 0  )
  {
    Blynk.notify("Your plants need water");
    delay(1000);
  }

//  else {
//    digitalWrite(pump, LOW);
//    delay(15000);
//    }
  delay(1000);
}
