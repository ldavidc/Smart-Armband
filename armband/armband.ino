#include <Adafruit_NeoPixel.h>

const int LEDPin = 6;
const int numPixels = 8; // Length of LED strip
const int brightness = 20;

const int temperaturePowerPin = 2; // 2
const int temperaturePin = 1; // A1
const int lowTemps[4] = {40, 30, 20, 10}; // Fahrenheit, increasing in severity
const int highTemps[4] = {70, 80, 90, 100}; // Fahrenheit, increasing in severity

// Temperature conversion variables
int Vo;
float R1 = 10000; // value of R1 on board
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; // Steinhart-Hart coefficients

const int UVPin = 2; // A2
const int UVThresholds[4] = {3, 5, 7, 9}; // UV index, increasing in severity

int temperature = 55; // Default value in the middle of the range
int UV = 0; // Default value
int timer = 0;
const int timerPeriod = 10;

Adafruit_NeoPixel pixels(numPixels, LEDPin, NEO_GRB + NEO_KHZ800); // Initialize pixels

void setup() {
    Serial.begin(9600);

    // Set power pins for the sensors

    // Set the pin modes for the output(LEDS) and input(temperature and UV sensors)
    pinMode(LEDPin, OUTPUT);
    pinMode(temperaturePowerPin, OUTPUT);

    pinMode(temperaturePin, INPUT);
    pinMode(UVPin, INPUT);

    pixels.begin(); // Initialize the NeoPixel strip
    pixels.setBrightness(brightness); // Turn down brightness because it's too bright otherwise
}

void loop() {
    readSensors();
    displayUV();
    displayTemperature();
    displayOnStatus(); // Display that the armband is on

    if (timer > timerPeriod) {
        timer = 0;
    } else {
        timer += 1;
    }
}

// Read the temperature and UV sensors
void readSensors() {
    // Read the temperature sensor
    digitalWrite(temperaturePowerPin, HIGH); // Turn on the temperature sensor
    Vo = analogRead(temperaturePin);
    R2 = R1 * Vo/(1023 - Vo); //calculate resistance on thermistor (https://arduinomodules.info/ky-013-analog-temperature-sensor-module/)
    logR2 = log(R2);
    temperature = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)) * 1.05; // temperature in Kelvin
    temperature = temperature - 273.15; //convert Kelvin to Celcius
    temperature = (temperature * 9.0)/ 5.0 + 32.0; //convert Celcius to Farenheit

    // Read the UV sensor
    UV = analogRead(UVPin);
    UV = max((0.0106 * UV) - 1.3577, 0); // Unit conversion

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" degrees Fahrenheit");

    Serial.print("UV index: ");
    Serial.println(UV);
}

void displayUV() {
    // Clear UV pixels
    for (int i = 0; i < numPixels / 2; i++) {
            pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off all pixels
    }

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
        flashColor(0, 3, 105, 65, 136); // Flash purple for all pixels
    }

    pixels.show(); // Update the strip
}

void displayTemperature() {
    // Clear temperature pixels
    for (int i = 3; i < numPixels; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off all pixels
    }

    // Light up a blue pixel for each low temperature threshold
    if (temperature < lowTemps[0]) {
        pixels.setPixelColor(3, pixels.Color(173, 216, 230)); // Light blue first pixel
    } 

    if (temperature < lowTemps[1]) {
        pixels.setPixelColor(4, pixels.Color(90, 147, 219)); // Medium blue second pixel
    }

    if (temperature < lowTemps[2]) {
        pixels.setPixelColor(5, pixels.Color(0, 0, 255)); // Blue third pixel
    }

    if (temperature < lowTemps[3]) {
        flashColor(3, 6, 0, 0, 255); // Flash blue for all pixels
    }

    // Light up a red pixel for each high temperature threshold
    if (temperature > highTemps[0]) {
        pixels.setPixelColor(3, pixels.Color(255, 255, 0)); // Yellow first pixel
    }

    if (temperature > highTemps[1]) {
        pixels.setPixelColor(4, pixels.Color(255, 69, 0)); // Orange second pixel
    }

    if (temperature > highTemps[2]) {
        pixels.setPixelColor(5, pixels.Color(255, 0, 0)); // Red third pixel
    }

    if (temperature > highTemps[3]) {
        flashColor(3, 6, 255, 0, 0); // Flash red for all pixels
    }

    pixels.show(); // Update the strip
}

void displayOnStatus() {
    for (int i = 6; i < 8; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 128, 0)); // Set color
    }
    pixels.show(); // Update the strip
}

void flashColor(int startIndex, int endIndex, int r, int g, int b) {
    if(timer < timerPeriod/2) {
        for (int i = startIndex; i < endIndex; i++) {
            pixels.setPixelColor(i, pixels.Color(r, g, b)); // Set color
        }
    } else {
        for (int i = startIndex; i < endIndex; i++) {
            pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off the pixel
        }
    }
}
 
/*
Ideas:
- Average out temperature and UV readings
- Break up reading sensors into separate functions
- Break up reading sensor and printing into separate functions
*/