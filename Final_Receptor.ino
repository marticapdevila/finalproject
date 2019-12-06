#include <SPI.h>                                // Includes the selected libraries.
#include <nRF24L01.h>
#include <RF24.h>
#include <PulseSensorPlayground.h>


int vibra = 3;                                  // Pin to which the vibrator is connected
unsigned long t;                                // The variable t (time) is set as an unsigned long to allow more bytes of information
int minimum = 100;                              // The variable sets the minimum BPM for activating the vibration
int maximum = 150;                              // The variable sets the maximum BPM. If BPM is higher than that it will disable the vibration as it is probably a reading issue of the sensor.

RF24 radio(9, 8);                               // CE, CSN

const byte address[6] = "00007";                // It sets a comunication channel. It is the same in both Arduinos.

void setup()
{
  pinMode(3, OUTPUT);                           // Sets the pin 3 (vibrator) as an output.
  pinMode(LED_BUILTIN, OUTPUT);                 // Sets the board's own led as an output.
  while (!Serial);                              // Sets up Serial Communication at certain speed
    Serial.begin(9600);
  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  radio.startListening();                         //Set module as receiver
}

void loop()
{

  if (radio.available())                          // This way the motors are only activated when there is a radio activated
  {
    int val=0;                                    // val=0 means that there is a "low". 
    radio.read(&val, sizeof(val));                
    
    if (val == 1) {
      int bpm=60000/(millis()-t);                 // millis-t measures the time between two "highs" (val=1). 60000 is to convert it to bpm as it would give the result in beats per milliseconds.
      t=millis();                                 // Time is reset every time the BPM are calculated
      Serial.println(bpm);                        // Prints the BPM to comprove the values and tweak the program.
      
      if (bpm >= minimum && bpm <= maximum) {     // If BPM is in range, the led and the vibrator are activated.
      digitalWrite (3, HIGH);                     
      digitalWrite (LED_BUILTIN, HIGH);
      delay (1000);                               // It lets the vibrator on for a second, and then turns it off.
      digitalWrite (3, LOW);                     
      digitalWrite (LED_BUILTIN, LOW);
      delay (1000)                                // The vibrator is at least one second off after being activated.
      } else {                                    // If the bpm is out uf the range both the led and the vibrator are turned off.
        digitalWrite (3, LOW);
        digitalWrite (LED_BUILTIN, LOW);
      }
     delay (10);                                   // There is a little delay for improved stability.
    
    }
  }
}
