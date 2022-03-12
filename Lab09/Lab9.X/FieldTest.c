/*
 * File:   FieldTest.c
 * Author: Luca Altaffer taltaffe@ucsc.edu
 *
 * Created on March 11, 2022, 3:09 PM
 */

#include "xc.h"
#include "Field.h"
#include "BOARD.h"
#include <stdio.h>
#include "BattleBoats.h"

void main(void) {
    BOARD_Init();

    printf("Welcome to the Field Test!\n\n");

    printf("Testing field init and field print uart.\n");
    printf("Field print requires field init to have worked,\n");
    printf("so these function tests work hand in hand to prove each other.\n\n");

    Field own_field, opp_field;

    FieldInit(&own_field, &opp_field);

    FieldPrint_UART(&own_field, &opp_field);

    printf("\nTest successful! Field initializes and prints.\n\n");

    //test field get square status
    printf("Testing field get square status.\n");
    printf("Square status of our own field should be empty.\n");

    if (FieldGetSquareStatus(&own_field, 1, 1) == FIELD_SQUARE_EMPTY) {

        printf("Test successful! Field get square status retrieved correct status.\n\n");

    }else {

        printf("Test failed. Field get square status did not retrieve correct status.\n\n");

    }
    
    printf("Square status of the opposing field should be unknown. \n");
    
    if(FieldGetSquareStatus(&opp_field, 1, 1) == FIELD_SQUARE_UNKNOWN){

        printf("Test successful! Field get square status retrieved correct status.\n\n");

    }else{

        printf("Test failed. Field get square status did not retrieve correct status.\n\n");

    }
    
    //test field set square status
    printf("Testing field set square status.\n");
    printf("Setting our own field's status to FIELD_SQUARE_HIT.\n");

    FieldSetSquareStatus(&own_field, 1, 1, FIELD_SQUARE_HIT);

    if(FieldGetSquareStatus(&own_field, 1, 1) == FIELD_SQUARE_HIT){

        printf("Test successful! Square status successfully changed.\n\n");

    }else{
        printf("Test failed. Square status not changed correctly.\n\n");

    }
    
     printf("Attempting to set square status back to FIELD_SQUARE_EMPTY.\n");

    FieldSetSquareStatus(&own_field, 1, 1, FIELD_SQUARE_EMPTY);

    if(FieldGetSquareStatus(&own_field, 1, 1) == FIELD_SQUARE_EMPTY){

        printf("Test successful! Square status successfully changed.\n\n");

    }else{

        printf("Test failed. Square status not changed correctly.\n\n");

    }

    //test field add boat
    printf("Testing field add boat.\n");
    printf("Adding a south facing boat from the top left corner.\n");

    FieldAddBoat(&own_field, 1, 1, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);

    if(FieldGetSquareStatus(&own_field, 1, 1) == FIELD_SQUARE_SMALL_BOAT){

        printf("Test successful! Boat added to field.\n\n");

    }else{

        printf("Test failed. Boat not added correctly to field.\n\n");

    }

    printf("Testing field add boat in the east direction now.\n");

    FieldAddBoat(&own_field, 5 , 1, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);

    if(FieldGetSquareStatus(&own_field, 5, 2) == FIELD_SQUARE_MEDIUM_BOAT){

        printf("Test successful! Boat added to field.\n\n");

    }else{

        printf("Test failed. Boat not added correctly to field.\n\n");

    }

    //test field register enemy attack
    printf("Creating guess data to test field register enemy attack.\n");

    GuessData guess = {1, 1,};

    printf("Testing field register enemy attack.\n");

    if(FieldRegisterEnemyAttack(&own_field, &guess) == FIELD_SQUARE_SMALL_BOAT){

        printf("Test successful! Function returned correct previous square state.\n\n");

    }else{

        printf("Test failed. Function failed to return correct square state.\n\n");

    }

    printf("Testing field register enemy attack's ability to set shot result.\n");

    if(guess.result == RESULT_HIT){

        printf("Test successful! Register enemy attack set guess data's result.\n\n");

    }else{

        printf("Test failed. Register enemy attack did not set guess data's result.\n\n");

    }

    //test field update knowledge
    printf("Testing field update knowledge to return square status of opponent field.\n");
    printf("Adding a medium boat to opponent's field and firing upon it.\n");

    FieldAddBoat(&opp_field, 5 , 1, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);

    GuessData guess2 = {5, 1,};

    if(FieldUpdateKnowledge(&opp_field, &guess2) == FIELD_SQUARE_UNKNOWN){

        printf("Test successful! Field update knowledge returned correct square status.\n\n");

    }else{

        printf("Test failed. Update knowledge did not return the correct square status.\n\n");

    }

    printf("Testing field update knowledge to see if enemy field was updated.\n");

    if(FieldGetSquareStatus(&opp_field, 5, 1) == FIELD_SQUARE_HIT){

        printf("Test successful! Enemy field updated correctly.\n\n");

    }else{

        printf("Test failed. Enemy field was not update properly.\n\n");

    }

    //test field get boat states
    printf("Testing field get boat states on my own field.\n");
    printf("Given that in this test harness we have only added a small and medium boat\n");
    printf("the expected return value is 0x03.\n");

    if(FieldGetBoatStates(&own_field) == 3){

        printf("Test successful! Get boat states accounted for all present boats.\n\n");

    }else{

        printf("Test failed. Get boat states did not account for the present boats.\n\n");

    }

    printf("Testing get boat states on opponent's field.\n");
    printf("Calling field init sets full boat lives for the opponent\n");
    printf("field so the expected output is 0x0F.\n");

    if(FieldGetBoatStates(&opp_field) == 0x0F){

        printf("Test successful! Get boat states accounted for all present boats.\n\n");

    }else{

        printf("Test failed. Get boat states did not account for the present boats.\n\n");

    }

    //test field AI place all boats
    printf("Testing field AI place all boats on my own field.\n");
    printf("A call to get boat states after placing all boats should return 0x0F.\n");
    printf("I also re-initialize both fields so there are no extra boats.\n");

    FieldInit(&own_field, &opp_field);

    FieldAIPlaceAllBoats(&own_field);

    if(FieldGetBoatStates(&own_field) == 0x0F){

        printf("Test successful! The AI managed to place all the boats.\n\n");

    }else{

        printf("Test failed. The AI did not manage to place all the boats.\n\n");

    }

    //test field AI decide guess
    printf("Testing field AI decide guess.\n");

    FieldAddBoat(&opp_field, 0 , 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);

    GuessData guess3 = {0, 0,};

    FieldUpdateKnowledge(&opp_field, &guess3);

    printf("I have added a medium sized boat in the east direction beginning at\n");
    printf("the origin 0,0 and fired upon the origin as well. If decide guess works\n");
    printf("correctly it will not guess the origin 0,0 but another place instead.\n\n");

    if(FieldAIDecideGuess(&opp_field).col != 0){

        printf("Test successful! The function did not guess a point that\n");
        printf("had already been guessed.\n\n");

    }else{

        printf("Test failed. The function fired at a single spot multiple times.\n\n");

    }

    GuessData guess4 = FieldAIDecideGuess(&opp_field);

    FieldUpdateKnowledge(&opp_field, &guess4);

    printf("To finish this test harness I will once again print the fields to\n");
    printf("the UART so you can see the guess and the boats.\n\n");


    FieldPrint_UART(&own_field, &opp_field);


    while(1);
}
    
}
