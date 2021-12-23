#include <FastLED.h>

const int NUM_SLIDERS = 6;
const int buttonInputs[NUM_SLIDERS] = {7, 6, 5, 4, 3, 2};
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A4, A5};

int buttonValues[NUM_SLIDERS];
int analogSliderValues[NUM_SLIDERS];

const int LED_PIN = 8;
CRGB leds[NUM_SLIDERS];


void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
    pinMode(buttonInputs[i], INPUT_PULLUP);
  }

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_SLIDERS);

  //set to a value 0-255. 255 is brighter
  FastLED.setBrightness(50);

  Serial.begin(115200);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
     buttonValues[i] = digitalRead(buttonInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    if(buttonValues[i] == HIGH) {
      String val = String((int)analogSliderValues[i]);
      builtString += val;
      leds[NUM_SLIDERS - 1 - i] = CRGB::Purple;
    }
    else {
      builtString += "0";
      leds[NUM_SLIDERS - 1 - i] = CRGB::Red;
    }

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  FastLED.show();
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
