//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "move_gate.h"

#include "unlocking_subsystem.h"

//=====[Declaration of private defines]========================================

#define GATE_MOVING_DELAY_MS                    28
#define DUTY_MIN                             0.04
#define DUTY_MAX                             0.0735
#define PERIOD                               0.02
#define MOTOR_UPDATE_TIME_MS                 20

//=====[Declaration of private data types]=====================================

typedef enum {
   GATE_OPENING,
   GATE_CLOSING
} gateDirection_t;

//=====[Declaration and initialization of public global objects]===============

PwmOut servo(PG_0);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static bool gateOpening = true;
WiperDirection_t currentGateDirection = GATE_OPENING;

//=====[Declarations (prototypes) of private functions]=========================

static void wiperReadUpdateMode();
static void windshieldWiperRunWipers();
static void intTypeReadUpdateMode();
static void rotateWiper(float wiperDelay);
static void intDelayWiper();
static void intDelayDisplayUpdateWiper(int delayTime, int iterations);

//=====[Implementations of public functions]===================================

/**
* Initializes the windshield wiper system by setting up user display and 
* positional servo motor
*/
void moveGateInit() {
    servo.period(PERIOD);
    servo.write(DUTY_MIN);
}


/**
* Updates the windshield wiper system if the engine is on. If it is off, the wipers
* remain off.
*/
void moveGateUpdate() {
    if ( gateUnlocked ) {
        openGate();
    } else {
        servo.write(DUTY_MIN);
    }
}

void windshieldWiperStop() {
    currentWiperMode = WIPER_OFF;
    userDisplayUpdate();
}

//=====[Implementations of private functions]==================================
static void wiperReadUpdateMode() {
   wiperModeReading = wiperMode.read();

    if ( wiperModeReading >= POTENTIOMETER_MIN_WIPER_HI_LEVEL ) {
        currentWiperMode = WIPER_HI;
    } else if ( wiperModeReading < POTENTIOMETER_MIN_WIPER_HI_LEVEL &&
                wiperModeReading >= POTENTIOMETER_MIN_WIPER_LO_LEVEL ) {
        currentWiperMode = WIPER_LO;
    } else if ( wiperModeReading < POTENTIOMETER_MIN_WIPER_LO_LEVEL &&
                wiperModeReading >= POTENTIOMETER_MIN_WIPER_INT_LEVEL ) {
        currentWiperMode = WIPER_INT;   
    } else {
        currentWiperMode = WIPER_OFF;
    }
    userDisplayUpdate();
}

static void intTypeReadUpdateMode() {
   intModeReading = intMode.read();

    if ( intModeReading >= POTENTIOMETER_MIN_INT_LONG_LEVEL ) {
        currentIntDelay = INT_LONG;
    } else if ( intModeReading < POTENTIOMETER_MIN_INT_LONG_LEVEL &&
                intModeReading >= POTENTIOMETER_MIN_INT_MED_LEVEL ) {
        currentIntDelay = INT_MED;
    } else {
        currentIntDelay = INT_SHORT;
    }
}

static void windshieldWiperRunWipers() {
    static float currentAngle = 0.0;
    static bool increasing = true;


    float dutyCycle = DUTY_MIN;
    if (currentWiperMode == WIPER_HI) {
        rotateWiper(WIPER_HI_DELAY_MS);
    } else if (currentWiperMode == WIPER_LO) {
        rotateWiper(WIPER_LO_DELAY_MS);
    } else if (currentWiperMode == WIPER_INT) {
        rotateWiper(WIPER_LO_DELAY_MS);
    } else if (currentWiperMode == WIPER_OFF) {
        windshieldWiperStop();
        servo.write(DUTY_MIN); 
        return;
    }
}

static void intDelayWiper() {
    switch ( currentIntDelay ) {
        case INT_LONG:
            intDelayDisplayUpdateWiper(WIPER_INT_LONG_DELAY_MS, 25);
            break;
        
        case INT_MED:
            intDelayDisplayUpdateWiper(WIPER_INT_MED_DELAY_MS, 25);
            break;

        case INT_SHORT:
            intDelayDisplayUpdateWiper(WIPER_INT_SHORT_DELAY_MS, 10);
            break;
    }

}

static void intDelayDisplayUpdateWiper(int delayTime, int iterations) {
    for (int i = 0; i < iterations; i++) {
        delay(delayTime / iterations);
        ignitionSubsystemUpdate();
        wiperReadUpdateMode();
        intTypeReadUpdateMode();
        userDisplayUpdate(); 
    }

}

static void rotateWiper(float wiperDelay) {
    static int accumulatedWiperTime = 0;
    accumulatedWiperTime = accumulatedWiperTime + 10;
    if (accumulatedWiperTime >= MOTOR_UPDATE_TIME_MS) {
        accumulatedWiperTime = 0;

        switch ( currentWiperDirection ) {
            case WIPER_MIN_TO_MAX:
                for (int i = 0; i < 10; i++) {
                    servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(wiperDelay);
                    ignitionSubsystemUpdate();
                }

                currentWiperDirection = WIPER_MAX_TO_MIN;
                break;

            case WIPER_MAX_TO_MIN:
                for (int i = 0; i < 10; i++){
                    servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(wiperDelay);
                    ignitionSubsystemUpdate();
                }
                if (currentWiperMode == WIPER_INT) {
                    intDelayWiper();
                }

                currentWiperDirection = WIPER_MIN_TO_MAX;
                break;

        }

    }
}