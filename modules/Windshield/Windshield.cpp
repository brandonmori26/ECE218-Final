#include "mbed.h"
#include "arm_book_lib.h"

#define PERIOD 0.02
#define DUTY_MIN 0.025
#define DUTY_MAX 0.125
#define DUTY_67 0.0584
#define DUTY_30RPM 0.00334


#define LOW_SPEED_DELAY_30RPM 1250 // delay time for 30 RPM
#define HIGH_SPEED_DELAY_40RPM 750 // delay time for 40 RPM
#define LOW_FLUID_DELAY 37    // delay for fluid positional mode movement
#define WIPER_FULLRANGE_DELAY 370 //Time it takes for wiper to make full 67 degree rotation

#define NUMBER_OF_INCREMENTS_30RPM 20

//=====[Declaration of private data types]=====================================
enum WiperMode {
    OFF,
    HI,
    LO,
    INT
};

//=====[Declaration and initialization of private global objects]===============

PwmOut servo(PF_9); //chargoggagoggmanchauggagoggchaubunagungamaugg

//=====[Declaration and initialization of private global variables]============

float currentDutyCycle;
bool wiper67 = false;

//=====[Implementations of public functions]===================================

void PwmInit();
void PwmMax();
void PwmMin();
void Pwm67();
void FullWipe();
// main() runs in its own thread in the OS

void PwmInit()
{
    servo.period(PERIOD);
    PwmMin();

}

void PwmMax()
{
    servo.write(DUTY_MAX);
}

void FullWipe()
{
    servo.write(DUTY_67);
    delay(370);
    servo.write(DUTY_MIN);
    delay(370);
}





