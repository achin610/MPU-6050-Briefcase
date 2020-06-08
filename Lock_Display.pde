import processing.serial.*; // add the serial library
Serial myPort; // define a serial port object to monitor

void setup() { // Sets up initial conditions for locked servo and draws it
 size(800,600);
  background (200, 200,200);
  fill(255,0,0);
  noStroke();
  rect(300,200,200,200,10);
  rect(350,150,100,50);
  ellipse(400,150, 100, 100);
  fill(200,200,200);
  ellipse(400,200, 50, 50);
  rect(375,150,50,50);
  ellipse(400,200,50,50);
  ellipse(400,150, 50, 50);
  textSize(32);
  fill(0,0,0);
  text("Locked",345,300);  
 myPort = new Serial(this, Serial.list()[0], 4800); // define input port
  myPort.clear(); // clear the port of any initial junk
}

void draw() {
  while (myPort.available () > 0) { // make sure port is open
  String inString = myPort.readStringUntil('\n'); // read input string
  
if (inString != null) { // ignore null strings
inString = trim(inString); // trim off any whitespace
String[] xyzaRaw = splitTokens(inString, "  "); // extract x & y into an array


if (xyzaRaw.length  == 1) {
  
  int a1= int(xyzaRaw[0]);
  
  //Using an if statements depending specific states of Arduino Code, processing will know what to display based on the string written on the serial monitor
  //1: If Serialprint displays a 1 in string, that means the password is correct and processing should display a green unlocked symbol
  //2: If Serialprint displays a 2 in string, this means that the Gyroscope has been activated in the while loop and the Arduino began saving data input into an array, displays a yellow lock to tell the user to start inputing the password
  //3" If Serialprint displays a 3 in string, this links to the idea that the briefcase is locked. This is found on the second interrupt and allows the processing code to show a locked symbol
  //Any other number: If Serialprint shows any other number then (In the arduino I just set it to 4), then it shows that the input is incorrect and displays that to the screen.
  
  if (a1 == 1) { // If Unlocked the processing will display a green unlocked lock on the screen
  fill(0,255,0);
  noStroke();
  rect(300,200,200,200,10);
  rect(350,150,100,50);
  ellipse(400,150, 100, 100);
  fill(200,200,200);
  ellipse(400,200, 50, 50);
  rect(375,150,50,50);
  ellipse(400,200,50,50);
  ellipse(400,150, 50, 50);
  rect(425,150,50,50);
  textSize(32);  
  fill(0,0,0);
  text("Granted",340,300);
  }

else if (a1 == 2) { // If the activation button is pressed this part of the code draws over what was previously drawn to create a yellow lock asking the user to input gestures
  fill(255,255,0);
  noStroke();
  rect(300,200,200,200,10);
  rect(350,150,100,50);
  ellipse(400,150, 100, 100);
  fill(200,200,200);
  ellipse(400,200, 50, 50);
  rect(375,150,50,50);
  ellipse(400,200,50,50);
  ellipse(400,150, 50, 50);
  textSize(32);  
  fill(0,0,0);
  text("Input",360,300);
  text("Password", 325,330);
  
}


else if (a1 ==3) { //Locked drawing command, this is the same as the setup but it draws over whatever is behind it to show taht it is locked.
  fill(255,0,0);
  noStroke();
  rect(300,200,200,200,10);
  rect(350,150,100,50);
  ellipse(400,150, 100, 100);
  fill(200,200,200);
  ellipse(400,200, 50, 50);
  rect(375,150,50,50);
  ellipse(400,200,50,50);
  ellipse(400,150, 50, 50);
  textSize(32);
  fill(0,0,0);
  text("Locked",350,300); 
}

else  { //Displays Incorrect. This is only used when incorrect gesture is put into while loop. It displays for only one second before it does back to displaying "lock". This is due to it being hardcoded into the Arduino
  fill(255,0,0);
  noStroke();
  rect(300,200,200,200,10);
  rect(350,150,100,50);
  ellipse(400,150, 100, 100);
  fill(200,200,200);
  ellipse(400,200, 50, 50);
  rect(375,150,50,50);
  ellipse(400,200,50,50);
  ellipse(400,150, 50, 50);
  textSize(32);
  fill(0,0,0);
  text("Incorrect",330,300);

 
}

  }
}
  }
}
