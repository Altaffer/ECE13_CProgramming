/*
 * Author: Luca Altaffer
 * Title: Agent.c
 * Desc: Implements the Agent AI state machine functions in Agent.h. 
 */

#include <stdint.h>
#include <stdio.h>

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
    Field ownField;
    Field oppField;

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

    Agent.A = 0;
    Agent.A_hash = 0;
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
                Agent.A = rand();
                Agent.A_hash = NegotiationHash(Agent.A);
                // send CHA
                Agent.returnMessage.type = MESSAGE_CHA;
                Agent.returnMessage.param0 = Agent.A_hash;
                // initialize fields
                FieldInit(&Agent.ownField, &Agent.oppField);
                //place own boats
                FieldAIPlaceAllBoats(&Agent.ownField);
                // go to challenging
                Agent.State = AGENT_STATE_CHALLENGING;

            } else if (event.type == BB_EVENT_CHA_RECEIVED) {
                // store #a
                Agent.A_hash = event.param0;
                // generate B
                Agent.B = rand();
                // send ACC
                Agent.returnMessage.type = MESSAGE_ACC;
                Agent.returnMessage.param0 = Agent.B;
                // init fields
                Field.Init(&Agent.ownField, &Agent.oppField);
                // place boats
                FieldAIPlaceAllBoats(&Agent.ownField);
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
            if (event.type == BB_EVENT_RESET_BUTTON) {
                AgentInit();

            } else if (event.type == BB_EVENT_ACC_RECEIVED) {
                // get B from event
                Agent.B = event.param0;
                Agent.returnMessage.type = MESSAGE_REV;
                Agent.returnMessage.param0 = Agent.A;
                // coin flip
                Agent.coinFlipResult = NegotiateCoinFlip(Agent.A, Agent.B);
                if (Agent.coinFlipResult == HEADS) {
                    Agent.State = AGENT_STATE_WAITING_TO_SEND;
                } else if (Agent.coinFlipResult == HEADS) {
                    Agent.State = AGENT_STATE_DEFENDING;
                }
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
            if (event.type == BB_EVENT_RESET_BUTTON) {
                AgentInit();
            } else if (event.type == BB_EVENT_REV_RECEIVED) {
                Agent.A = event.param0;

                if (NegotiationVerify(Agent.A, Agent.A_hash)) {
                   // not cheating
                    Agent.coinFlipResult = NegotiateCoinFlip(Agent.A, Agent.B);
                    
                }
            } else {

            }
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
            if (event.type == BB_EVENT_RESET_BUTTON) {
                AgentInit();
            } else if (event.type == BB_EVENT_RES_RECEIVED) {

            }
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
            if (event.type == BB_EVENT_RESET_BUTTON) {
                AgentInit();
            } else if (event.type == BB_EVENT_SHO_RECEIVED) {

            }
            break;

        case(AGENT_STATE_WAITING_TO_SEND):
            /*
             * AGENT_STATE_WAITING_TO_SEND, //5
             * - on MESSAGE sent 
             * - decide new guess on enemy field
             * - go to attacking
             * 
             */
            if (event.type == BB_EVENT_MESSAGE_SENT) {

            }
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
