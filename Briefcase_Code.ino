// Gyroscope sensor and libraries were referenced from an arduino Motion Gesture Control video
// https://www.youtube.com/watch?v=7ZmmFVJ8dAI&list=LL&index=42&t=0s
#include "Wire.h"       
#include "I2Cdev.h"     
#include "MPU6050.h"
 

// Sensor Variables 
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
unsigned int sensorarrayX[250]; // Saves information from the sensor into an array
unsigned int sensorarrayY[250]; // Saves information from sensor into an array;

// Supporting electronics Variables
int buzzer = 8; //Establishes buzzer pin
int green = 7;  //Establishes rgb pin
int blue = 6;   //Establishes rgb pin
int red = 5;    //Establishes rgb pin
int greenlight =0;  //Establishes for loop condition for the color indicatator on briefcase
int i; // Establishes counter for for while loop

// Servo Variables
int servo = 9;  //Establishes Servo


// Interupt Variables Referenced from Arduino article on interrupts
// https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
volatile byte state = LOW;
volatile int pn = 0;



void setup()
{
  Serial.begin(4800); // Baud Rate
  //Establishes conditions for GY521 Sensor
  Wire.begin();
  mpu.initialize();

// Establishes variables
  pinMode(buzzer, OUTPUT); // Establishes buzzer as output;
  pinMode(green, OUTPUT);  // Establishes rgb output
  pinMode(blue, OUTPUT);   // Establish rgb output
  pinMode(red,OUTPUT);     // Establishes rgb output
  pinMode(servo, OUTPUT);

// Establishing Interrupts
// Creates an interupt pin based on the change this starts the sensing for the sensor using a while loop
  pinMode(interruptPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), turnon, CHANGE);
  
// Creates interrup pin to lock the suitcase based on change as well
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), turnoff, CHANGE); 
}



void loop(){

// LED indicator code, because its not in the while loop, it's always running and can identify when it's unlocked and locked
  if (greenlight == 1) { // Identifies what color the LED should be based on the greenlight indicator , if 1 == green
    digitalWrite(green, HIGH); 
    digitalWrite(red, LOW);
  }
  else { //if not 1 == its red
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }


// Until the first interrupt occurs, this while loop doesn't occur, When the interrupt occurs pn==1 allowing the sensor starts 
// recording data into an array.
  while (pn ==1) { // Sensing starts once once the first button is pressed
    //Turns LED yellow
    digitalWrite(green,HIGH); //Assists in turning the LED yellow 
    digitalWrite(red, HIGH);  //Assists in turning the LED yellow 

  
// Reads Gyro Position
  for (int i=1;i<250;i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    sensorarrayX[i] = map(ax, -16000, 18000, 0, 180); // X axis data
    sensorarrayY[i] = map(ay, -17000, 17000, 0, 180);  // Y axis data
// NOTE: raw data ranges from map were found from seeing what raw input values were when first using the sensor

// Due to noise or the atypical inputs for the sensor, if the sensor picks up a fast movement outside the typical range of the 
// raw value from the map, it retakes the data. This ensures that the code cannot be unlocked due to random shaking and that the 
// input is very deliberate.
    if (sensorarrayX[i] > 180 || sensorarrayY[i] > 180) {
      sensorarrayX[i]=map(ax, -16000, 18000, 0, 180); // X axis data;
      sensorarrayY[i]=map(ay, -17000, 17000, 0, 180);  // Y axis data;
    }
    else {
    }
    delay(10); // Delays so the total time collecting data to allow the user to put in the password is roughly 2.5 seconds
    }
    
// Reading Sesnor Data - Done after collecting the array data above
// The reading is done using if and while loops. Each while loop represents reading one of the input directions (tilt left, right, forward, backwards). 
// There are five inputs required to unlock the servo. For each input it goes through a while loop where it stays in and reads through the array until 
// that input is reached. Then it leaves the while loop before entering another while loop and repeating the process. If it reads through the entire 
// array before the while loops are done, they exist them and we know that because it left the while loop then the incorrect password was given.
    
    i=1; //initializes where to start reading (the first indice)
    while (sensorarrayY[i] > 45 && i < 250) { // Reads the array data, once it reads the correct gyro input from the array (or the array reaches its last indice) it exits the while loop
      i = i+1; 
    }
    
    if (i < 250) {  // Reads second step based on how many were counted for in the first one, once it reads the correct gyro input from the array (or the array reaches its last indice), it exits the while loop 
      while (sensorarrayX[i] > 45 && i < 250) { //gesture : up 
        i = i+1;
      }
    }
    else {
      pn=0;
    }
    
    if (i < 250) {  // Reads third step based on how many were counted for in the first one, once it reads the correct gyro input from the array (or the array reaches its last indice), it exits the while loop 
      while (sensorarrayX[i] < 135 && i < 250) { //gesture : up 
        i = i+1;
      }
    }
    else {
      pn=0;
    }
    if (i < 250) {  // Reads fourth step based on how many were counted for in the first one, once it reads the correct gyro input from the array (or the array reaches its last indice), it exits the while loop 
      while (sensorarrayX[i] > 45 && i < 250) { //gesture : up 
        i = i+1;
      }
    }
    else {
      pn=0;
    }
    if (i < 250) {  // Reads fourth step based on how many were counted for in the first one, once it reads the correct gyro input from the array (or the array reaches its last indice), it exits the while loop 
      while (sensorarrayY[i] < 135 && i < 250) { //gesture : up 
        i = i+1;
      }
    }
    else {
      pn=0;
    }



// Case where the servo unlocks once it reads through the array and if it didn't reach the end of the array, then based on the while loops above the correct code is has been inputted and the servo unlocks
    if (i <250) { // If the for loop is true then the correct X/Y angles were given within the bounds of the array and the servo should unlock
      
    for(int i=1; i<2; i++) {    // Buzzes once for audio feedback
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
    }
      for(int i=1; i<20;i++) { // PWM Servo Actuation
        digitalWrite(servo, HIGH);
        delayMicroseconds(700);
        digitalWrite(servo, LOW);
        delayMicroseconds(10000-700);
      }
      int unlock = 1; //Turns Lock Green in Processing
      String processing = String(unlock);
      Serial.println(processing);
      greenlight = 1;  // Turns LED green after it leaves the while loop
    }

    else { // If the incorrect input is given, the code will enter this else statement which displays a graphic on processing and activates a buzzer 
      int unlock = 4;       // Unlock isn't 1,2, or 3 so it shows that the password is incorrect in Processing
      String processing =  String(unlock);
        Serial.println(processing);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    for (i=0;i<3;i++) {     //Buzzes three times for audio feedback
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
    }
    delay(1000); // delays for 1 second before displaying the next processing screen
       unlock = 3; // Displays the Lock picture back on processing
      processing =  unlock;
      Serial.println(processing);
    }
    pn = 0; // Exits the while loop until the interrupt button is pressed again
  }
}


// Interupt Code 
// Referenced from Arduino Website
// https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
// Once the button is pressed, the while in void loop initiates allowing the sensor to start reading data into the array 
void turnon() { // Allows void loop to enter the while loop
    pn = 1; // Fulfills while loop 
    int unlock = 2; // Turns Lock Yellow in Processing
    String processing = String(unlock);
    Serial.println(processing);
}

// Once the button is pressed, pn goes back to a value of 0 allowing the void loop to exit the while code and returns to a locked state
void turnoff() { // Locks Servo
    pn=0; // Makes sure to exits the while loop
    greenlight = 0; // Turns green light off if it was on
    for (int i=1; i<20; i++) { // PWM Servo Code to lock it
      digitalWrite(servo, HIGH);
      delayMicroseconds(1500);
      digitalWrite(servo, LOW);
      delayMicroseconds(10000-1500);
    }
    int unlock = 3; // Dislays Locked screen in Processing
    String processing = String(unlock);
    Serial.println(processing);
}
  
