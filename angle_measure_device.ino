#define buzzerPin 2
#define greenLed 8
#define yellowLed 9
#define redLed 10
#define wiperPin A0

#include <Wire.h>    //Include wire library 
#include <MPU6050_light.h>  //Include library for MPU communication

MPU6050 mpu(Wire);   //Create object mpu

int sample = 0;
unsigned long long timer = 0;
float threshold = 30;
bool prevState = false;


void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Serial.begin(9600);    //Start serial communication
  Wire.begin();     
  mpu.begin();     
  Serial.print(F("MPU6050 status: "));
  Serial.println(F("Calculating offsets, do not move MPU6050"));   
  delay(1000);
  mpu.calcGyroOffsets();     //Calibrate gyroscope
  delay(2000);
  Serial.println("Done!\n");
  for(int i=0;i<100;i++){
    mpu.update();
  }
  sample = mpu.getAngleX();

}
void loop() {
  
  mpu.update();    //Get values from MPU
  timer++;
  if (timer == 300) { // print data every 100ms
    int diff = abs(sample - mpu.getAngleX());
    diff = min(360 - diff, diff);
    bool state = diff > threshold;
    if((float)diff / (float)threshold >= 1){
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(redLed, HIGH);
    }else if((float)diff / (float)threshold >= 0.6){
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else if((float)diff / (float)threshold >= 0.3){
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, LOW);
    }else{
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, LOW);
    }
    if(state != prevState){
      if(state == true){
        digitalWrite(buzzerPin, HIGH);
      }else{
        digitalWrite(buzzerPin, LOW);
      }
    }
    timer = 0;
    Serial.print(diff);
    Serial.print(" ");
    Serial.println((360 + int(mpu.getAngleX()))%360);
    delay(10);
    int analogValue = analogRead(wiperPin);
    int mappedValue = map(analogValue, 0, 1023, 0, 50);
    Serial.print("tresh: ");
    threshold = mappedValue;
    Serial.println(mappedValue);
    prevState = state;
  }
}
