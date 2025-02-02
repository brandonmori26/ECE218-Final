//Libraries
#include "mbed.h"
#include "arm_book_lib.h"

#define NIGHT_LEVEL 35   // Dusk level
#define DAY_LEVEL 40     // Daylight level
#define LIGHT_SENSOR_SAMPLES 10  // Number of samples to average
#define TIME_INCREMENT_MS 50     // Main loop delay time
#define AUTO_THRESHOLD 0.66     // Potentiometer threshold for AUTO
#define OFF_THRESHOLD 0.33      //Potentiometer threshold for OFF

//Declaration and initialization of public global objects
DigitalIn ignitionButton(BUTTON1);    // Simulates the ignition button
DigitalIn pSeatSens(D5);              // Passenger seat sensor
DigitalIn dSeatSens(D6);              // Driver seat sensor
DigitalIn pSeatBelt(D4);              // Passenger seatbelt switch
DigitalIn dSeatBelt(D3);              // Driver seatbelt switch

AnalogIn lightsens(A2);               // Light sensor (LDR)
AnalogIn headlightMode(A1);           // Potentiometer for headlight mode

DigitalOut ignitionLed(LED1);         // Green LED: Ignition enabled
DigitalOut engineLed(LED2);           // Blue LED: Engine started
DigitalOut alarmBuzzer(PE_10);        // Alarm Buzzer
DigitalOut headlights(D7);            // Combined headlight output

BufferedSerial uartUsb(USBTX, USBRX, 115200); // UART for messages

//Declaration and initialization of public global variables
bool engineRunning = false;           // Tracks if engine is running
bool welcomeDisplayed = false;        // Tracks if welcome message has been shown
bool previousButtonState = false;     // For detecting new button presses
bool headlightsOn = false;           // Tracks current headlight state
int accumulatedTimeDelay = 0;        // For tracking delay times

enum HeadlightMode {MODE_OFF, MODE_ON, MODE_AUTO};

float lightReadingsArray[LIGHT_SENSOR_SAMPLES];  // Array to store light readings
static int lightSampleIndex = 0;                 // Index for current sample

//Function Prototypes
void inputsInit();
void outputsInit();
void checkIgnitionConditions();
void handleIgnitionButton();
void displayInhibitReasons();
void controlHeadlights();
float sensorUpdate();
HeadlightMode getHeadlightMode(); 

//Main function
int main()
{
    inputsInit();
    outputsInit();
    
    // Initialize the light readings USE ARRAY IN CHAP 3
    for (int i = 0; i < LIGHT_SENSOR_SAMPLES; i++) {
        lightReadingsArray[i] = 0.0;
    }
    
    while (true) {
        if (dSeatSens && !welcomeDisplayed) {
            uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n", 48);
            welcomeDisplayed = true;
        }
        if (!dSeatSens) {
            welcomeDisplayed = false;
        }
        if (!engineRunning) {
            checkIgnitionConditions();
        }
        handleIgnitionButton();
        controlHeadlights();
        delay(TIME_INCREMENT_MS);
    }
}
// Helper function to determine headlight mode from potentiometer
HeadlightMode getHeadlightMode() {
    float potentialValue = headlightMode.read();
    if (potentialValue < OFF_THRESHOLD) return MODE_OFF;
    if (potentialValue < AUTO_THRESHOLD) return MODE_AUTO;
    return MODE_ON;
}

float sensorUpdate() //USE TEMPERATURE SENSOR MODULE
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

void controlHeadlights()
{    
    // If engine is off, turn off headlights and reset state
    if (!engineRunning) {
        headlights = 0;
        headlightsOn = false;
        accumulatedTimeDelay = 0;
        return;
    }
    //get variables from previous functions
    HeadlightMode mode = getHeadlightMode();
    float lightLevel = sensorUpdate();
    
    
    // Handle manual modes
    if (mode == MODE_ON) {
        headlights = 1;
        headlightsOn = true;
        accumulatedTimeDelay = 0;
        return;
    }
    if (mode == MODE_OFF) {
        headlights = 0;
        headlightsOn = false;
        accumulatedTimeDelay = 0;
        return;
    }
    
    // Handle AUTO mode
    if (mode == MODE_AUTO) {
        // Light level is dark (above NIGHT_LEVEL) - should turn on after delay
        if (lightLevel > NIGHT_LEVEL && !headlightsOn) {
            accumulatedTimeDelay += TIME_INCREMENT_MS;
            if (accumulatedTimeDelay >= 1000) { // 1 second delay
                headlights = 1;
                headlightsOn = true;
                accumulatedTimeDelay = 0;
            }
        }
        // Light level is bright (below DAY_LEVEL) - should turn off after delay
        else if (lightLevel < DAY_LEVEL && headlightsOn) {
            accumulatedTimeDelay += TIME_INCREMENT_MS;
            if (accumulatedTimeDelay >= 2000) { // 2 second delay
                headlights = 0;
                headlightsOn = false;
                accumulatedTimeDelay = 0;
            }
        }
        // Between levels - keep current state and reset delay
        else {
            accumulatedTimeDelay = 0;
        }
    }
}
//REST IS SAME AS PROJ 1 CODE
void checkIgnitionConditions()
{
    bool allConditionsMet = dSeatSens && pSeatSens && dSeatBelt && pSeatBelt;
    ignitionLed = allConditionsMet;
}

void handleIgnitionButton()
{
    bool currentButtonState = ignitionButton;
    if (currentButtonState && !previousButtonState) {
        if (!engineRunning) {
            if (ignitionLed) {
                engineLed = 1;
                ignitionLed = 0;
                engineRunning = true;
                uartUsb.write("Engine started.\r\n", 17);
            } else {
                alarmBuzzer = 1;
                uartUsb.write("Ignition inhibited.\r\n", 20);
                displayInhibitReasons();
                delay(500);
                alarmBuzzer = 0;
            }
        } else {
            engineLed = 0;
            engineRunning = false;
            uartUsb.write("Engine stopped.\r\n", 17);
        }
    }
    previousButtonState = currentButtonState;
}

void displayInhibitReasons()
{
    if (!dSeatSens) uartUsb.write("Driver seat not occupied\r\n", 25);
    if (!pSeatSens) uartUsb.write("Passenger seat not occupied\r\n", 28);
    if (!dSeatBelt) uartUsb.write("Driver seatbelt not fastened\r\n", 30);
    if (!pSeatBelt) uartUsb.write("Passenger seatbelt not fastened\r\n", 33);
}

void inputsInit()
{
    ignitionButton.mode(PullDown);
    pSeatSens.mode(PullDown);
    dSeatSens.mode(PullDown);
    pSeatBelt.mode(PullDown);
    dSeatBelt.mode(PullDown);
}

void outputsInit()
{
    ignitionLed = 0;
    engineLed = 0;
    alarmBuzzer = 0;
    headlights = 0;
}
