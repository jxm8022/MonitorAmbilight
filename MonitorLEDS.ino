#include "FastLED.h"
#define NUM_LED 120
#define DATA_PIN 7
CRGB leds[NUM_LED];

#define NUM_DATA 362 // NUM_LED * 3 + 2
#define RECON_TIME 2000 // after x seconds idle time, send afk again.

uint8_t led_color[NUM_DATA];
int index = 0;
unsigned long last_afk = 0;
unsigned long cur_time = 0;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LED);
  FastLED.show(); // Initialize all pixels to 'off'
  Serial.begin(115200);
  Serial.print("ozy"); // Send ACK string to host

  for(;;){
    if (Serial.available() > 0) {
      led_color[index++] = (uint8_t)Serial.read();

      if (index >= NUM_DATA){
        Serial.write('y');
        last_afk =  millis();
        index = 0;    

        if ((led_color[0] == 'o') && (led_color[1] == 'z')){
          // update LEDs
          for(int i=0; i<NUM_LED; i++){
            int led_index = i*3 + 2;
            leds[i].setRGB(led_color[led_index], led_color[led_index+1], led_color[led_index+2]);
          }
          FastLED.show();
         }
      }
    } else{
        cur_time = millis();
        if (cur_time - last_afk > RECON_TIME){
          Serial.write('y');
          last_afk =  cur_time;
          index = 0;
          for(int i=0; i<NUM_LED; i++){
            leds[i].setRGB(0,0,0);
          }
          FastLED.show(); // added this line
        }

    }
  }
}

void loop() {
}
