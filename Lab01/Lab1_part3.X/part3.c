// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "Ascii.h"
#include "Oled.h"
#include "OledDriver.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main() {
    BOARD_Init();
    OledInit();
    {
        //Add code here to print to OLED
        OledDrawString("Hello, World!\n");
        OledUpdate();
        
        // You can never return from main() in an embedded system (one that lacks an operating system).
        // This will result in the processor restarting, which is almost certainly not what you want!
        while (1);
    }
}


