#include "UNIXBOARD.h"
#include "Game.h"
#include "Player.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Necessary Definitions
#define ROOMFILE "RoomFiles/room"
#define TXT ".txt"
#define FILE_NAME_SIZE 21     //"RoomFiles/room**.txt\0"
// seek values
#define RPG_OFFSET 3 // gets rid of the first 3 chars (RPG)
// exit bits
#define BIT_3 8
#define BIT_2 4
#define BIT_1 2
#define BIT_0 1

typedef struct {
    // for title and desc strings
    int titleLength;
    int descLength;
    char title[GAME_MAX_ROOM_TITLE_LENGTH];
    char desc[GAME_MAX_ROOM_DESC_LENGTH];

    // for inventory checking
    uint8_t numItemsRequired;
    uint8_t itemsContained;

    // holds number of current roomN.txt
    uint8_t currentRoom;

    // bool used to know if player has required items for room versions
    uint8_t playerHasReqItems;

    // used to know if a certain exit exists
    uint8_t northExit;
    uint8_t eastExit;
    uint8_t southExit;
    uint8_t westExit;
    // 4-bit bitfield to tell which exits are available
    uint8_t exits;

    uint8_t error; // used to tell GameGet functions what to return

} GameData;

// global variables
static GameData room;

int FileHandler(void);
/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void){
    if (room.northExit) {
        room.currentRoom = room.northExit;
    }
    return FileHandler();
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void){
    if (room.eastExit) {
        room.currentRoom = room.eastExit;
    }
    return FileHandler();
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void){
    if (room.southExit) {
        room.currentRoom = room.southExit;
    }
    return FileHandler();
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void){
    if (room.westExit) {
        room.currentRoom = room.westExit;
    }
    return FileHandler();
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void){
    room.playerHasReqItems = TRUE;
    room.currentRoom = STARTING_ROOM;
    room.northExit = 0;
    room.southExit = 0;
    room.westExit = 0;
    room.eastExit = 0;
    room.exits = 0;
    room.error = FALSE;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title){
    if (room.error){
        title = NULL;
        return 0;
    } else {
        strncpy(title, room.title, room.titleLength);
        title[room.titleLength] = '\0';
        return (room.titleLength);
    }
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc){
    if (room.error){
       desc = NULL;
        return 0;
    } else {
        strncpy(desc, room.desc, room.descLength);
        desc[room.descLength] = '\0';
        return (room.titleLength);
    }
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void){
    room.exits = 0;
    if (room.northExit){
        room.exits += BIT_3;
    }
    if (room.eastExit){
        room.exits += BIT_2;
    }
    if (room.southExit){
        room.exits += BIT_1;
    }
    if (room.westExit){
        room.exits += BIT_0;
    }
    return room.exits;
}

int FileHandler(void) 
{

    room.error = FALSE; // no errors at the start

    // for loop index
    int i = 0;

    // for skipping room versions
    int skip_desc_len = 0;
    int skip_items_contained = 0;
    int skip_exits = 4;

    // for keeping track of the amount of items the player has which match the req items
    int itemMatch = 0;

    // holds the file name
    char fileName[FILE_NAME_SIZE];
    sprintf(fileName, "%s%d%s", ROOMFILE, room.currentRoom, TXT);
    // file pointer to the room file
    FILE* file;
    file = fopen(fileName, "r");

    if (file) {
        // skip "RPG"
        if (!fseek(file, RPG_OFFSET, SEEK_CUR)) {

            // get title length and build title array
            room.titleLength = fgetc(file);
            fgets(room.title, room.titleLength + 1, file); // fgets gets n-1 chars so + 1            

            // while player doesnt have item, keep skipping versions
            do {
                // get required items, and check if the player has them
                room.numItemsRequired = fgetc(file);
                for (i = 0; i < room.numItemsRequired; i++) {
                    if (FindInInventory(fgetc(file)) == SUCCESS) {
                        itemMatch++; // keep track of how many items match 
                    }
                }

                room.playerHasReqItems = (room.numItemsRequired == itemMatch);

                // if player does not have all required items, skip to next version.
                // fseek is nonzero when it encounters an error
                // the reason this is done using fseek rather than running through every version
                // of the room and storing the desc, items, and exits is because of the inventory.
                // the inv does not support item removal and the game breaks if more than 4 items
                // are inserted. thus we skip the versions the user cant enter rather than store. 
                if (!room.playerHasReqItems) {
                    skip_desc_len = fgetc(file);
                    if (fseek(file, skip_desc_len, SEEK_CUR)) {
                        room.error = TRUE;
                        return STANDARD_ERROR;
                    }
                    skip_items_contained = fgetc(file);
                    if (fseek(file, skip_items_contained + skip_exits, SEEK_CUR)) {
                        room.error = TRUE;
                        return STANDARD_ERROR;
                    }
                }

            } while (!room.playerHasReqItems);


            // get desc length and build desc array
            room.descLength = fgetc(file);
            fgets(room.desc, room.descLength + 1, file);


            // check items contained
            room.itemsContained = fgetc(file);
            for (i = 0; i < room.itemsContained; i++) {
                if (AddToInventory(fgetc(file)) != SUCCESS) {
                    room.error = TRUE;
                    return STANDARD_ERROR;
                }
            }
            // get exits 
            room.northExit = fgetc(file);
            room.eastExit = fgetc(file);
            room.southExit = fgetc(file);
            room.westExit = fgetc(file);
            // build exits variable            
        } else {
            room.error = TRUE;
            return STANDARD_ERROR;
        }
    } else {
        room.error = TRUE;
        return STANDARD_ERROR;
    }


    if (!fclose(file)) {
        return SUCCESS;
    } else {
        room.error = TRUE;
        return STANDARD_ERROR;
    }

    // if it somehow made it down here, return error.
    room.error = TRUE;
    return STANDARD_ERROR;
}