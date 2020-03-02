#include <ClickEncoder.h>
#include <TimerOne.h>
#include <HID-Project.h>
#include <EasyNeoPixels.h>

#define ENCODER_CLK A0
#define ENCODER_DT A1
#define ENCODER_SW A2

ClickEncoder *encoder; 
int16_t last, value;

void timerIsr() {
  encoder->service();
}

void setup() {
  Serial.begin(9600); // Opens the serial connection used for communication with the PC. 
  Consumer.begin(); // Initializes the media keyboard
  encoder = new ClickEncoder(ENCODER_DT, ENCODER_CLK, ENCODER_SW); // Initializes the rotary encoder with the mentioned pins

  Timer1.initialize(1000); // Initializes the timer, which the rotary encoder uses to detect rotation
  Timer1.attachInterrupt(timerIsr); 
  setupEasyNeoPixels(2, 2);
  last = -1;
  writeEasyNeoPixel(0, 255, 255, 255);
  writeEasyNeoPixel(1, 255, 255, 255);
  delay(1500);
} 

void loop() {  
  value += encoder->getValue();

  // This part of the code is responsible for the actions when you rotate the encoder
  if (value != last) { // New value is different than the last one, that means to encoder was rotated
    if(last<value){ // Detecting the direction of rotation
      Consumer.write(MEDIA_VOLUME_UP); // Replace this line to have a different function when rotating counter-clockwise
      writeEasyNeoPixel(1, 255, 0, 0);
      writeEasyNeoPixel(0, 0, 50, 0);
      delay(100);}
      else{
      Consumer.write(MEDIA_VOLUME_DOWN); // Replace this line to have a different function when rotating clockwise
      writeEasyNeoPixel(0, 255, 0, 0);
      writeEasyNeoPixel(1, 0, 50, 0);
      delay(100);}
    last = value; // Refreshing the "last" varible for the next loop with the current value
    Serial.print("Encoder Value: "); // Text output of the rotation value used manily for debugging (open Tools - Serial Monitor to see it)
    Serial.println(value);
  }

  // This next part handles the rotary encoder BUTTON
  ClickEncoder::Button b = encoder->getButton(); // Asking the button for it's current state
  if (b != ClickEncoder::Open) { // If the button is unpressed, we'll skip to the end of this if block
    switch (b) {
      case ClickEncoder::Clicked: // Button was clicked once
        Consumer.write(MEDIA_PLAY_PAUSE);
        writeEasyNeoPixel(0, 255, 255, 0);
        writeEasyNeoPixel(1, 255, 255, 0);
        delay(500);
      break;      
      
      case ClickEncoder::DoubleClicked: // Button was double clicked
         Consumer.write(MEDIA_NEXT);
         writeEasyNeoPixel(0, 0, 255, 0);
         writeEasyNeoPixel(1, 0, 255, 0);
         delay(500);
      break;      
    }
  }

  delay(10); // Wait 10 milliseconds, we definitely don't need to detect the rotary encoder any faster than that
  writeEasyNeoPixel(0, 0, 0, 255);
  writeEasyNeoPixel(1, 0, 0, 255);
  }
