//=====[#include guards - begin]===============================================

#ifndef _SERVO_MOTOR_H_
#define _SERVO_MOTOR_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

void servoMotorInit();

/**
 *  Sets the speed of the motor given some rpm
 */
void setSpeed(int rpm);

/**
 *  Sets the taget angle for the servo to turn to
 */
void setAngle(int degrees);

//=====[#include guards - end]=================================================

#endif // _SERVO_MOTOR_H_