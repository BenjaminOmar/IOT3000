//This code is designed for a system detection smoke through digital setup

//Define pin number for the smoke sensor  
int smokePinD = 2;

void setup() {
  //Start the serial communication at a baud rate of 19200
  Serial.begin(19200);
  //Set the pin mode for the smoke sensor
  pinMode(smokePinD, INPUT);  
  //delay for 5 seconds to allow initialization
  delay(5000);
}

void loop() {
  //Check the digital state of the smoke sensor pin
  if (digitalRead(smokePinD) == HIGH) {
    //If the smoke sensor reads HIGH, indicate smoke is detected
    Serial.println("Warning, value high");//Print a warning message    
  } else {
    //If the smoke sensor reads LOW, indicate no smoke is detected
    Serial.println("Normal values"); //Print a normal message
  }
  //Delay for 5 seconds before checking again
  delay(5000);
}
