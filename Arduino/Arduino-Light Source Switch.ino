//Digital pin 2: Switch
//Digital pin 4 & 5: LED
//Digital pin 6: Servo

#include <Servo.h>

boolean isRight=false; //True if the output is on the right
int readValue=0; //Push button's status
int inByte = 0; //Incoming serial byte
int rightPosition=37; //Angle of the servo to push/pull the lever to the right
int leftPosition=97; //Angle of the servo to push/pull the lever to the left
int timeout=250; //Refractory period: won't answer in case an action is fired less than XXX msec fro mthe previous one


Servo myservo;

void setup() {
  pinMode(2, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  myservo.attach(6, rightPosition, leftPosition);
  changeSide(); //Initialises to the right side
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
}

// the loop function runs over and over again forever
void loop() {
  //Check action on the physical button
  readValue=digitalRead(2);
  if(readValue==HIGH){
    changeSide();
  }
  
  //Check for instructions on the serial port.
  //Uses the Vincent Unblitz VMM-D3/D4 communication protocol
  if (Serial.available() > 0) {
    //Get incoming byte:
    inByte = Serial.read();
    
    //Check for open channel 1, 2, 3 or all
    if(inByte==64 || inByte==66 || inByte==68 || inByte==70){
      isRight=true;
      changeSide();
    }
    
    //Check for close channel 1, 2, 3 or all
    if(inByte==65 || inByte==67 || inByte==69 || inByte==71){
      isRight=false;
      changeSide();
    }
  }
  delay(timeout);
}

// Changes the side of the switch
void changeSide(){
  isRight=!isRight;
  if(isRight==true){
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    myservo.write(rightPosition);
  }else{
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    myservo.write(leftPosition);
  }
  delay(timeout);
}
