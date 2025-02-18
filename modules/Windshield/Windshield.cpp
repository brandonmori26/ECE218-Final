#include "mbed.h"
#include "arm_book_lib.h"
#include "Windshield.h"
#include "Ignition.h"
#include "display.h"

#define PERIOD 0.02
#define DUTY_MIN 0.025
#define DUTY_MAX 0.125
#define DUTY_67 0.0584
#define DUTY_INCREMENT 0.00334


#define LOW_SPEED_DELAY_30RPM 1250 // delay time for 30 RPM
#define HIGH_SPEED_DELAY_40RPM 750 // delay time for 40 RPM
#define LOW_DELAY_FLUID 27    // delay for fluid positional mode movement
#define HIGH_DELAY_FLUID 18
#define WIPER_FULLRANGE_DELAY 370 //Time it takes for wiper to make full 67 degree rotation
#define INT_SHORT_DELAY 3000
#define INT_MEDIUM_DELAY 6000
#define INT_LONG_DELAY 8000
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

float wiperInt = 0.25;
float wiperLow = 0.5;
float wiperHigh = 0.75;
float wiperModeSelector;

float shortMode = 0.33;
float longMode = 0.66;
float intModeSelector;


//=====[Declarations (prototypes) of private functions]=========================

void PwmInit();
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

WiperMode_t wiperModeUpdate()
{
    static WiperMode_t wiperMode;
    wiperModeSelector = wiperModePot.read();

    if (wiperModeSelector <= wiperInt)
    {
        wiperMode = OFF_MODE;
    } else if (wiperModeSelector > wiperInt && wiperModeSelector <= wiperLow)
    {
        wiperMode = INT;
    } else if(wiperModeSelector > wiperLow && wiperModeSelector <= wiperHigh)
    {
        wiperMode = LO;
    } else if (wiperModeSelector > wiperHigh)
    {
        wiperMode = HI;
    }
    return wiperMode;
}

void FullWipe(int delayTime)
{
    static int accumulatedDelayTime = 0;
    if (revCompleted && accumulatedDelayTime < delayTime)
    {
        accumulatedDelayTime = accumulatedDelayTime + 2 * INT_TIME_INCREMENT;  //incrementing by twice as much
        delay(INT_TIME_INCREMENT);
    }
    else
    {
        LowSpeed();
        accumulatedDelayTime = 0;

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

void windshieldUpdate()
{
    if (ignitionUpdate())
    {
        switch(wiperModeUpdate())
        {
            case OFF_MODE:
                PwmInit();

                break;
            
            case INT:
                IntermittentMode();

                break;

            case LO:
                LowSpeed();

                break;

            case HI:
                HighSpeed();

                break;
        }
    } else { windshieldInit(); }
}

static void windshieldDisplayUpdate()
{
    static int accumulatedDisplayTime = 0;
    
    if(accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS) {
        accumulatedDisplayTime = 0;

        displayCharPositionWrite(5,0);
        switch(wiperModeUpdate()) {
            case OFF_MODE:
                displayStringWrite("OFF ");
                break;
            case INT:
                displayStringWrite("INT ");
                break;
            case LO:
                displayStringWrite("LOW ");
                break;
            case HI:
                displayStringWrite("HIGH");
                break;
        }

        // Display Intermittent Mode if in INT mode
        displayCharPositionWrite(4,1);
        if(wiperModeUpdate() == INT) {
            switch(intModeUpdate()) {
                case SHORT:
                    displayStringWrite("SHORT ");
                    break;
                case MEDIUM:
                    displayStringWrite("MEDIUM");
                    break;
                case LONG:
                    displayStringWrite("LONG  ");
                    break;
            }
        } else {
            displayStringWrite("      "); // Clear intermittent mode line if not in INT mode
        }
    } else {
        accumulatedDisplayTime = accumulatedDisplayTime + SYSYEM_TIME_INCREMENT_MS;        
    } 
}
