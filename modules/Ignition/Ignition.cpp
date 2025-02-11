//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of private global variables]============
void inputsInit();
void outputsInit();
void checkIgnitionConditions();

//Declaration and initialization of public global objects
DigitalIn ignitionButton(BUTTON1);    // Simulates the ignition button
DigitalIn pSeatSens(D10);              // Passenger seat sensor
DigitalIn dSeatSens(D11);              // Driver seat sensor
DigitalIn pSeatBelt(D12);              // Passenger seatbelt switch
DigitalIn dSeatBelt(D13);              // Driver seatbelt switch


DigitalOut ignitionLed(LED1);         // Green LED: Ignition enabled
DigitalOut engineLed(LED2);           // Blue LED: Engine started
DigitalOut alarmBuzzer(D15);        // Alarm Buzzer
BufferedSerial uartUsb(USBTX, USBRX, 115200); // UART for messages

//=====[Implementations of public functions]===================================
void checkIgnitionConditions()
{
    // Enable ignition only when all conditions are met
    bool allConditionsMet = dSeatSens && pSeatSens && dSeatBelt && pSeatBelt;
    ignitionLed = allConditionsMet;
}

void handleIgnitionButton()
{
    bool currentButtonState = ignitionButton;
    
    // Detect new button press (button is pressed and wasn't pressed before)
    if (currentButtonState && !previousButtonState) {
        if (!engineRunning) {
            // Try to start engine
            if (ignitionLed) {
                // Normal ignition
                engineLed = 1;
                ignitionLed = 0;
                engineRunning = true;
                uartUsb.write("Engine started.\r\n", 17);
            } else {
                // Inhibited ignition
                alarmBuzzer = 1;
                uartUsb.write("Ignition inhibited.\r\n", 20);
                displayInhibitReasons();
                ThisThread::sleep_for(500ms);  // Keep buzzer on briefly
                alarmBuzzer = 0;
            }
        } else {
            // Stop engine on new button press while engine is running
            engineLed = 0;
            engineRunning = false;
            uartUsb.write("Engine stopped.\r\n", 17);
        }
    }
    
    previousButtonState = currentButtonState;
}

void displayInhibitReasons()
{
    if (!dSeatSens) {
        uartUsb.write("Driver seat not occupied\r\n", 25);
    }
    if (!pSeatSens) {
        uartUsb.write("Passenger seat not occupied\r\n", 28);
    }
    if (!dSeatBelt) {
        uartUsb.write("Driver seatbelt not fastened\r\n", 30);
    }
    if (!pSeatBelt) {
        uartUsb.write("Passenger seatbelt not fastened\r\n", 33);
    }
}

void inputsInit()
{
    // Configure inputs as pull-down to ensure default state is LOW
    ignitionButton.mode(PullDown);
    pSeatSens.mode(PullDown);
    dSeatSens.mode(PullDown);
    pSeatBelt.mode(PullDown);
    dSeatBelt.mode(PullDown);
}

void outputsInit()
{
    // Initialize outputs to OFF
    ignitionLed = 0;
    engineLed = 0;
    alarmBuzzer = 0;
}