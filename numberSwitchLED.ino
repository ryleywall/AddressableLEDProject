#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 13

#define NUM_LEDS 300

#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

uint32_t goodColors[7] = {strip.Color(255 , 0 , 0 , 0),
                           strip.Color(0 , 255 , 0 , 0),
                           strip.Color(0 , 0 , 255 , 0),
                           strip.Color(255 , 255 , 0 , 0),
                           strip.Color(255 , 100 , 0 , 0),
                           strip.Color(0 , 255 , 140 , 0),
                           strip.Color(0 , 0 , 0 , 255)};


#define SIZE_COLORS 7;

void colorPut(uint32_t color){
  for(int i = 0; i < strip.numPixels()-1; i++){
    strip.setPixelColor(i , color);
  }
  strip.show();
}


void colorWipe(uint32_t color , int timeDelay){
  for(int i = 0; i < strip.numPixels()-1; i++){
    strip.setPixelColor(i , color);
    strip.show();
    delay(timeDelay);
   if (keypad.getState() == RELEASED){
    break;
    };
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
      if(keypad.keyStateChanged()){
        return;
      }
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170){
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}


void switchOff(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i , 0 , 0 , 0 , 0);
  }
  strip.show();
}

void strobe(uint8_t wait){ //wait reccomendation: around 1000
    
  while(true){
    for(int i = 0; i < strip.numPixels();i++){
     strip.setPixelColor(i,255,255,255,0); 
    }
    strip.show();
    delay(wait);
    switchOff();
  }
}

void partyLights(){
  while(true){
    for(int i = 0; i < strip.numPixels()-1; i++){
      strip.setPixelColor(i, random(0,190), random(0,190), random(0,190), 0);
    }
    strip.show();
  };
}  

void randomGoodColors(){
  while (true){
    for(int i = 0; i < strip.numPixels(); i++){
       strip.setPixelColor(i, goodColors[random(0,6)]);
    }
    strip.show();
  }
}

void snake(int snakeLength , int numBites){

  switchOff();

  for(int i = 0; i < strip.numPixels()*20; i++){

   int pos = i - (300*(i/300));
    if(250 == strip.getPixelColor(pos + snakeLength -1)){

      snakeLength += 2;
      i -= 2;
    }
    Serial.print(pos);
    Serial.print("\n");
      while(pos == 0){
         for(int i = 0; i < numBites; i++){
    strip.setPixelColor(random(pos+snakeLength,299) , goodColors[2]);
  }
        pos++;
        };
    createSnake(snakeLength , pos);
    delay(10);
    strip.setPixelColor(pos-1 , 0 , 0 , 0 , 0);
    };
  }

void createSnake(int snakeLength , int intialPos){
  for(int i = 0; i < snakeLength; i++){
    int j = (intialPos + i);
    strip.setPixelColor(j - (300*(j/300)), goodColors[0]);
    //strip.show();
  }
  strip.show();
}



void setup() {

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); //pixels off

  Serial.begin(9600);
  
  for(int j = 0; j <= 255; j+=1){
    for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i , 0 , 0 , 0 , j);
    }
    strip.show();
  }
}

void loop() {
   char key = keypad.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '1': {
        colorPut(goodColors[0]);
        break;
      } 
      case '2': {
        uint32_t color2 = strip.Color(0 , 255 , 0 , 0);
        colorPut(color2); 
        break;
      }  
      case '3': {
        uint32_t color3 = strip.Color(0 , 0 , 255 , 0);
        colorPut(color3); 
        break;
      }  
      case '4': {
        uint32_t color4 = strip.Color(0 , 0 , 0 , 255);
        colorPut(color4); 
        break;
      }  
      case '5': {
        snake(5 , 5); 
        break;
      }  
      default: {
        switchOff();
      }  
    }
  }
}


   





