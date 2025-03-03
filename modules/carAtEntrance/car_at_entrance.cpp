// Libraries
#include "mbed.h"
#include "arm_book_lib.h"
#define NIGHT_LEVEL 35   // Dusk level
#define DAY_LEVEL 40     // Daylight level
#define LIGHT_SENSOR_SAMPLES 10  // Number of samples to average

//Public Variables
bool Car_Detect = false;           


//Private Variables
static int accumulatedTimeDelay = 0;        // For tracking delay times
static float lightReadingsArray[LIGHT_SENSOR_SAMPLES];  // Array to store light readings
static int lightSampleIndex = 0;                 // Index for current sample

//Public Functions
float sensorUpdate() 
{
    // Add new reading to array
    lightReadingsArray[lightSampleIndex] = lightsens.read();
    lightSampleIndex++;
    
    // Reset index if we reach the end of array
    if (lightSampleIndex >= LIGHT_SENSOR_SAMPLES) {
        lightSampleIndex = 0;
    }
    
    // Calculate average
    float lightReadingsSum = 0.0;
    for (int i = 0; i < LIGHT_SENSOR_SAMPLES; i++) {
        lightReadingsSum += lightReadingsArray[i];
    }
    
    // Scale to 0-100 (higher number means darker)
    float currentLightLevel = (1.0 - (lightReadingsSum / LIGHT_SENSOR_SAMPLES)) * 100;
    
    // Check if the light level indicates dusk or darker
    if (currentLightLevel >= NIGHT_LEVEL) {
        Car_Detect = true;  // Set Car_Detect to true when it's dusk or darker
    } else if (currentLightLevel < DAY_LEVEL) {
        Car_Detect = false; // Reset when it's clearly daylight
    }
    
    return currentLightLevel;
}

void carAtEntrance()
{
    Car_Detect = false;  // Changed to match the convention in sensorUpdate
}
