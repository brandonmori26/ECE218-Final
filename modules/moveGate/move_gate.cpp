//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "move_gate.h"

//=====[Declaration of private defines]========================================

#define GATE_MOVE_DELAY_MS                   50
#define DUTY_MIN                             0.04
#define DUTY_MAX                             0.0735
#define PERIOD                               0.02

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut greenLED(D6);
DigitalOut redLED(D7);
PwmOut servo(PG_0);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]=========================

//=====[Implementations of public functions]===================================

/**
* Initializes the windshield wiper system by setting up user display and 
* positional servo motor
*/
void moveGateInit() {
    greenLED = OFF;
    redLED = OFF;
    servo.period(PERIOD);
    servo.write(DUTY_MIN);
}


/**
* Updates the windshield wiper system if the engine is on. If it is off, the wipers
* remain off.
*/
void openGate() {
    redLED = ON;
    for (int i = 0; i < 10; i++) {
        servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 10) * i);
        delay(GATE_MOVE_DELAY_MS);
    }
    redLED = OFF;
    greenLED = ON;
}

void closeGate() {
    greenLED = OFF;
    redLED = ON;
    for (int i = 0; i < 10; i++) {
        servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 10) * i);
        delay(GATE_MOVE_DELAY_MS);
    }
    redLED = OFF;
}

//=====[Implementations of private functions]==================================
