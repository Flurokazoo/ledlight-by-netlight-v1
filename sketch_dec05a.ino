#include <FastLED.h>
#include <math.h>

#define SENSOR1_ECHO    12
#define SENSOR1_TRIGGER 11
#define SENSOR2_ECHO    10
#define SENSOR2_TRIGGER 9
#define TOTAL_DISTANCE  300

#define NUM_LEDS    58
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)


CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

int durationSensor1;
int durationSensor2;
int distanceSensor1;
int distanceSensor2;


int program = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int width;
int prevWidth;
int currentDensity = 0;



DEFINE_GRADIENT_PALETTE( coldest ) {
    0, 157, 21,  2,
   35, 229,244, 16,
   73, 255, 44,  7,
  107, 142,  7,  1,
  153, 229,244, 16,
  206, 142,  7,  1,
  255, 135, 36,  0};
 
 
DEFINE_GRADIENT_PALETTE( cold ) {
    0, 184,  1,128,
  160,   1,193,182,
  219, 153,227,190,
  255, 255,255,255};
 
DEFINE_GRADIENT_PALETTE( warm ) {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};
 
DEFINE_GRADIENT_PALETTE( warmest) {
    0,  71,  3,  1,
   45, 128,  5,  2,
   84, 186, 11,  3,
  127, 215, 27,  8,
  153, 224, 69, 13,
  188, 229, 84,  6,
  226, 242,135, 17,
  255, 247,161, 79};



void setup() {
    delay( 3000 ); // power-up safety delay
    Serial.begin(115200);
    
    FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    Serial.print("turned on");
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    pinMode(SENSOR1_TRIGGER, OUTPUT); 
    pinMode(SENSOR1_ECHO, INPUT); 
    pinMode(SENSOR2_TRIGGER, OUTPUT); 
    pinMode(SENSOR2_ECHO, INPUT); 
}

void loop() {

  

  int density;

  

        digitalWrite(SENSOR1_TRIGGER, LOW);
        digitalWrite(SENSOR2_TRIGGER, LOW);
        delayMicroseconds(2);
  
        digitalWrite(SENSOR1_TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(SENSOR1_TRIGGER, LOW);

        durationSensor1 = pulseIn(SENSOR1_ECHO, HIGH);
        distanceSensor1 = durationSensor1*0.034/2;

        digitalWrite(SENSOR2_TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(SENSOR2_TRIGGER, LOW);

        durationSensor2 = pulseIn(SENSOR2_ECHO, HIGH);
        distanceSensor2 = durationSensor2*0.034/2;

        if(distanceSensor1 > TOTAL_DISTANCE / 2)
        {
          distanceSensor1 = TOTAL_DISTANCE / 2;
        }

        if(distanceSensor2 > TOTAL_DISTANCE / 2)
        {
          distanceSensor2 = TOTAL_DISTANCE / 2;
        }

        int measuredLength = TOTAL_DISTANCE - distanceSensor1 - distanceSensor2;

        
        density = DetermineDensity(measuredLength);
  
        
  

   SetProgram(density);     

          static uint8_t startIndex = 0;
      startIndex = startIndex + 1; /* motion speed */ 
      
    
        FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
  //FastLED.delay(1000 / UPDATES_PER_SECOND);

}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = NUM_LEDS; i > 0; i--) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void CreatePalette()
{
  
  if(currentDensity < 1)
  {
    currentPalette = coldest;
  
  }
  else if(currentDensity < 20)
  {
    currentPalette = cold;
 

  }
  else if(currentDensity < 30)
  {
    currentPalette = warm;


  }
  else
  {
    currentPalette = warmest;
 
  }
}


int DetermineDensity(float measuredLength){

  float distance = TOTAL_DISTANCE;

  float percentage = measuredLength / distance * 100;  

  if (percentage > 100)
  {
    percentage = 100.00;
  }
  else if (percentage < 0)
  {
    percentage = 0.00;
  }

  

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    Serial.println("<------------------------------->");
        Serial.println("Distance sensor 1:");
  Serial.println(distanceSensor1);
  Serial.println("<------------------------------->");

  Serial.println("<------------------------------->");
        Serial.println("Distance sensor 2:");
  Serial.println(distanceSensor2);
  Serial.println("<------------------------------->");

  Serial.println("<------------------------------->");
        Serial.println("PERCENTAGEEEEE:");
  Serial.println("<------------------------------->");
  Serial.println(percentage);
  Serial.println("<------------------------------->");

    // save the last time you blinked the LED
    previousMillis = currentMillis;

    if (percentage >= 0.00 && percentage < 21.00)
  {
            Serial.println("COLLDDD:");

    currentDensity = currentDensity - 3;
  }
  else if (percentage > 20.99 && percentage < 40.00)
  { 
            Serial.println("MEDIUM:");

    currentDensity = currentDensity - 1;
  }
  else if (percentage > 39.99 && percentage < 60.00)
  { 
            Serial.println("HIGH:");

            currentDensity = currentDensity + 2;

  }
  else if (percentage > 59.99 && percentage < 80.00 )
  { 
            Serial.println("VREY HIGH:");

        currentDensity = currentDensity + 4;

  }
  else if (percentage > 79.99 )
  { 
            Serial.println("fekking HIGH:");

        currentDensity = currentDensity + 8;

  } else
  {
            Serial.println("quick mahs:");
            Serial.println(percentage);
  }
  Serial.println("CURRENT DENSITY");
  Serial.println(currentDensity);

  }
  
  

  if(currentDensity > 100)
  {
    currentDensity = 100;
  } 
  else if(currentDensity < 0)
  {
    currentDensity = 0;
  }
    
}

int SetProgram(int density) {
  if (program == 0)

  {
        CreatePalette();
  }
}


