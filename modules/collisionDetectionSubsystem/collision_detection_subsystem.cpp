//=====[Libraries - include]====================================================
#include "mbed.h"
#include "collision_detection_subsystem.h"

//=====[Declaration of private constants]=======================================
// GPIO pin definitions (adjust these to match your actual hardware connections)
#define TRIG_PIN PB_6   // Trigger pin for HC-SR04
#define ECHO_PIN PB_7   // Echo pin for HC-SR04
#define BUZZER_PIN PB_5 // Buzzer activation pin

// Timing constants
#define TRIGGER_PULSE_DURATION_US 10
#define MEASUREMENT_INTERVAL_MS 100

//=====[Declaration of private variables]=======================================
static DigitalOut trigPin(TRIG_PIN);
static DigitalIn  echoPin(ECHO_PIN);
static DigitalOut buzzerPin(BUZZER_PIN);

static CollisionState currentCollisionState = COLLISION_NO_OBJECT;
static float lastMeasuredDistance = 0.0f;

// Accumulated time tracking
static Timer accumulatedTimer;
static int lastMeasurementTime = 0;

// State machine for non-blocking distance measurement
enum DistanceMeasurementState {
    IDLE,
    TRIGGER_SENT,
    WAITING_ECHO_START,
    MEASURING_ECHO
};
static DistanceMeasurementState currentMeasurementState = IDLE;
static Timer echoTimer;
static int triggerSentTime = 0;

//=====[Declaration of private functions]=======================================
// Non-blocking distance measurement function
static bool measureDistanceNonBlocking(float& distance) {
    int currentTime = accumulatedTimer.read_ms();
    
    switch (currentMeasurementState) {
        case IDLE:
            // Check if it's time for a new measurement
            if (currentTime - lastMeasurementTime >= MEASUREMENT_INTERVAL_MS) {
                // Send trigger pulse
                trigPin = 0;
                trigPin = 1;
                triggerSentTime = currentTime;
                currentMeasurementState = TRIGGER_SENT;
            }
            return false;

        case TRIGGER_SENT:
            // Ensure trigger pulse duration
            if (currentTime - triggerSentTime >= 1) {
                trigPin = 0;
                currentMeasurementState = WAITING_ECHO_START;
            }
            return false;

        case WAITING_ECHO_START:
            if (echoPin == 1) {
                // Echo started, begin timing
                echoTimer.reset();
                echoTimer.start();
                currentMeasurementState = MEASURING_ECHO;
            }
            return false;

        case MEASURING_ECHO:
            if (echoPin == 0) {
                // Echo ended, calculate distance
                echoTimer.stop();
                float duration = echoTimer.read_us();
                distance = duration * 0.0343f / 2.0f;
                
                // Reset for next measurement
                lastMeasurementTime = currentTime;
                currentMeasurementState = IDLE;
                return true;
            }
            return false;
    }
    
    return false;
}

// Update collision state based on measured distance
static void updateCollisionState(float distance) {
    lastMeasuredDistance = distance;

    if (distance <= COLLISION_MIN_DISTANCE_CM) {
        currentCollisionState = COLLISION_IMMINENT;
        buzzerPin = 1; // Activate buzzer at highest intensity
    } else if (distance <= COLLISION_MAX_DISTANCE_CM / 2.0f) {
        currentCollisionState = COLLISION_OBJECT_DETECTED;
        buzzerPin = 1; // Activate buzzer at lower intensity
    } else {
        currentCollisionState = COLLISION_NO_OBJECT;
        buzzerPin = 0; // Deactivate buzzer
    }
}

//=====[Implementations of public functions]====================================
void collisionDetectionSubsystemInit() {
    // Initialize pins
    trigPin = 0;
    buzzerPin = 0;

    // Start accumulated timer
    accumulatedTimer.start();
}

void collisionDetectionSubsystemUpdate() {
    float distance;
    
    // Non-blocking distance measurement
    if (measureDistanceNonBlocking(distance)) {
        // If a measurement is completed, update collision state
        updateCollisionState(distance);
    }
}

CollisionState collisionDetectionGetState() {
    return currentCollisionState;
}

float collisionDetectionGetDistance() {
    return lastMeasuredDistance;
}
