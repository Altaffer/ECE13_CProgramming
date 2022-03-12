/*
 *  @file Message.c
 *  @authr Nicholas Kuipers
 *  @date 3/4/2022
 * 
 *  @purpose Definitions for Message.h library
 */

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

typedef enum {
    WAITING_FOR_START = 0,
    RECORDING_PAYLOAD = 1,
    RECORDING_CHECKSUM = 2            
}Parse_Machine;

//the following static variables are used for the Message_Decode() function
static Parse_Machine decode_state = WAITING_FOR_START;               //the state machine
static char decoded_message[MESSAGE_MAX_LEN];                        //stores the decoded message until it is ready to be passed on
static char decoded_checksum[3];                                     //stores the checksum
static uint8_t array_pointer = 0;                   //points to the next open position in decoded_message

//Helper function
void resetValues();

//Helper function

/**
 * Calculates the checksum of the payload parameter and returns it
 * as an 8-bit value
 * 
 * @param payload
 * @return the checksum, an 8-bit value
 */
uint8_t Message_CalculateChecksum(const char* payload)
{
    uint8_t result = 0;          //our result will be an 8-bit value
    const char* temp = payload;
    while(*payload != '\0' && *payload != '*')       //loops until end character '\0' or '*' is reached
    {
        result ^= *payload;      //new character is XOR'd with our result value
        payload++;               //advances to the next position in memory
    }
    payload = temp;
    return result;               //result = value of all characters XOR'd
}
/**
 * This takes in a payload parameter (a message string) and transfers the information
 * to the parameters of a BB_Event struct message_event. 
 * 
 * This first tests the parameters to ensure the checksum_string is of a proper length and 
 * that the checksum of the payload matches the checksum_string parameter. 
 * 
 * If all requirements are met, the information from payload is passed to the BB_Event struct.
 * 
 * @param payload
 * @param checksum_string
 * @param message_event
 * @return STANDARD_ERROR if: checksum_string is not 2 characters long
 *                            checksum of payload does not match checksum_string
 *                            the message does not correspond to a proper message template
 * 
 *         SUCCESS otherwise
 */
int Message_ParseMessage(const char* payload, const char* checksum_string, BB_Event * message_event)
{
    //const variables save the pointer values of our parameters
    const char* temp_payload = payload;
    const char* temp_checksum_string = checksum_string; 
    
    //First, we check our STANDARD_ERROR conditions
    //is the checksum string two characters long?
    int testcheck = strlen(checksum_string);
    if(strlen(checksum_string) != 2)
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    //Does the payload match the checksum?
    /*
     *  Note: A prior (and far simpler) implementation here used payload as the param for Message_CalculateChecksum().
     *  For unexplainable reasons, however, this changed the address of the message_event param
     *  and prevented any further interactions with the struct, despite Message_CalculateChecksum() having
     *  zero interaction with the struct. The following code is a work-around that solves this
     *  problem, but sits as a testament to how much of a time sink troubleshooting this single issue proved to be.
     */
    char data[MESSAGE_MAX_LEN] = {};
    for(int i = 0; i < MESSAGE_MAX_LEN; i++)
    {
        data[i] = *payload++;
        if(*payload == '\0')
            break;
    }
    payload = temp_payload;        //reset payload
    int checksum_payload = Message_CalculateChecksum(data);
    int checksum_param = (int)strtol(checksum_string, NULL, 16);
    if(checksum_payload != checksum_param)
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    /*
     *  Now we want to pass the information off to each parameter of
     *  the BB_Event struct. Structure is as follows:
     *      - type   = type of event specified at beginning
     *      - param0 = X-coordinate for SHO, RES. Negotiation info for other types.
     *      - param1 = Y-coordinate for SHO, RES
     *      - param2 = HIT or MISS for RES
     */
    
    //We can have a min of 2 tokens and up to 4 tokens (from a legal message)
    
    char delimiters[] = {',', '*', '\n'};
    char *type, *field0, *field1, *field2, *errorTest;
    uint16_t assign_val = 0;
    type = strtok(data, delimiters);                  //type always comes first
    field0 = strtok(NULL, delimiters);         //at least one data field
    field1 = strtok(NULL, delimiters);         //can be another value or NULL
    field2 = strtok(NULL, delimiters);         //can be another value or NULL
    errorTest = strtok(NULL, "\n");            //catches any leftover values
    
    //We assign the type to message_event->type, or return an error if type does not match any possible value
    if(strcmp(type,"CHA") == 0)
        message_event->type = BB_EVENT_CHA_RECEIVED;
    else if(strcmp(type,"ACC") == 0)
        message_event->type = BB_EVENT_ACC_RECEIVED;
    else if(strcmp(type,"REV") == 0)
        message_event->type = BB_EVENT_REV_RECEIVED;
    else if(strcmp(type,"SHO") == 0)
        message_event->type = BB_EVENT_SHO_RECEIVED;
    else if(strcmp(type,"RES") == 0)
        message_event->type = BB_EVENT_RES_RECEIVED;
    else
    {
        payload = temp_payload;
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    //Then, we assign each field to its respective parameter in message_event
    if(field0 == NULL || errorTest != NULL)        //if there are any leftover values, returns an error
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    assign_val = strtol(field0, NULL, 16);
    message_event->param0 = assign_val;
    
    if(field1 != NULL)
    {
        assign_val = strtol(field1, NULL, 16);
        message_event->param1 = assign_val;
    }
    else
        message_event->param1 = 0;
    if(field2 != NULL)
    {
        assign_val = strtol(field2, NULL, 16);
        message_event->param2 = assign_val;
    }
    else
        message_event->param2 = 0;
    
    checksum_string = temp_checksum_string;
    payload = temp_payload;
    return SUCCESS;
}

/**
 * Converts information from the Message struct message_to_encode into a singular string, which is
 * passed into the message_string pointer.
 * 
 * Tested -- this will work with either a message_string that uses malloc or an array (both of size MESSAGE_MAX_LEN)
 * 
 * @param message_string
 * @param message_to_encode
 * @return length of message_string as an int
 *         0 if MESSAGE_NONE
 *         -1 if there's an error
 */
int Message_Encode(char *message_string, Message message_to_encode)
{
    MessageType m_type = message_to_encode.type;
    switch(m_type)
    {
        case MESSAGE_CHA:
            sprintf(message_string, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            break;
        case MESSAGE_ACC:
            sprintf(message_string, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            break;
        case MESSAGE_REV:
            sprintf(message_string, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            break;
        case MESSAGE_SHO:
            sprintf(message_string, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            break;
        case MESSAGE_RES:
            sprintf(message_string, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            break;
        case MESSAGE_NONE:
            return 0;
        case MESSAGE_ERROR:
            return -1;
        default:
            return -1;
    }
    if(message_to_encode.type == MESSAGE_NONE)
        return 0;
    return strlen(message_string);
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                          if char_in is the last character of an invalid message,
 *                              then decoded_message should have an ERROR type.
 *                          otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event)
{
    uint8_t ERROR_FLAG = FALSE;      //setting to true activates a function that resets all values to initial values
    
    if(decode_state == WAITING_FOR_START)    //state machine waits here until it receives the start delimiter
    {
        if(char_in == '$')                  //if the function receives the start delimiter
            decode_state = RECORDING_PAYLOAD;           //state machine moves to RECORDING_PAYLOAD
    }
    else if(decode_state == RECORDING_PAYLOAD)    //state machine waits here until it receives an invalid character, the array overflows, or receives a checksum delimiter
    {
        if(char_in == '$' || char_in == '\n' || array_pointer >= MESSAGE_MAX_LEN)    //error conditions
            ERROR_FLAG = TRUE;
        else if(char_in == '*')                  //function received the checksum delimiter
        {
            array_pointer = 0;                   //reset to 0 to be used for checksum array
            decode_state = RECORDING_CHECKSUM;
        }
        else                                     //all other characters are stored in the decoded_message array
            decoded_message[array_pointer++] = char_in;
    }
    else if(decode_state == RECORDING_CHECKSUM)
    {
        //checks the ASCII values ranges for 0-9 and A-F (valid hex values)
        if(((int)char_in >= 48 && (int)char_in <= 57) || ((int)char_in >= 65 && (int)char_in <= 70))
            if(array_pointer >= 2)    //if array exceeds maximum value for checksum, return an error
                ERROR_FLAG = TRUE;
            else
                decoded_checksum[array_pointer++] = char_in;
        else if(char_in == '\n')   //reaches end delimiter
        {
            if(array_pointer != 2)   //checksum value is exactly two digits
                ERROR_FLAG = TRUE;
            else                     //array_pointer == 2. Therefore, we parse our message
            {
                decoded_checksum[++array_pointer] = '\0';
                Message_ParseMessage(decoded_message, decoded_checksum, decoded_message_event);
                resetValues();
            }
        }
        else                      //received an illegal character
            ERROR_FLAG = TRUE;
    }
    
    
    if(ERROR_FLAG == TRUE)       //if function encounters an error, state machine and respective values are reverted to initial
    {
        resetValues();
        decoded_message_event->type = BB_EVENT_ERROR;
        ERROR_FLAG = FALSE;
        return STANDARD_ERROR;
    }
    
    return SUCCESS;
}

//Helper function resets the arrays and array pointer when needed
void resetValues()
{
    for(int i = 0; i < array_pointer; i++)
        decoded_message[i] = '\0';
    array_pointer = 0;
    decode_state = WAITING_FOR_START;
}
