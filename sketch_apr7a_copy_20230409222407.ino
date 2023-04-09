#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(0, 1);

int pinNumberServo = 10;
int pinNumberButton = 2;
int pinNumberToggle = 4;

unsigned long timeStart = 0; // variable to store the start time of a button press
unsigned long timeEnd = 0; // variable to store the end time of a button press
String morseCode = "";
String text=""; // variable to store the decoded Morse code
int toggle = 0;
Servo myservo;


static String morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
"..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "!"
};

int dotDelay = 200;

void Morse_decode()
{
  int i = 0;
  while (morse[i] != "!")  
  {
    if (morse[i] == morseCode)
    {
      text += char('A' + i);
      break;
    }
    i++;
  }
}

void setup() {
    BTSerial.begin(9600);
    pinMode(pinNumberButton, INPUT_PULLUP);
    pinMode(pinNumberToggle, INPUT_PULLUP);
    myservo.attach(pinNumberServo);
    Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  int buttonState = digitalRead(pinNumberButton);
  int toggleState = digitalRead(pinNumberToggle);

  Serial.println(toggle);

  if(toggleState == LOW && toggle == 0){
    toggle = 1;
    delay(500);
  }else if(toggleState == LOW && toggle == 1){
    toggle = 0;
    delay(500);
  }


  if(toggle == 0){
    if(buttonState == LOW){
      myservo.writeMicroseconds(950);
      delay(200);
      myservo.writeMicroseconds(1000);
    }
  }else{
    
      if (buttonState == LOW) { // check if button is pressed
       timeStart = millis(); // record the start time of the button press

      while (buttonState == LOW) { // wait for the button to be released
        buttonState = digitalRead(pinNumberButton);
      }

      timeEnd = millis(); // record the end time of the button press

      long duration = timeEnd - timeStart; // calculate the duration of the button press

      if (duration < 100) { // if duration is less than 200 milliseconds, it's a dot
        morseCode += ".";
      } else { // if duration is greater than or equal to 200 milliseconds, it's a dash
        morseCode += "-";
      }

      delay(100); // wait for a short time before checking the button again
    }

    if (morseCode.length() > 0) { // check if there's any decoded Morse code
      Morse_decode();
      BTSerial.println(morseCode); // print the decoded Morse code to the serial monitor
      morseCode = ""; // reset the Morse code variable
    }
  }
  // put your main code here, to run repeatedly:

}
