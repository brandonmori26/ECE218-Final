#include "mbed.h"
#include "arm_book_lib.h"
#include "Windshield.h"

#define PERIOD 0.02
#define DUTY_MIN 0.025
#define DUTY_MAX 0.125
#define DUTY_67 0.0584
#define DUTY_INCREMENT 0.00334


#define LOW_SPEED_DELAY_30RPM 1250 // delay time for 30 RPM
#define HIGH_SPEED_DELAY_40RPM 750 // delay time for 40 RPM
#define LOW_DELAY_FLUID 37    // delay for fluid positional mode movement
#define HIGH_DELAY_FLUID 28
#define WIPER_FULLRANGE_DELAY 370 //Time it takes for wiper to make full 67 degree rotation
#define INT_SHORT_DELAY 3000
#define INT_MEDIUM_DELAY 6000
#define INT_LONG_DELAY 9000
#define INT_TIME_INCREMENT 10

#define NUMBER_OF_INCREMENTS_30RPM 20

//=====[Declaration of private data types]======================================

//=====[Declaration and initialization of private global objects]===============

PwmOut servo(PF_9); //chargoggagoggmanchauggagoggchaubunagungamaugg
AnalogIn wiperModePot(A0);
AnalogIn intModePot(A1);

//=====[Declaration and initialization of private global variables]============

float currentDutyCycle;
bool wiper67 = false;
bool revCompleted = false;

int accumulatedDelayTime = 0;

WiperMode_t wiperMode;
IntMode_t intMode;

float shortMode = 0.33;
float longMode = 0.66;
float intModeSelector;

//=====[Declarations (prototypes) of private functions]=========================

void PwmInit();
void PwmMax();
void Pwm67();
void FullWipe(int delayTime);

void LowSpeed();
void HighSpeed();


//=====[Implementations of public functions]===================================

void windshieldInit()
{
    PwmInit();
    wiperMode = OFF_MODE;
    intMode = LONG;

}
void PwmInit()
{
    servo.period(PERIOD);
    servo.write(DUTY_MIN);

}

void PwmMax()
{
    servo.write(DUTY_MAX);
}

void LowSpeed()
{
    revCompleted = false;
    if (!wiper67) 
    {
        currentDutyCycle = currentDutyCycle + DUTY_INCREMENT;
        servo.write(currentDutyCycle);
        delay(LOW_DELAY_FLUID);
            
        if (currentDutyCycle > DUTY_67){ wiper67 = true; }
    }
    else
    {
        currentDutyCycle = currentDutyCycle - DUTY_INCREMENT;
        servo.write(currentDutyCycle);
        delay(LOW_DELAY_FLUID);

        if (currentDutyCycle < DUTY_MIN)
        {
            currentDutyCycle = DUTY_MIN;
            wiper67 = false;
            revCompleted = true;
        }
    }
}

void HighSpeed()
{
    if (!wiper67) 
    {
        currentDutyCycle = currentDutyCycle + DUTY_INCREMENT;
        servo.write(currentDutyCycle);
        delay(HIGH_DELAY_FLUID);
            
        if (currentDutyCycle > DUTY_67){ wiper67 = true; }
    }
    else if(wiper67)
    {
        currentDutyCycle = currentDutyCycle - DUTY_INCREMENT;
        servo.write(currentDutyCycle);
        delay(HIGH_DELAY_FLUID);

        if (currentDutyCycle < DUTY_MIN)
        {
            currentDutyCycle = DUTY_MIN;
            wiper67 = false;
        }
    }
}



void FullWipe(int delayTime)
{
    static int accumulatedDelayTime = 0;
    if (revCompleted && accumulatedDelayTime < delayTime)
    {
        accumulatedDelayTime = accumulatedDelayTime + INT_TIME_INCREMENT;
        delay(INT_TIME_INCREMENT);
    }
    else
    {
        LowSpeed();
        accumulatedDelayTime = 0;

    }
}

void IntermittentMode()
{
    switch(intModeUpdate())
    {
        case SHORT:
            FullWipe(INT_SHORT_DELAY);

            break;

        case MEDIUM:
            FullWipe(INT_MEDIUM_DELAY);

            break;

        case LONG:
            FullWipe(INT_LONG_DELAY);

            break;
    }
}

IntMode_t intModeUpdate()
{
    static IntMode_t intMode;
    intModeSelector = intModePot.read();

    if (intModeSelector <= shortMode)
    {
        intMode = SHORT;
    } else if (intModeSelector > shortMode && intModeSelector <= longMode)
    {
        intMode = MEDIUM;
    } else if (intModeSelector > longMode)
    {
        intMode = LONG;
    }
    return intMode;
}

