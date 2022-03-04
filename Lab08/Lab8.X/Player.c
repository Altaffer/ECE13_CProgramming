#include "UNIXBOARD.h"
#include <stdint.h>
#include "Player.h"
// Define how big the player's inventory is.
#define INVENTORY_SIZE 4
#define slot -1

// Defining variables
static uint8_t inventory[INVENTORY_SIZE] = {slot, slot, slot, slot};
static int cur_inv_size = 0;

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item){
 if (cur_inv_size < INVENTORY_SIZE){
     inventory[cur_inv_size++] = item;
     return SUCCESS;
 }else{
     return STANDARD_ERROR;
 }
}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item){
    int i = 0;
    for(i; i < INVENTORY_SIZE; i++){
        if(inventory[i] == item){
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}
