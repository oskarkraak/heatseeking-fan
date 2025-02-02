#include <dht11.h>
#include <Servo.h>


dht11 DHT11;
Servo servo1;
Servo servo2;
int servoPosDegrees = 0;
int DELTA_TEMP = 2;
int DELAY_MILLIS = 4000;
int SERVO_DEFAULT_OFFSET_DEGREES = 125;


int servo1Pin = 2;
int servo2Pin = 3;
int sensorPins[] = {4, 5, 6, 7};
int ledPins[] = {10, 11, 12, 13};


int numSensors;


void  setup()
{
 Serial.begin(9600);


 numSensors = sizeof(sensorPins)/sizeof(sensorPins[0]);
  for (int ledPin: ledPins) {
   pinMode(ledPin, OUTPUT);
 }
 servo1.attach(servo1Pin);
 servo2.attach(servo2Pin);
}


void loop()
{
 Serial.println();


 for (int ledPin: ledPins) {
   digitalWrite(ledPin, LOW);
 }


  // get max temp
 int temperatures[numSensors];
 int maxTemp = 0;
 int maxTempIndex = 0;
 for (int i = 0; i < numSensors; i++) {
   temperatures[i] = readTemperature(sensorPins[i]);
   Serial.print("temp. ");
   Serial.print(i);
   Serial.print(" - ");
   Serial.println(temperatures[i]);
   if (temperatures[i] > maxTemp) {
     maxTemp = temperatures[i];
     maxTempIndex = i;
   }
 }
 Serial.print("max temp pin: ");
 Serial.println(sensorPins[maxTempIndex]);
 digitalWrite(ledPins[maxTempIndex], HIGH);
 servoPosDegrees = (numSensors - maxTempIndex) * 90;
 // check neighbors for similar temp
 int left = maxTempIndex - 1;
 if (left < 0) {
   left = numSensors-1;
 }
 int right = (maxTempIndex+1)%4;
 int higherNeighbor;
 int offsetDegrees;
 if (temperatures[left] > temperatures[right]) {
   higherNeighbor = left;
   offsetDegrees = -45;
 }
 else {
   higherNeighbor = right;
   offsetDegrees = 45;
 }
 Serial.print("second max temp pin (neighbor): ");
 if (temperatures[higherNeighbor] > temperatures[maxTempIndex]-DELTA_TEMP) {
   // middle
   Serial.println(sensorPins[higherNeighbor]);
   digitalWrite(ledPins[higherNeighbor], HIGH);
   servoPosDegrees = servoPosDegrees-offsetDegrees; // subtract offsetDegrees because the motor is in the opposite direction to increasing pins numbers
 } else {
   Serial.println("none above threshold");
 }

 // set servo

 servoPosDegrees = servoPosDegrees + SERVO_DEFAULT_OFFSET_DEGREES;
 if (servoPosDegrees >= 360) {
   servoPosDegrees = servoPosDegrees - 360;
 }
 if (servoPosDegrees < 0) {
   servoPosDegrees = servoPosDegrees + 360;
 }

 servo1.write(servoPosDegrees/2);
 servo2.write(servoPosDegrees/2);
 Serial.print("direction: ");
 Serial.println(servoPosDegrees);

 delay(DELAY_MILLIS);

}


int readTemperature(int pin) {
 //int chk = DHT11.read(DHT11PIN);
 DHT11.read(pin);


 Serial.print(pin);
 Serial.print(" - Humidity (%): ");
 Serial.println((float)DHT11.humidity, 2);


 Serial.print(pin);
 Serial.print(" - Temperature  (C): ");
 Serial.println((float)DHT11.temperature, 2);


 return DHT11.temperature;
}
