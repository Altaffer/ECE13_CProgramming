/*
 * File:   Agent.c
 * Author: Luca Altaffer taltaffe@ucsc.edu
 *
 * Created on March 10, 2022, 9:46 AM
 */

#include <stdint.h>
#include <stdio.h>
#include <xc.h>

#include "Agent.h"
#include "Field.h"
#include "BOARD.h"
#include "Oled.h"
#include "Message.h"
#include "Negotiation.h"
#include "BattleBoats.h"

typedef enum {
    // outcomes of the game or still playing the game
    PLAYING,
    CHEATING,
    WIN,
    LOSS
} EndStates;

typedef struct {// all data an agent can access
    // store own field and a knowledge of opp field
    // Agent State
    AgentState State;

    // set the outcome of the game
    EndStates Outcome;

    // guess at where to shoot
    GuessData guess;

    // store own field and a knowledge of opponent field
    Field ownField;
    Field oppField;

    // A, #a, B
    NegotiationData A;
    NegotiationData A_hash;
    NegotiationData B;
    // coin flip result
    NegotiationOutcome coinFlipResult;

    // holds return or respective field function
    SquareStatus regEnemyAttack;
    SquareStatus updatedKnowledge;

    // message to return
    Message returnMessage;

    // need to store and int turn counter
    int turnCount;
} AgentData;

static AgentData Agent;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *   -setting the start state of the Agent SM.
 *   -setting turn counter to 0
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * It is not advised to call srand() inside of AgentInit.  
 *  */
void AgentInit(void) {
    FieldInit(&Agent.ownField, &Agent.oppField);
    FieldAIPlaceAllBoats(&Agent.ownField);

    Agent.A = 0;
    Agent.A_hash = NegotiationHash(Agent.A);
    Agent.B = 0;

    Agent.Outcome = PLAYING;

    Agent.State = AGENT_STATE_START;

    Agent.turnCount = 0;

    Agent.returnMessage.type = MESSAGE_NONE;
    Agent.returnMessage.param0 = 0;
    Agent.returnMessage.param1 = 0;
    Agent.returnMessage.param2 = 0;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event) {
    switch (Agent.State) {

        case(AGENT_STATE_START):
            /*
             * AGENT_STATE_START, //0
             * - Display new game message
             * - Negotiation Hash
             * - Set up your field
             * - Place boats
             * - Go to Challenge or Accepting state
             * --- (you pressed button to challenge)
             * --- (you received a challenge "CHA")
             * 
             */
            // <editor-fold defaultstate="collapsed" desc="AGENT_STATE_START">
            if (event.type == BB_EVENT_START_BUTTON) {
                // generate A, #a
                printf("START BUTTON PRESSED!\n");
                Agent.A = rand() & 0xFFFF;
                Agent.A_hash = NegotiationHash(Agent.A);
                // send CHA
                Agent.returnMessage.type = MESSAGE_CHA;
                Agent.returnMessage.param0 = Agent.A_hash;
                // go to challenging
                Agent.State = AGENT_STATE_CHALLENGING;

            } else if (event.type == BB_EVENT_CHA_RECEIVED) {
                // store #a
                Agent.A_hash = event.param0;
                // generate B
                Agent.B = rand() & 0xFFFF;
                // send ACC
                Agent.returnMessage.type = MESSAGE_ACC;
                Agent.returnMessage.param0 = Agent.B;
                // go to accepting
                Agent.State = AGENT_STATE_ACCEPTING;
            }
            // </editor-fold>
            break;

        case(AGENT_STATE_CHALLENGING):
            /*
             * AGENT_STATE_CHALLENGING, //1
             * - When ACC from opp is received, coin flip
             * - if coin flip goes your way go to waiting to send
             * - if coin flip doesnt go your way go to defending
             * 
             */
            // <editor-fold defaultstate="collapsed" desc="AGENT_STATE_CHALLENGING">
            if (event.type == BB_EVENT_ACC_RECEIVED) {
                // get B from event
                Agent.B = event.param0;
                Agent.returnMessage.type = MESSAGE_REV;
                Agent.returnMessage.param0 = Agent.A;
                // coin flip
                Agent.coinFlipResult = NegotiateCoinFlip(Agent.A, Agent.B);
                if (Agent.coinFlipResult == HEADS) {
                    Agent.State = AGENT_STATE_WAITING_TO_SEND;
                } else if (Agent.coinFlipResult == TAILS) {
                    Agent.State = AGENT_STATE_DEFENDING;
                }
            } else if(event.type == BB_EVENT_RESET_BUTTON) {
                // nada
            }
            // </editor-fold>
            break;

        case(AGENT_STATE_ACCEPTING):
            /*
             * AGENT_STATE_ACCEPTING, //2
             * - When REV is received
             * - if coin flip goes your way go to attacking
             * - if coin flip doesnt go your way go to defending
             * 
             */
            // <editor-fold defaultstate="collapsed" desc="AGENT_STATE_ACCEPTING">

            if (event.type == BB_EVENT_REV_RECEIVED) {

                Agent.A = event.param0;

                if (NegotiationVerify(Agent.A, Agent.A_hash)) {

                    // not cheating
                    Agent.coinFlipResult = NegotiateCoinFlip(Agent.A, Agent.B);
                    if (Agent.coinFlipResult == TAILS) {
                        Agent.returnMessage.type = MESSAGE_NONE;
                        Agent.State = AGENT_STATE_DEFENDING;
                    } else if (Agent.coinFlipResult == HEADS) {
                        // decide a coordinate to guess
                        Agent.guess = FieldAIDecideGuess(&Agent.oppField);
                        // create the message to send

                        Agent.returnMessage.type = MESSAGE_SHO;
                        Agent.returnMessage.param0 = Agent.guess.row;
                        Agent.returnMessage.param1 = Agent.guess.col;

                        Agent.State = AGENT_STATE_ATTACKING;
                    }

                } else {
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Cheating Detected.");
                    OledUpdate();
                    Agent.returnMessage.type = MESSAGE_NONE;
                    Agent.Outcome = CHEATING;
                    Agent.State = AGENT_STATE_END_SCREEN;
                }

            }


            // </editor-fold>
            break;

        case(AGENT_STATE_ATTACKING):
            /*
             * AGENT_STATE_ATTACKING, //3
             * - When RES is received
             * - get payload and update field knowledge
             * - did you win?
             * - yes print ending
             * - not yet go to defending
             * 
             */
            // <editor-fold defaultstate="collapsed" desc="AGENT_STATE_ATTACKING">
            // should this state check if res coordinates == its guess?
            if (event.type == BB_EVENT_RES_RECEIVED) {
                // update record of enemy field
                Agent.guess.row = event.param0;
                Agent.guess.col = event.param1;
                Agent.guess.result = event.param2;
                Agent.updatedKnowledge = FieldUpdateKnowledge(&Agent.oppField, &Agent.guess);

                // if get boat states is 0 the player won, go to end
                // else go defending
                if (!FieldGetBoatStates(&Agent.oppField)) {
                    Agent.Outcome = WIN;
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("You sunk all enemy ships");
                    OledUpdate();
                    Agent.State = AGENT_STATE_END_SCREEN;
                } else {
                    Agent.State = AGENT_STATE_DEFENDING;
                }

                Agent.returnMessage.type = MESSAGE_RES;
            }
            // </editor-fold>
            break;

        case(AGENT_STATE_DEFENDING):
            /*
             * AGENT_STATE_DEFENDING, //4
             * - When SHO is received
             * - get payload and register enemy attack
             * - did you lose?
             * - yes print ending
             * - not yet go to waiting to sent
             * 
             */
            // <editor-fold defaultstate="collapsed" desc="AGENT_STATE_DEFENDING">
            if (event.type == BB_EVENT_RESET_BUTTON) {
                AgentInit();
            } else if (event.type == BB_EVENT_SHO_RECEIVED) {
                // update own field based on event params                
                Agent.guess.row = event.param0;
                Agent.guess.col = event.param1;
                Agent.regEnemyAttack = FieldRegisterEnemyAttack(&Agent.ownField, &Agent.guess);

                // send RES
                Agent.returnMessage.type = MESSAGE_RES;
                Agent.returnMessage.param0 = event.param0;
                Agent.returnMessage.param1 = event.param1;
                // use regEnemyAttack to figure out if the shot was a hit or miss
                // if it is a part of a boat then shot hit
                // else any other return means shot missed
                if ((Agent.regEnemyAttack == FIELD_SQUARE_HUGE_BOAT) ||
                        (Agent.regEnemyAttack == FIELD_SQUARE_LARGE_BOAT) ||
                        (Agent.regEnemyAttack == FIELD_SQUARE_MEDIUM_BOAT) ||
                        (Agent.regEnemyAttack == FIELD_SQUARE_SMALL_BOAT)) {
                    Agent.returnMessage.param2 = TRUE;
                } else {
                    Agent.returnMessage.param2 = FALSE;
                }

                if (!FieldGetBoatStates(&Agent.ownField)) {
                    Agent.Outcome = LOSS;
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString("Enemy sunk all of your ships");
                    OledUpdate();
                    Agent.State = AGENT_STATE_END_SCREEN;
                } else {
                    Agent.State = AGENT_STATE_WAITING_TO_SEND;
                }
            }
            // </editor-fold>
            break;

        case(AGENT_STATE_WAITING_TO_SEND):
            /*
             * AGENT_STATE_WAITING_TO_SEND, //5
             * - on MESSAGE sent 
             * - decide new guess on enemy field
             * - go to attacking
             * 
             */
            // <editor-fold defaultstate="collapsed" desc="AGENT_STATE_W2S">
            if (event.type == BB_EVENT_MESSAGE_SENT) {
                Agent.turnCount++;
                // decide a coordinate to guess
                Agent.guess = FieldAIDecideGuess(&Agent.oppField);

                // create the message to send
                Agent.returnMessage.type = MESSAGE_SHO;
                Agent.returnMessage.param0 = Agent.guess.row;
                Agent.returnMessage.param1 = Agent.guess.col;

                Agent.State = AGENT_STATE_ATTACKING;
            }
            // </editor-fold>
            break;

        case (AGENT_STATE_END_SCREEN):
            /*
             * AGENT_STATE_END_SCREEN, //6
             * - break
             * 
             */
            Agent.returnMessage.type = MESSAGE_NONE;
            break;

        case (AGENT_STATE_SETUP_BOATS):
            // for now just break 
            break;
    }
    return Agent.returnMessage;
}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void) {
    return Agent.State;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState) {
    Agent.State = newState;
}
