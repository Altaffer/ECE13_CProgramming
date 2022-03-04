// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CSE013E Support Library
#include "UNIXBOARD.h"

// User libraries
//#include "Player.h"
#include "Game.h"

// **** Set any macros or preprocessor directives here ****
// using this to check upper case and lower case letters, could have just gone with tolower()
#define CHECK_DIR(x, upper, lower)  ((x == upper) || (x == lower))
// for directions
#define UPPER_N 'N'
#define UPPER_E 'E'
#define UPPER_S 'S'
#define UPPER_W 'W'
#define LOWER_N 'n'
#define LOWER_E 'e'
#define LOWER_S 's'
#define LOWER_W 'w'
// for user input
#define MAX_INPUT_SIZE 1
#define EXIT_Q 'q'

#define MAX_TITLE_W_NULL GAME_MAX_ROOM_TITLE_LENGTH + 1
#define MAX_DESC_W_NULL GAME_MAX_ROOM_DESC_LENGTH + 1
// **** Declare any data types here ****

// **** Define any global or external variables here ****
// the following variables are used to account for exits after calling printroom
// bools to know if an exit exists rather than calling GameGet multiple times
static uint8_t exitN = FALSE;
static uint8_t exitE = FALSE;
static uint8_t exitS = FALSE;
static uint8_t exitW = FALSE;
// use lower 4 bits for the exits
static uint8_t exits = 0;

// **** Declare any function prototypes here ****
void PrintRoom(void);

int main() {
    /******************************** Your custom code goes below here ********************************/
    char userInput;
    // holds return of fread, the amout if bytes actually read

    if (GameInit() != SUCCESS) {
        printf("\nGame encountered a fatal error while opening initial game file.");
        printf("\nExiting game.");
        FATAL_ERROR();
    }
    // if game init was successful, print the room 
    PrintRoom();
    
    // now ask for user input into the next rooms and run the appropriate gamego function if the 
    // exit exists.
    do {

        printf("\n\nEnter direction to travel (n, e, s, w) or q to exit the game: ");

        int userInSize = 0;
        userInput = getchar();
        userInSize++;
        while (getchar() != '\n') { // clear input buffer
            userInSize++;
        }

        if (userInSize > MAX_INPUT_SIZE) {
            printf("\nPlease enter a single character. (n,e,s,w) or q to quit.");
            printf("\n\n----------------------------------------------------------------------------");
        } else {

            printf("\n\n----------------------------------------------------------------------------");

            if (CHECK_DIR(userInput, UPPER_N, LOWER_N) && (exitN)) {
                // if user wants to go north and there is a north exit
                if (GameGoNorth() != SUCCESS) {
                    // after running game go, check if the return was not successful.
                    // if so print error message and run fatal error
                    printf("\nGame encountered a fatal error while opening file for north direction.");
                    printf("\nExiting game.");
                    FATAL_ERROR();
                }
            } else if (CHECK_DIR(userInput, UPPER_E, LOWER_E) && (exitE)) {
                // see north case
                if (GameGoEast() != SUCCESS) {
                    printf("\nGame encountered a fatal error while opening file for east direction.");
                    printf("\nExiting game.");
                    FATAL_ERROR();
                }
            } else if (CHECK_DIR(userInput, UPPER_S, LOWER_S) && (exitS)) {
                // see north case
                if (GameGoSouth() != SUCCESS) {
                    printf("\nGame encountered a fatal error while opening file for south direction.");
                    printf("\nExiting game.");
                    FATAL_ERROR();
                }
            } else if (CHECK_DIR(userInput, UPPER_W, LOWER_W) && (exitW)) {
                // see north case
                if (GameGoWest() != SUCCESS) {
                    printf("\nGame encountered a fatal error while opening file for west direction.");
                    printf("\nExiting game.");
                    FATAL_ERROR();
                }
            } else {
                // invalid input
                // user gave a bad char if the input is not q
                if (userInput != EXIT_Q) {
                    printf("\nPlease enter valid input. (n,e,s,w) or q to quit.");
                }
            }
        }

        // reprint the room after the input
        // only print if the game is still playable
        if ((userInput != EXIT_Q)) {
            PrintRoom();
        }

    } while ((userInput != EXIT_Q));

    printf("\n\nGame Over.\n\n");

    exit(SUCCESS); // exit(SUCCESS) since this is not intended for an embedded system

    /**************************************************************************************************/
}


void PrintRoom(void){
    char title [MAX_TITLE_W_NULL]; // plus 1 for the null
    char desc[MAX_DESC_W_NULL]; // plus 1 for the null

    int titleRet = GameGetCurrentRoomDescription(desc);
    int descRet = GameGetCurrentRoomTitle(title);
    exits = GameGetCurrentRoomExits();

    printf("\n");
    if (titleRet) {
        puts(title);
    } else {
        printf("\nError while printing title.");
    }
    printf("\n");
    if (descRet) {
        puts(desc);
    } else {
        printf("\nError while printing description.");
    }
    printf("\n");
    // printing exits
    printf("\nExits: ");
    if (exits & GAME_ROOM_EXIT_NORTH_EXISTS) {
        exitN = TRUE;
        printf("%c ", UPPER_N);
    }
    if (exits & GAME_ROOM_EXIT_EAST_EXISTS) {
        exitE = TRUE;
        printf("%c ", UPPER_E);
    }
    if (exits & GAME_ROOM_EXIT_SOUTH_EXISTS) {
        exitS = TRUE;
        printf("%c ", UPPER_S);
    }
    if (exits & GAME_ROOM_EXIT_WEST_EXISTS) {
        exitW = TRUE;
        printf("%c ", UPPER_W);
    }
}