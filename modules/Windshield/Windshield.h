//=====[#include guards - begin]===============================================

#ifndef _WINDSHIELD_H_
#define _WINDSHIELD_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================
typedef enum {
    OFF_MODE,
    HI,
    LO,
    INT
} WiperMode_t;

typedef enum {
    SHORT,
    MEDIUM,
    LONG
} IntMode_t;
//=====[Declarations (prototypes) of public functions]=========================


void windshieldInit();
WiperMode_t wiperModeUpdate(); // Don't need to be public functions
void IntermittentMode();
IntMode_t intModeUpdate();

void windshieldUpdate();

//=====[#include guards - end]=================================================

#endif // _WINDSHIELD_H_
