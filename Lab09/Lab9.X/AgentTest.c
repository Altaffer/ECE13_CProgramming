/*
 * File:   AgentTest.c
 * Author: Luca Altaffer taltaffe@ucsc.edu
 *
 * Created on March 10, 2022, 9:46 AM
 */


#include "xc.h"
#include "Agent.h"
#include "BOARD.h"
#include <stdio.h>

void main(void)
{
    BOARD_Init();

    printf("Welcome to the Agent Test!\n\n");

    //test Agent Init

    printf("Testing Agent Init...\n\n");

    AgentInit();
    //Because I did not see an easy way to test agent init I am simply calling it
    //and seeing if the other functions work because of this initialization

    //test Agent Run
    printf("Testing agent run with event type challenge received.\n");
    printf("Expected return event type = MESSAGE_ACC.\n\n");

    BB_Event tester = {BB_EVENT_CHA_RECEIVED, 10};

    if((AgentRun(tester)).type == MESSAGE_ACC){

        printf("Test successful! Agent run returned the expected event.\n\n");

    }else{

        printf("Test failed. Agent run did not return the expected output.\n\n");

    }

    printf("Testing agent run with event type start button.\n");
    printf("Expected return event type = MESSAGE_CHA.\n\n");

    AgentInit();

    BB_Event tester2 = {BB_EVENT_START_BUTTON, 10};

    if((AgentRun(tester2)).type == MESSAGE_CHA){

        printf("Test successful! Agent run returned the expected event.\n\n");

    }else{

        printf("Test failed. Agent run did not return the expected output.\n\n");

    }

    //test agent get state 
    printf("Beginning test for agent get state.\n");
    printf("After last call to agent run, state should be in AGENT_STATE_CHALLENGING.\n\n");

    if(AgentGetState() == AGENT_STATE_CHALLENGING){

        printf("Test successful! State machine kept track of current state.\n\n");

    }else{

        printf("Test failed. State machine did not keep accurate track of current state.\n\n");

    }

    printf("After another call to agent init the state should be reset back to start.\n\n");

    AgentInit();

    if(AgentGetState() == AGENT_STATE_START){

        printf("Test successful! State machine kept track of current state.\n\n");

    }else{

        printf("Test failed. State machine did not keep accurate track of current state.\n\n");

    }

    //test agent set state
    printf("Beginning test for agent get state.\n");
    printf("Attempting to set state to AGENT_STATE_ATTACKING.\n\n");

    AgentSetState(AGENT_STATE_ATTACKING);

    if(AgentGetState() == AGENT_STATE_ATTACKING){

        printf("Test successful! State was successfully set to desired state.\n\n");

    }else{

        printf("Test failed. State was not successfully switched.\n\n");

    }

    printf("Attempting to set state to AGENT_STATE_ACCEPTING.\n\n");

    AgentSetState(AGENT_STATE_ACCEPTING);

    if(AgentGetState() == AGENT_STATE_ACCEPTING){

        printf("Test successful! State was successfully set to desired state.\n\n");

    }else{

        printf("Test failed. State was not successfully switched.\n\n");

    }


    while(1);
}
