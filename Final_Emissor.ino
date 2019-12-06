#include <SPI.h>
#include <RF24.h>                    // Includes the selected libraries.
#include <nRF24L01.h>
#include <PulseSensorPlayground.h>


int PulseSensorPurplePin = A0;        // Pin to which the sensor is connected
int LED13 = 5;                        // Pin to which the Led is connected

RF24 radio(9, 8);                    // CE, CSN

int Signal;                          // Holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 516;                 // Determines which Signal to "count as a beat", and which to ingore
int val = 0;
const byte address[6] = "00007";     // It sets a comunication channel. It is the same in both Arduinos.

void setup() {

  pinMode(LED13, OUTPUT);           // Pin that will blink to the user's heartbeat
  Serial.begin(9600);               // Sets up Serial Communication at certain speed
  radio.begin();

               

  radio.openWritingPipe(address);   // Sets the module as emitter

  radio.stopListening();          

}

void loop() {

  Signal = analogRead(PulseSensorPurplePin);  // Reads the PulseSensor's value and assigns this value to the "Signal" variable

  Serial.println(Signal);                     // Sends the Signal value to Serial Plotter to monitorise

  if (Signal > Threshold) {
    
                                              // If the signal is above the variable "Threshold", it turns on the LED.
    if (val == 0) {                           // In case the variable val is a valley but the signal is higher than the threshold, it sends an 1 to the receiver
      val = 1;
      radio.write(&val, sizeof(val));
    }
    digitalWrite(LED13, HIGH);

  } else {
    if (val == 1) {                           // In case the variable val is a high but the signal is lower than the threshold it sends a 0 to the receiver
      val = 0;
      radio.write(&val, sizeof(val));        
    }
    digitalWrite(LED13, LOW);                 // If the signal is below the variable "Threshold" it turns off Arduino's on-Board LED.
  }

  delay(10);
}
