//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "gate.h"

//=====[Defines]===============================================================
#define PERIOD 0.02        // 20ms period for standard servos
#define DUTY_MIN 0.025     // Minimum duty cycle (0 degrees)
#define DUTY_MAX 0.115     // Maximum duty cycle (180 degrees)
#define GATE_MOVEMENT_DELAY_MS 3000 // Delay for gate movement in milliseconds

// Angle definitions
#define ANGLE_MIN 0.0f     // Minimum angle (0 degrees)
#define ANGLE_MAX 180.0f   // Maximum angle (180 degrees)

//=====[Declaration and initialization of public global variables]=============
gateState_t currentGateState = GATE_CLOSED;
bool gateOperational = false;

//=====[Declaration and initialization of private global variables]============
static PwmOut servo(PF_9);  // Servo motor control pin
static int accumulatedGateTime = 0;

//=====[Declarations of private functions]=====================================
static float dutyFromAngle(float angle);

//=====[Implementations of public functions]===================================
void gateInit()
{
    // Set the PWM period
    servo.period(PERIOD);
    
    // Initialize gate in closed position
    servo.write(DUTY_MIN);
    
    // Reset variables
    currentGateState = GATE_CLOSED;
    gateOperational = true;
    accumulatedGateTime = 0;
}

void gateUpdate()
{
    // Main state machine for gate operation
    switch (currentGateState) {
        case GATE_CLOSED:
            // Gate is fully closed - nothing to do
            break;
            
        case GATE_OPENING:
            // Gate is in process of opening
            servo.write(DUTY_MAX);
            
            // Increment timing counter
            accumulatedGateTime += 100; // Assuming 100ms between calls
            
            // Check if we need to transition to OPEN state
            if (accumulatedGateTime >= GATE_MOVEMENT_DELAY_MS) {
                currentGateState = GATE_OPEN;
                accumulatedGateTime = 0;
            }
            break;
            
        case GATE_OPEN:
            // Gate is fully open - nothing to do
            break;
            
        case GATE_CLOSING:
            // Gate is in process of closing
            servo.write(DUTY_MIN);
            
            // Increment timing counter
            accumulatedGateTime += 100; // Assuming 100ms between calls
            
            // Check if we need to transition to CLOSED state
            if (accumulatedGateTime >= GATE_MOVEMENT_DELAY_MS) {
                currentGateState = GATE_CLOSED;
                accumulatedGateTime = 0;
            }
            break;
            
        default:
            // Unexpected state, reset gate
            gateInit();
            break;
    }
}

void gateOpen()
{
    if (!gateOperational) {
        return;
    }
    
    if (currentGateState == GATE_CLOSED || currentGateState == GATE_CLOSING) {
        currentGateState = GATE_OPENING;
        accumulatedGateTime = 0;
    }
}

void gateClose()
{
    if (!gateOperational) {
        return;
    }
    
    if (currentGateState == GATE_OPEN || currentGateState == GATE_OPENING) {
        currentGateState = GATE_CLOSING;
        accumulatedGateTime = 0;
    }
}

bool isGateOpen()
{
    return (currentGateState == GATE_OPEN || currentGateState == GATE_OPENING);
}

bool isGateMoving()
{
    return (currentGateState == GATE_OPENING || currentGateState == GATE_CLOSING);
}

void gateStop()
{
    // Stop the gate immediately
    if (currentGateState == GATE_OPENING) {
        currentGateState = GATE_OPEN;
    } else if (currentGateState == GATE_CLOSING) {
        currentGateState = GATE_CLOSED;
    }
    
    accumulatedGateTime = 0;
}

void gateReset()
{
    // Re-initialize the gate
    gateInit();
}

//=====[Implementations of private functions]==================================
static float dutyFromAngle(float angle)
{
    // Ensure angle is within valid range
    if (angle < ANGLE_MIN) angle = ANGLE_MIN;
    if (angle > ANGLE_MAX) angle = ANGLE_MAX;
    
    // Convert angle (ANGLE_MIN-ANGLE_MAX) to duty cycle (DUTY_MIN-DUTY_MAX)
    return DUTY_MIN + (angle / ANGLE_MAX) * (DUTY_MAX - DUTY_MIN);
}
