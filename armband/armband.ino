#include <Adafruit_NeoPixel.h>

const int LEDPin = 6;
const int numPixels = 6; // Length of LED strip

const int temperaturePin = O; // AO
const int lowTemps[4] = {40, 30, 20, 10}; // Fahrenheit, increasing in severity
const int highTemps[4] = {70, 80, 90, 100}; // Fahrenheit, increasing in severity

const int UVPin = 1; // A1
const int UVThresholds[4] = {3, 5, 7, 9}; // UV index, increasing in severity

int temperature, UV;

Adafruit_NeoPixel pixels(numPixels, LEDPin, NEO_GRB + NEO_KHZ800); // Initialize pixels

void setup() {
    Serial.begin(9600);

    //Set the pin modes for the output(LEDS) and input(temperature and UV sensors)
    pinMode(LEDPin, OUTPUT);
    pinMode(temperaturePin, INPUT);
    pinMode(UVPin, INPUT);

    pixels.begin(); // Initialize the NeoPixel strip
}

void loop() {
    readSensors();
    displayUV();
    displayTemperature();
}

// Read the temperature and UV sensors
void readSensors() {
    temperature = analogRead(temperaturePin);
    //Unit conversion

    UV = analogRead(UVPin);
    //Unit conversion

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" degrees Fahrenheit");

    Serial.print("UV index: ");
    Serial.println(UV);
}

void displayUV() {
    // Light up a purple pixel for each UV threshold
    if (UV > UVThresholds[0]) {
        pixels.setPixelColor(0, pixels.Color(88, 77, 100)); // Light purple first pixel
    }

    if (UV > UVThresholds[1]) {
        pixels.setPixelColor(1, pixels.Color(75, 47, 96)); // Medium purple second pixel
    }

    if (UV > UVThresholds[2]) {
        pixels.setPixelColor(2, pixels.Color(105, 65, 136)); // Dark purple third pixel
    }

    if (UV > UVThresholds[3]) {
        flashColor(0, numPixels / 2, 105, 65, 136); // Flash purple for all pixels
    }

    pixels.show(); // Update the strip
}

void displayTemperature() {
    // Light up a blue pixel for each low temperature threshold
    if (temperature < lowTemps[0]) {
        pixels.setPixelColor(numPixels / 2, pixels.Color(173, 216, 230)); // Light blue first pixel
    }

    if (temperature < lowTemps[1]) {
        pixels.setPIxelColor(numPixels / 2 + 1, pixels.Color(90, 147, 219)); // Medium blue second pixel
    }

    if (temperature < lowTemps[2]) {
        pixels.setPixelColor(numPixels / 2 + 2, pixels.Color(0, 0, 255)); // Blue third pixel
    }

    if (temperature < lowTemps[3]) {
        flashColor(numPixels / 2, numPixels, 0, 0, 255); // Flash blue for all pixels
    }

    // Light up a red pixel for each high temperature threshold
    if (temperature > highTemps[0]) {
        pixels.setPixelColor(numPixels / 2, pixels.Color(255, 255, 0)); // Yellow first pixel
    }

    if (temperature > highTemps[1]) {
        pixels.setPixelColor(numPixels / 2 + 1, pixels.Color(255, 69, 0)); // Orange second pixel
    }

    if (temperature > highTemps[2]) {
        pixels.setPixelColor(numPixels / 2 + 2, pixels.Color(255, 0, 0)); // Red third pixel
    }

    if (temperature > highTemps[3]) {
        flashColor(numPixels / 2, numPixels, 255, 0, 0); // Flash red for all pixels
    }

    pixels.show(); // Update the strip
}

void flashColor(int startIndex, int endIndex, int r, int g, int b) {
    for (int i = startIndex; i < endIndex; i++) {
        pixels.setPixelColor(i, pixels.Color(r, g, b)); // Set color
        pixels.show(); // Update the strip
        delay(500); // Wait for 500 milliseconds
        pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off the pixel
        pixels.show(); // Update the strip
    }
}
 
/*
Ideas:
- Average out temperature and UV readings
- Break up reading sensors into separate functions
- Break up reading sensor and printing into separate functions
*/