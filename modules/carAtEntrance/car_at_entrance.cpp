//Libraries
#include "mbed.h"
#include "arm_book_lib.h"

#define NIGHT_LEVEL 35   // Car Detected
#define DAY_LEVEL 40     // No Car Detected
#define LIGHT_SENSOR_SAMPLES 10  // Number of samples to average

bool Car_Detect = false;           
int accumulatedTimeDelay = 0;        // For tracking delay times

float lightReadingsArray[LIGHT_SENSOR_SAMPLES];  // Array to store light readings
static int lightSampleIndex = 0;                 // Index for current sample

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
    return (1.0 - (lightReadingsSum / LIGHT_SENSOR_SAMPLES)) * 100;
}

void carAtEntrance()
{
    Car_Detect = 0;
}

