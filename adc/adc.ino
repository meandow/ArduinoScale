#include <Wire.h>
#include <Adafruit_ADS1015.h>

 Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */
double varVolt = 1.12184278324081E-06;  // variance determined using excel and reading samples of raw sensor data
double varProcess = 1e-8;
double Pc = 0.0;
double G = 0.0;
double P = 1.0;
double Xp = 0.0;
double Zp = 0.0;
double Xe = 0.0;

unsigned int sensorValue[4] = {0,0,0,0};

void setup(void) 
{
  Serial.begin(9600);
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
   ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();
}

void loop(void) 
{
readSensor();
sendValues();
delay(10);
}

void readSensor(){
  int16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);
  double voltage = adc0;
  Pc = P + varProcess;
  G = Pc/(Pc + varVolt);
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(voltage-Zp)+Xp;
sensorValue[0] = Xe;
}

void sendValues(){
  Serial.print('#');
  for (int i = 0; i < 4; i++){
    Serial.print(sensorValue[i]);
    Serial.print('+');
  }
  Serial.print('~');
  Serial.println();
  delay(10);
}
