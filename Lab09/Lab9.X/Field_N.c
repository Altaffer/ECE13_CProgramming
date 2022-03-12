/*
 *  @file Field.c
 *  @authr Nicholas Kuipers
 *  @date 3/4/2022
 * 
 *  @purpose Definitions for Message.h library
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BOARD.h"
#include "Field.h"

//Variables for FieldAIDecideGuess() function
typedef enum{
    SEARCHING,
    TARGET_IDENTIFIED,
    DIRECTION_IDENTIFIED
}State;

typedef enum{
    NORTH,
    EAST,
    SOUTH,
    WEST
}Target_Direction;

typedef struct{
    uint8_t row;
    uint8_t col;
}Square_Marker;

static State AI_State = SEARCHING;
static Target_Direction shot_direction = NORTH;
static uint8_t AI_Grid[FIELD_ROWS][FIELD_COLS];
static Square_Marker initial_hit;
static Square_Marker AI_Shot_Guess;
static GuessData AIGuess;


//Helper functions
void PlaceBoat(Field *own_field, uint8_t row, uint8_t col, BoatType boat_type);
void SetBoatLives(Field *own_field, BoatType boat_type);
void checkTargetingDirection(const Field *opp_field);
void moveInTargetingDirection(const Field *opp_field);
void flipTargetingDirection();
void clearBoat_AIGrid(uint8_t boat_lives);

/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus (
 * FIELD_SQUARE_EMPTY for your own field, FIELD_SQUARE_UNKNOWN for opponent's).
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be filled when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */
void FieldInit(Field *own_field, Field * opp_field)
{
    //initialize the grid squares, including the AI_Grid
    for(int i = 0; i < FIELD_ROWS; i++)
        for(int j = 0; j < FIELD_COLS; j++)
        {
            own_field->grid[i][j] = FIELD_SQUARE_EMPTY;
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN;
            AI_Grid[i][j] = FIELD_SQUARE_UNKNOWN;
        }
    //opponent's field's boatLives parameters are filled
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  FIELD_SQUARE_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced square 
 */
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col)
{
    if((row >= 0 && row < FIELD_ROWS) && (col >= 0 && col < FIELD_COLS))
    {
        return f->grid[row][col];
    }
    
    return FIELD_SQUARE_INVALID;      //row and/or col are not valid field locations
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p)
{
    if((row >= 0 && row < FIELD_ROWS) && (col >= 0 && col < FIELD_COLS))
    {
        SquareStatus oldValue = f->grid[row][col];
        f->grid[row][col] = p;
        return oldValue;
    }
    
    return FIELD_SQUARE_INVALID;      //row and/or col are not valid field locations
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. 
 * 
 * All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 * 
 * In addition, this function should update the appropriate boatLives parameter of the field.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type)
{
    //first check that the specified row and col values are valid
    if((row >= 0 && row < FIELD_ROWS) && (col >= 0 && col < FIELD_COLS))
    {
        uint8_t boat_size = 3 + boat_type;     //size of the ship passed in boat_type
        
        if(dir == FIELD_DIR_EAST)        //boat will be facing east -- row remains the same
        {
            if(col + boat_size > FIELD_COLS)         //check if the boat will cross outside the field
                return STANDARD_ERROR;
            for(int i = 0; i < boat_size; i++)       //check that all squares (where boat will go) are empty
                if(own_field->grid[row][col + i] != FIELD_SQUARE_EMPTY)
                    return STANDARD_ERROR;
            
            for(int i = 0; i < boat_size; i++)       //all squares are available. Place respective value in each square
                PlaceBoat(own_field, row, col + i, boat_type);      //helper function contains switch
            SetBoatLives(own_field, boat_type);      //set respective boat lives
        }
        
        else if(dir == FIELD_DIR_SOUTH)    //boat will be facing south -- col remains the same
        {
            if(row + boat_size > FIELD_ROWS)         //check if the boat will cross outside the field
                return STANDARD_ERROR;
            for(int i = 0; i < boat_size; i++)       //check that all squares (where boat will go) are empty
                if(own_field->grid[row + i][col] != FIELD_SQUARE_EMPTY)
                    return STANDARD_ERROR;
            
            for(int i = 0; i < boat_size; i++)       //all squares are available. Place respective value in each square
                PlaceBoat(own_field, row + i, col, boat_type);      //helper function contains switch
            SetBoatLives(own_field, boat_type);      //set respective boat lives
        }
        
        //if everything worked properly, returns a success
        return SUCCESS;
    }
    else
        return STANDARD_ERROR;      //row and/or col are not valid field locations
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.  The result can be a RESULT_HIT, RESULT_MISS, or RESULT_***_SUNK.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess)
{
    SquareStatus hitSquare = own_field->grid[opp_guess->row][opp_guess->col];
    switch(hitSquare)
    {
        case FIELD_SQUARE_EMPTY:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_MISS;
            opp_guess->result = RESULT_MISS;
            break;
        case FIELD_SQUARE_SMALL_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->smallBoatLives--;
            if(own_field->smallBoatLives == 0)
                opp_guess->result = RESULT_SMALL_BOAT_SUNK;
            else
                opp_guess->result = RESULT_HIT;
            break;
        case FIELD_SQUARE_MEDIUM_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->mediumBoatLives--;
            if(own_field->mediumBoatLives == 0)
                opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
            else
                opp_guess->result = RESULT_HIT;
            break;
        case FIELD_SQUARE_LARGE_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->largeBoatLives--;
            if(own_field->largeBoatLives == 0)
                opp_guess->result = RESULT_LARGE_BOAT_SUNK;
            else
                opp_guess->result = RESULT_HIT;
            break;
        case FIELD_SQUARE_HUGE_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->hugeBoatLives--;
            if(own_field->hugeBoatLives == 0)
                opp_guess->result = RESULT_HUGE_BOAT_SUNK;
            else
                opp_guess->result = RESULT_HIT;
            break;
        default:
            opp_guess->result = RESULT_MISS;
            return FIELD_SQUARE_INVALID;
    }
    return hitSquare;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within own_guess was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The Field struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess)
{
    SquareStatus hitSquare = opp_field->grid[own_guess->row][own_guess->col];
    switch(own_guess->result)
    {
        case RESULT_MISS:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_MISS;
            if(AI_State == TARGET_IDENTIFIED)
            {
                shot_direction++;
            }
            if(AI_State == DIRECTION_IDENTIFIED)
            {
                flipTargetingDirection();
                AI_Shot_Guess.row = initial_hit.row;
                AI_Shot_Guess.col = initial_hit.col;
            }
            break;
        case RESULT_HIT:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            AI_Grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            if(AI_State == SEARCHING)             //used for state machine in later function
            {
                AI_State = TARGET_IDENTIFIED;
                initial_hit.row = AIGuess.row;
                initial_hit.col = AIGuess.col;
            }
            else if(AI_State == TARGET_IDENTIFIED)
                AI_State = DIRECTION_IDENTIFIED;
            break;
        case RESULT_SMALL_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            clearBoat_AIGrid(opp_field->smallBoatLives);
            opp_field->smallBoatLives = 0;
            break;
        case RESULT_MEDIUM_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            clearBoat_AIGrid(opp_field->mediumBoatLives);
            opp_field->mediumBoatLives = 0;
            break;
        case RESULT_LARGE_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            clearBoat_AIGrid(opp_field->largeBoatLives);
            opp_field->largeBoatLives = 0;
            break;
        case RESULT_HUGE_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            clearBoat_AIGrid(opp_field->hugeBoatLives);
            opp_field->hugeBoatLives = 0;
            break;
        default:
            return FIELD_SQUARE_INVALID;
    }
    return hitSquare;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f)
{
    uint8_t boatStates = 0b00000000;
    
    if(f->smallBoatLives > 0)
        boatStates |= FIELD_BOAT_STATUS_SMALL;
    if(f->mediumBoatLives > 0)
        boatStates |= FIELD_BOAT_STATUS_MEDIUM;
    if(f->largeBoatLives > 0)
        boatStates |= FIELD_BOAT_STATUS_LARGE;
    if(f->hugeBoatLives > 0)
        boatStates |= FIELD_BOAT_STATUS_HUGE;
    
    return boatStates;
}


/**
 * This function is responsible for placing all four of the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 */
uint8_t FieldAIPlaceAllBoats(Field *own_field)
{
    //AI places its boats from largest to smallest
    BoatType boat = FIELD_BOAT_TYPE_HUGE;
    BoatDirection direction;
    uint8_t row_guess = 0;
    uint8_t col_guess = 0;
    int errorCounter = 0;           //if this goes over 100, function returns an error
    
    do
    {
        row_guess = rand() % FIELD_ROWS;
        col_guess = rand() % FIELD_COLS;
        direction = rand() & 0x1;
        int result = FieldAddBoat(own_field, row_guess, col_guess, direction, boat);
        if(result == STANDARD_ERROR)       //if boat couldn't be added
        {
            boat++;                 //counter the boat-- below, AI attempts to place the boat again in the next loop
            if(++errorCounter >= 100)
                return STANDARD_ERROR;
        }
    }
    while(boat-- != FIELD_BOAT_TYPE_SMALL);
    
    return SUCCESS;
}

/**
 * Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a square which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
GuessData FieldAIDecideGuess(const Field *opp_field)
{
    /*
     *  This will make use of a state machine. A HIT_GRID static variable tracks AI hits.
     *      - Searching
     *          - Randomly chooses positions to fire upon. Stays here until AI scores a hit.
     *      - Target_Identified 
     *          - Triggered upon the first hit. Positions fired upon are each of the cardinal
     *              directions around the initial hit point (randomly chosen)
     *          - Also triggers when a ship sunk and no additional hits were identified
     *      - Direction Identified
     *          - Triggered upon second hit from Target_Identified. Fires along the direction
     *              pointed to by the direction of the second hit.
     *          - If hit && not sunk, fires at the next position on path next turn
     *          - If miss && not sunk, or next position is invalid, aims at first position in
     *              opposite direction from initial hit point
     *          - If miss && not sunk in both directions, aims at positions in remaining cardinal
     *              directions at initial hit point
     *          - If hit && sunk, hit points on the HIT_GRID from the sinking hit to #boatlives - 1
     *              are removed.
     *                  - If there are no remaining hits on HIT_GRID, return to Searching state
     *                  - If there are remaining hits on HIT_GRID, set one of the hits to new initial hit
     *                      point and move to Target_Identified state
     */
    //SEARCHING state is the simplest: it chooses a random row and column to fire upon (that hasn't already been fired upon)
    switch(AI_State)
    {
        case SEARCHING:
            while(1)
            {
                //can be made more efficient by organizing all spaces into a linked list
                AI_Shot_Guess.row = rand() % FIELD_ROWS;
                AI_Shot_Guess.col = rand() % FIELD_COLS;
                if(FieldGetSquareStatus(opp_field, AI_Shot_Guess.row, AI_Shot_Guess.col) == FIELD_SQUARE_UNKNOWN)
                {
                    AIGuess.row = AI_Shot_Guess.row;
                    AIGuess.col = AI_Shot_Guess.col;
                    break;
                }
            }
            break; 
        case TARGET_IDENTIFIED:
            checkTargetingDirection(opp_field);
            break;
        case DIRECTION_IDENTIFIED:
            moveInTargetingDirection(opp_field);
            break;
    }
    
    return AIGuess;
}

/*
 *  The following functions are "Helper Functions" that assist the functions identified in Field.h. 
 *  Most of these relate to the AI logic contained in this file.
 */

/**
 * This places a specific boat type in the desired coordinates of the field pointed to by f. The coordinates have
 * been vetted in a prior function before coming here.
 * @param f
 * @param row
 * @param col
 * @param boat_type
 */
void PlaceBoat(Field *f, uint8_t row, uint8_t col, BoatType boat_type)
{
    switch(boat_type)
    {
        case FIELD_BOAT_TYPE_SMALL:
            f->grid[row][col] = FIELD_SQUARE_SMALL_BOAT;
            break;
        case FIELD_BOAT_TYPE_MEDIUM:
             f->grid[row][col] = FIELD_SQUARE_MEDIUM_BOAT;
             break;
        case FIELD_BOAT_TYPE_LARGE:
            f->grid[row][col] = FIELD_SQUARE_LARGE_BOAT;
            break;
        case FIELD_BOAT_TYPE_HUGE:
            f->grid[row][col] = FIELD_SQUARE_HUGE_BOAT;
            break;
    }
}

/**
 * Simple switch statement that sets the boat lives for the field own_field, depending on the contained boat_type
 * @param own_field
 * @param boat_type
 */

void SetBoatLives(Field *own_field, BoatType boat_type)
{
    switch(boat_type)
    {
        case FIELD_BOAT_TYPE_SMALL:
            own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
            break;
        case FIELD_BOAT_TYPE_MEDIUM:
             own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
             break;
        case FIELD_BOAT_TYPE_LARGE:
            own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
            break;
        case FIELD_BOAT_TYPE_HUGE:
            own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
            break;
    }
}

/**
 * This is used in the AI state machine state TARGET_IDENTIFIED. When this function is called, the AI has
 * scored an initial hit and is now cycling through the cardinal directions counter-clockwise until another hit is scored.
 * @param opp_field
 */
void checkTargetingDirection(const Field *opp_field)
{
    while(1)
    {
        switch(shot_direction)
        {
            case NORTH:
                AI_Shot_Guess.row = initial_hit.row - 1;
                AI_Shot_Guess.col = initial_hit.col;
                 break;
            case EAST:
                AI_Shot_Guess.row = initial_hit.row;
                AI_Shot_Guess.col = initial_hit.col + 1;
                break;
            case SOUTH:
                AI_Shot_Guess.row = initial_hit.row + 1;
                AI_Shot_Guess.col = initial_hit.col;
                break;
             case WEST:
                AI_Shot_Guess.row = initial_hit.row;
                AI_Shot_Guess.col = initial_hit.col - 1;
                break;
            default:
                shot_direction = NORTH;
                AI_State = SEARCHING;
                AIGuess.row = -1;
                AIGuess.col = -1;
        }
        if(FieldGetSquareStatus(opp_field, AI_Shot_Guess.row, AI_Shot_Guess.col) == FIELD_SQUARE_UNKNOWN)
        {
            AIGuess.row = AI_Shot_Guess.row;
            AIGuess.col = AI_Shot_Guess.col;
            break;
        }
        else
            shot_direction++;
    }
}

/**
 * This is used in the AI state machine state DIRECTION_ACQUIRED. This moves the targeting 'cursor' in the direction
 * identified in the previous state. This will be continually called until either a ship is sunk or a miss is returned
 * @param opp_field
 */
void moveInTargetingDirection(const Field *opp_field)
{
    while(1)
    {
        switch(shot_direction)
        {
            case NORTH:
                AI_Shot_Guess.row = AI_Shot_Guess.row - 1;
                AI_Shot_Guess.col = initial_hit.col;
                break;
            case EAST:
                AI_Shot_Guess.row = initial_hit.row;
                AI_Shot_Guess.col = AI_Shot_Guess.col + 1;
                break;
            case SOUTH:
                AI_Shot_Guess.row = AI_Shot_Guess.row + 1;
                AI_Shot_Guess.col = initial_hit.col;
                break;
            case WEST:
                AI_Shot_Guess.row = initial_hit.row;
                AI_Shot_Guess.col = AI_Shot_Guess.col - 1;
                break;
            default:
                shot_direction = NORTH;
                AI_State = SEARCHING;
                AIGuess.row = -1;
                AIGuess.col = -1;
        }
        if(FieldGetSquareStatus(opp_field, AI_Shot_Guess.row, AI_Shot_Guess.col) == FIELD_SQUARE_UNKNOWN)
        {
            AIGuess.row = AI_Shot_Guess.row;
            AIGuess.col = AI_Shot_Guess.col;
            break;
        }
        else
        {
            flipTargetingDirection();
            AI_Shot_Guess.row = initial_hit.row;
            AI_Shot_Guess.col = initial_hit.col;
        }
    }
}

/**
 * This simply flips the current directions the targeting 'cursor' is traversing. Useful in DIRECTION_IDENTIFIED when a miss
 * is encountered but the ship is not yet sunk
 */
void flipTargetingDirection()
{
    switch(shot_direction)
    {
        case NORTH:
            shot_direction = SOUTH;
            break;
        case SOUTH:
            shot_direction = NORTH;
            break;
        case EAST:
            shot_direction = WEST;
            break;
        case WEST:
            shot_direction = EAST;
            break;
    }
}

/**
 * AI_Grid is an internal grid used by the AI to keep track of hits that are not yet part of a sunken ship. This is used when
 * a ship is sunk so the respective hits are cleared, and the grid is checked for any remaining hits (just as a human would)
 * @param boat_lives
 */
void clearBoat_AIGrid(uint8_t boat_lives)
{
    //might be able to use AIGuess rather than killSpot
    Square_Marker killSpot;
    killSpot.row = AIGuess.row;
    killSpot.col = AIGuess.col;
    
    //first, clear the hits from the respective ship off the AI_Grid
    flipTargetingDirection();             //We're going to be moving the opposite direction as we clear out the ship
    AI_Grid[AI_Shot_Guess.row][AI_Shot_Guess.col] = FIELD_SQUARE_MISS;
    for(int i = 0; i < boat_lives - 1; i++)
    {
        switch(shot_direction)
        {
            case NORTH:
                AI_Shot_Guess.row = AI_Shot_Guess.row - 1;
                AI_Shot_Guess.col = initial_hit.col;
                break;
            case EAST:
                AI_Shot_Guess.row = initial_hit.row;
                AI_Shot_Guess.col = AI_Shot_Guess.col + 1;
                break;
            case SOUTH:
                AI_Shot_Guess.row = AI_Shot_Guess.row + 1;
                AI_Shot_Guess.col = initial_hit.col;
                break;
            case WEST:
                AI_Shot_Guess.row = initial_hit.row;
                AI_Shot_Guess.col = AI_Shot_Guess.col - 1;
                break;
        }
        AI_Grid[AI_Shot_Guess.row][AI_Shot_Guess.col] = FIELD_SQUARE_MISS;
    }
    
    //Now, we check the board for any other hits
    uint8_t ID = FALSE;
    for(int i = 0; i < FIELD_ROWS; i++)
    {
        for(int j = 0; j < FIELD_COLS; j++)
            if(AI_Grid[i][j] == FIELD_SQUARE_HIT)
            {
                initial_hit.row = i;
                initial_hit.col = j;
                AI_State = TARGET_IDENTIFIED;
                ID = TRUE;
                break;
            }
        if(ID == TRUE)
            break;
    }
           
    
    if(AI_State == DIRECTION_IDENTIFIED)
        AI_State = SEARCHING;
}