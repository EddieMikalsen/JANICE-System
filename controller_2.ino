#include <Servo.h>                  // include servo library
Servo jtservo;                      // create servo object to control a servo
String inputString = "";            // a String to hold incoming data
bool stringComplete = false;        // whether the string is complete

void setup() {
  jtservo.attach(3);                // attach servo to specified PWM pin
  delay(2000);
  Serial.begin(115200);             // initialize serial
  Serial.println("Begin");
  inputString.reserve(200);         // reserve 200 bytes for the inputString
}

// this loop will convert the serial moniter input (desired TV angle) to a target servo position and write that position to the servo
void loop() {
  if (stringComplete) {             // if input string is marked as complete (see serial.Event()) do the following:
    int ival = inputString.toInt(); // convert serial moniter input string to integer (absoulute value is rounded down if decimal input)
    Serial.println(ival);           // print input (for debugging)
    inputString = "";               // clear the string
    stringComplete = false;
    double target = -ival - 3;      // convert integer input to double, flip sign, and subtract 3 (3 is measured from offset due to driving grear teeth)
    double fpos = 0.0022*(target*target*target)+0.0371*(target*target)+2.9744*(target)+76.222; // governing equation from MATLAB
    int newposition = fpos;         // convert position back to integer (servo accuracy likley cannot do decimals)
    jtservo.write(newposition);     // send position to servo
  }
}

// since there is no delay or loop within the main loop() above the serialEvent() function will start without being called
// this loop will check the serial moniter for complete inputs
void serialEvent() {
  while (Serial.available()) {              // while the serial moniter is open do the following:
    char inChar = (char)Serial.read();      // read character from the serial moniter
    if (inChar == '\n' || inChar == '\r')   // if "Enter" or "Send" are pressed, the command line is set as a complete string
      stringComplete = true;
    else                                    // if "Enter" or "Send" are not pressed, the length of the string is increased
      inputString += inChar;                
  }
}
