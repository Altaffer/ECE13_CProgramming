// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

//CE13 standard libraries:
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"

// Battleboats Libraries:
#include "BattleBoats.h"
#include "Agent.h"
#include "Negotiation.h"
#include "Message.h"
#include "Field.h"

int main()
{
    Field own_field;
    Field opp_field;
    FieldInit(&own_field, &opp_field);
    Field AI_own_field;
    Field AI_opp_field;
    FieldInit(&AI_own_field, &AI_opp_field);
    
    srand(11);
    
    
    GuessData opp_guess;
    GuessData AI_guess;
    opp_guess.row = 1;
    opp_guess.col = 2;
    
    int test = 0;
    
    test = FieldAddBoat(&own_field, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_LARGE);
    test = FieldAddBoat(&own_field, 2, 2, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_MEDIUM);
    test = FieldAddBoat(&own_field, 1, 2, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    
    test = FieldGetBoatStates(&own_field);
    
    //a miss
    //FieldRegisterEnemyAttack(&own_field, &opp_guess);
    /*
    //a hit
    opp_guess.col = 1;
    FieldRegisterEnemyAttack(&own_field, &opp_guess);
    //run to sinking the ship
    opp_guess.row = 2;
    FieldRegisterEnemyAttack(&own_field, &opp_guess);
    opp_guess.row = 3;
    FieldRegisterEnemyAttack(&own_field, &opp_guess);
     * */
    
    test = FieldGetBoatStates(&own_field);
    
    test = FieldAIPlaceAllBoats(&AI_own_field);
    
    while(1)
    {
        AI_guess = FieldAIDecideGuess(&AI_opp_field);
    
        FieldRegisterEnemyAttack(&own_field, &AI_guess);
    
        FieldUpdateKnowledge(&AI_opp_field, &AI_guess);
    }
    
    
    
    return 0;
}