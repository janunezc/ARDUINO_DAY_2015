/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#define trigPin 2
#define echoPin 3
#define led 11
#define led2 10

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  int distance = (int) readDistance();
  Serial.print("DISTANCE @ LOOP: ");
  Serial.println(distance);
  
  sendValueToPVCloud(distance);
  
  delay(1000);
}


long readDistance(){
    long duration, distance;
    digitalWrite(trigPin, LOW);  
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    
    if(distance <=0 || distance >=600) {
        distance = -1;      
    }
    return distance;
}

void sendValueToPVCloud(int value){
  String valueToSend = "";
  
  valueToSend = valueToSend + value;
  String pvcloudCommand = "node /home/root/pvcloud_api.js action='add_value' value='" + valueToSend + "' value_type='INT' value_label='HCSR04-DISTANCE' captured_datetime='2015-01-01+00:00' >> log.txt";
  Serial.println(pvcloudCommand);
  
  system ( pvcloudCommand.buffer );    
}


