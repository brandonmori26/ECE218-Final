//=====[#include guards - begin]===============================================

#ifndef _WINDSHIELD_WIPER_H_
#define _WINDSHIELD_WIPER_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

typedef enum {
    OFF,
    LO,
    INT,
    HI
} windshield_wiper_state_t;

typedef enum {
    SHORT,
    MEDIUM,
    LONG
} windshield_wiper_delay_state_t;

//=====[Declarations (prototypes) of public functions]=========================

void windshieldWiperInit();

void windshieldWiperUpdate();

windshield_wiper_state_t getWindshieldWiperState();

windshield_wiper_delay_state_t getWindshieldWiperDelay();

//=====[#include guards - end]=================================================

#endif // _DISPLAY_H_