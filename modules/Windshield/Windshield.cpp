#include "mbed.h"
#include "arm_book_lib.h"

#define PERIOD 0.02
#define DUTY_MIN 0.025
#define DUTY_MAX 0.125

#define THREE_SECONDS 3000

PwmOut servo(PF_9); //chargoggagoggmanchauggagoggchaubunagungamaugg

void PwmInit();
void PwmMax();
void PwmMin();
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

void PwmMin()
{
    servo.write(DUTY_MIN);
}


