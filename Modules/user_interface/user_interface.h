//=====[#include guards - begin]===============================================

#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

typedef enum {
    OFF,
    LO,
    INT,
    HI
} windshield_wiper_state_t;

//=====[Declarations (prototypes) of public functions]=========================

void windshieldWiperInit();

void windshieldWiperUpdate();

windshield_wiper_state_t getWindshieldWiperState();

//=====[#include guards - end]=================================================

#endif // _USER_INTERFACE_