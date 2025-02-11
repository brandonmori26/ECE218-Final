
//=====[Declarations (prototypes) of public functions]=========================
void inputsInit();
void outputsInit();
void checkIgnitionConditions();
void handleIgnitionButton();
void displayInhibitReasons();
//=========[Declarations of Private Variables]=========================
bool engineRunning = false;           // Tracks if engine is running
bool welcomeDisplayed = false;        // Tracks if welcome message has been shown
bool previousButtonState = false;     // For detecting new button presses
