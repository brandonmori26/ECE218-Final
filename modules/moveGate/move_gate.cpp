//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "move_gate.h"

//=====[Declaration of private defines]========================================

#define GATE_MOVING_DELAY_MS                 50
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
gateDirection_t currentGateDirection = GATE_OPENING;

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
void moveGate() {
    static int accumulatedGateTime = 0;
    accumulatedGateTime = accumulatedGateTime + 10;
    if (accumulatedGateTime >= MOTOR_UPDATE_TIME_MS) {
        accumulatedGateTime = 0;

        switch ( currentGateDirection ) {
            case GATE_OPENING:
                for (int i = 0; i < 10; i++) {
                    servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(GATE_MOVING_DELAY_MS);
                }

                currentGateDirection = GATE_CLOSING;
                break;

            case GATE_CLOSING:
                for (int i = 0; i < 10; i++) {
                    servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(GATE_MOVING_DELAY_MS);
                }

                currentGateDirection = GATE_OPENING;
                break;
        }
    }
}

//=====[Implementations of private functions]==================================