#include <Wire.h>
#include <Adafruit_AS7341.h>
#include "BluetoothSerial.h"

// Pins
#define SDA_PIN 21
#define SCL_PIN 22
#define LDR1_PIN 34
#define LDR2_PIN 35
#define LDR3_PIN 32
#define TCA_ADDR 0x70

Adafruit_AS7341 sensor1 = Adafruit_AS7341();
Adafruit_AS7341 sensor2 = Adafruit_AS7341();
Adafruit_AS7341 sensor3 = Adafruit_AS7341();

BluetoothSerial SerialBT;
bool loggingEnabled = false;

void tcaSelect(uint8_t channel) {
  if(channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Sensor");
  Wire.begin(SDA_PIN, SCL_PIN);

  tcaSelect(0); sensor1.begin();
  tcaSelect(1); sensor2.begin();
  tcaSelect(2); sensor3.begin();

  Serial.println("Send 'start' or 'stop' via Bluetooth.");
}

void loop() {
  // Bluetooth control
  if(SerialBT.available()){
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();
    if(cmd.equalsIgnoreCase("start")) loggingEnabled = true;
    if(cmd.equalsIgnoreCase("stop")) loggingEnabled = false;
  }
  if(!loggingEnabled) return;

  // KY-018 Lux average
  int ldrPins[3] = {LDR1_PIN,LDR2_PIN,LDR3_PIN};
  float luxSum = 0;
  for(int i=0;i<3;i++){
    int raw = analogRead(ldrPins[i]);
    luxSum += raw*3.3/4095.0;
  }
  float avgLux = luxSum/3.0;

  // AS7341 readings
  Adafruit_AS7341* sensors[3] = {&sensor1,&sensor2,&sensor3};
  float avgBlueFraction=0, avgSpectral=0;
  int rawBlue[3]={0,0,0};
  int totalChannels=10;

  for(int i=0;i<3;i++){
    tcaSelect(i);
    sensors[i]->update();
    rawBlue[i]=sensors[i]->channel(AS7341_CH4);
    uint32_t sumCh=0;
    for(int ch=0;ch<totalChannels;ch++) sumCh+=sensors[i]->channel(ch);
    avgBlueFraction += rawBlue[i]/float(sumCh+0.001);
    avgSpectral += sumCh/float(totalChannels);
  }

  avgBlueFraction/=3.0;
  avgSpectral/=3.0;

  // Print single line for plotting
  Serial.print(avgLux); Serial.print(" "); // lux levels
  Serial.print(avgBlueFraction); Serial.print(" "); // blue fraction
  Serial.print(avgSpectral); Serial.print(" "); //spectral reading
  Serial.print(rawBlue[0]); Serial.print(" "); // blue wavelength 1
  Serial.print(rawBlue[1]); Serial.print(" ");// blue wavelength 2
  Serial.println(rawBlue[2]); //blue wavelength 3

  delay(5000); //every 5 sec
}