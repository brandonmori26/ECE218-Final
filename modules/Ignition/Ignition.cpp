//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "Ignition.h"

//=====[Defines]===============================================================

#define BUZZER_ON 0
#define BUZZER_OFF 1
#define TIME_DEBOUNCE_MS 30

//=====[Declaration of public data types]======================================

typedef enum{
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING
} debouncedIgnitionReleasedStateMachine_t;

//Declaration and initialization of public global objects

DigitalIn ignitionButton(BUTTON1);    // Simulates the ignition button
DigitalIn passengerPresent(D10);              // Passenger seat sensor
DigitalIn driverPresent(D11);              // Driver seat sensor
DigitalIn passengerSeatbelt(D12);              // Passenger seatbelt switch
DigitalIn driverSeatbelt(D13);              // Driver seatbelt switch


DigitalOut greenLED(LED1);         // Green LED: Ignition enabled
DigitalOut blueLED(LED2);           // Blue LED: Engine started
DigitalOut sirenPin(D15);        // Alarm Buzzer
BufferedSerial uartUsb(USBTX, USBRX, 115200); // UART for messages


//=====[Declaration and initialization of public global variables]=============

int accumulatedButtonTime = 0;
int accumulatedHeadlightDelayTime_ON = 0;
int accumulatedHeadlightDelayTime_OFF = 0;

bool engineStarted = false;
bool driverWelcomed = false;

debouncedIgnitionReleasedStateMachine_t ignitionButtonState;

//=====[Declaration and initialization of private global variables]============
void debounceIgnitionInit();
void ignitionEnable();
void welcomeMessage();
void errorMessage();
bool debounceIgnition();


//=====[Implementations of public functions]===================================
void inputsInit()
{
    driverPresent.mode(PullDown);
    passengerPresent.mode(PullUp);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
}

void outputsInitIgnition()
{
    greenLED = OFF;
    blueLED = OFF;
    sirenPin = BUZZER_OFF;
}

void debounceIgnitionInit()
{
    if( ignitionButton ) {
        ignitionButtonState = BUTTON_UP;
    } else {
        ignitionButtonState = BUTTON_DOWN;
    }
}
void ignitionEnable()
{
    if (driverPresent && driverSeatbelt && !passengerPresent && passengerSeatbelt){
        greenLED = ON;
    }
    else{
        greenLED = OFF;
    }
}

bool debounceIgnition()
{
    bool ignitionReleased = false;

    switch( ignitionButtonState ) {
        case BUTTON_UP:
            if( ignitionButton ) {
                ignitionButtonState = BUTTON_FALLING;
                accumulatedButtonTime = 0;
            }
            break;

        case BUTTON_FALLING:
            if( accumulatedButtonTime >= TIME_DEBOUNCE_MS ) {
                if( ignitionButton ) {
                    ignitionButtonState = BUTTON_DOWN;
                } else {
                ignitionButtonState = BUTTON_UP;
                }
            }
            accumulatedButtonTime = accumulatedButtonTime + TIME_INCREMENT_MS;
            break;
        
        case BUTTON_DOWN:
            if (!ignitionButton){
                ignitionButtonState = BUTTON_RISING;
                accumulatedButtonTime = 0;
            }
            break;

        case BUTTON_RISING:
            if (!ignitionButton){
                ignitionButtonState = BUTTON_UP;
                ignitionReleased = true;
            }
            else{
                ignitionButtonState = BUTTON_DOWN;
            }
            accumulatedButtonTime = accumulatedButtonTime + TIME_INCREMENT_MS;
            break;
    }
    return ignitionReleased;
}
void welcomeMessage()
{
    if (driverPresent && !driverWelcomed){
        uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n\r\n", 50);
        driverWelcomed = true;
    }
    else if (!driverPresent){
        driverWelcomed = false;
    }
}

void errorMessage()
{

    uartUsb.write("Ignition Inhibited\r\n\r\n", 22);
    
    if(!driverPresent){
        uartUsb.write("Driver seat not occupied.\r\n\r\n", 29);
    } 
    if(!driverSeatbelt){
        uartUsb.write("Driver seatbelt not fastened.\r\n\r\n", 33);
    }
    if(passengerPresent){
        uartUsb.write("Passenger seat not occupied.\r\n\r\n", 32);
    }
    if(!passengerSeatbelt){
        uartUsb.write("Passenger seatbelt not fastened.\r\n\r\n", 36);
    }
}

//PUBLIC FUNCTION 
bool ignitionUpdate()
{
    welcomeMessage();
    ignitionEnable();
    if (!engineStarted && debounceIgnition()){
        uartUsb.write("Ignition attempted.\r\n\r\n", 23);
        if (!greenLED){
            sirenPin = BUZZER_ON;
            errorMessage();
            engineStarted = false;
        }
        else{
            sirenPin = BUZZER_OFF;
            greenLED = OFF;
            blueLED = ON;
            uartUsb.write("Engine started.\r\n\r\n", 19);
            engineStarted = true;
        }
    }
    else if (engineStarted && debounceIgnition()){
        uartUsb.write("Engine off.\r\n\r\n", 15);
        engineStarted = false;
        blueLED = OFF;
    }
    
    return engineStarted;
}
