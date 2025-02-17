//=====[#include guards - begin]===============================================

#ifndef _WINDSHIELD_H_
#define _WINDSHIELD_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================
typedef enum {
    OFF,
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

WiperMode_t wiperModeUpdate();
IntMode_t intModeUpdate();


//=====[#include guards - end]=================================================

#endif // _WINDSHIELD_H_