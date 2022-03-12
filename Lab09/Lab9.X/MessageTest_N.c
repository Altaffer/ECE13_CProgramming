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

void testFunction(BB_Event * Battle_Event)
{
    BB_EventType event = Battle_Event->type;
    if(event == BB_EVENT_NO_EVENT)
    {
        Battle_Event->type = BB_EVENT_SHO_RECEIVED;
    }
}

int main()
{
    int yes = 0;
    BB_Event Battle_Event;
    yes = yes + 1;
    const char* testSite = "ACC,2";
    char test[] = "123";
    printf("ACC = %i\n", atoi(test));
    yes = Message_CalculateChecksum(testSite);
    printf("Result of Checksum: %i\n", yes);
    yes = Message_ParseMessage("ACC,4,2,6", "5D", &Battle_Event);
    
    //char *message_string = malloc(MESSAGE_MAX_LEN);
    char message_string[MESSAGE_MAX_LEN] = {};
    Message message_to_encode;
    message_to_encode.type = MESSAGE_SHO;
    message_to_encode.param0 = 4;
    message_to_encode.param1 = 2;
    yes = Message_Encode(message_string, message_to_encode);
    
    char testArray[MESSAGE_MAX_LEN] = "$SHO,3,5*52\n";
    for(int i = 0; i < strlen(testArray); i++)
    {
        yes = Message_Decode(testArray[i], &Battle_Event);
    }
    printf("Battle Event: %d,%d\n", Battle_Event.param0, Battle_Event.param1);
    
    return 0;
}

