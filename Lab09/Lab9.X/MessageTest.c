/*
 * File:   MessageTest.c
 * Author: Luca Altaffer taltaffe@ucsc.edu
 *
 * Created on March 11, 2022, 3:09 PM
 */

#include <stdio.h>
#include <string.h>
#include "xc.h"
#include "BOARD.h"
#include "Message.h"
#include "BattleBoats.h"

int main(void)
{
    BOARD_Init();
    int score = 0;
    printf("\n****Testing CalculateChecksum()****\n");
    const char *testPayload1 = "CHA,43182";
    printf("Payload: %s || Checksum: 5A\n", testPayload1);
    int cSum1 = Message_CalculateChecksum(testPayload1);
    printf("Calculated Checksum: %X\n\n", cSum1);
    if (cSum1 = 0x5A){
        score++;
    }
    else{
        printf("CalculateChecksum() test 1 failed...\n");
    }
    
    const char *testPayload2 = "RES,3,3,1";
    printf("Payload: %s || Checksum: 59\n", testPayload2);
    int cSum2 = Message_CalculateChecksum(testPayload2);
    printf("Calculated Checksum: %X\n\n", cSum2);
    if (cSum2 = 0x59){
        score++;
    }
    else{
        printf("CalculateChecksum() test 2 failed...\n");
    }
    
    //////////////////////////////////////////////////
    
    printf("\n****Testing ParseMessage()****\n");
    BB_Event pTestEvent;
     
    const char *testCSum1 = "5A";
    Message_ParseMessage(testPayload1, testCSum1, &pTestEvent);
    printf ("Expected BB_Event Fields:\n    Type: 3 | Param0: 43182\n");
    printf("Resultant BB_Event Fields:\n    Type: %d | Param0: %u\n\n", pTestEvent.type, pTestEvent.param0);
    if ((pTestEvent.type == 3) && (pTestEvent.param0 == 43182)){
        score++;
    }
    else{
        printf("ParseMessage() test 1 failed...\n");
    }
    
    const char *testCSum2 = "59";
    Message_ParseMessage(testPayload2, testCSum2, &pTestEvent);
    printf ("Expected BB_Event Fields:\n    Type: 7 | Param0: 3 | Param1: 3 | Param2: 1\n");
    printf("Resultant BB_Event Fields:\n    Type: %d | Param0: %u | Param1: %u | Param2: %u\n\n",
            pTestEvent.type, pTestEvent.param0, pTestEvent.param1, pTestEvent.param2);
    if ((pTestEvent.type == 7) && (pTestEvent.param0 == 3) && (pTestEvent.param1 == 3) && (pTestEvent.param2 == 1)){
        score++;
    }
    else{
        printf("ParseMessage() test 2 failed...\n");
    }
    //////////////////////////////////////////////////
    
    printf("\n****Testing Encode()****\n");
    Message testMessage;
    char messageString1[MESSAGE_MAX_LEN];
    testMessage.type = MESSAGE_CHA;
    testMessage.param0 = 44082;
    Message_Encode(messageString1, testMessage);
    printf("Expected Message String: $CHA,44082*5C\n");
    printf("First Message String:    %s\n", messageString1);
    if (!strcmp(messageString1, "$CHA,44082*5C\n")){
        score++;
    }
    else{
        printf("MessageEncode() test 1 failed...\n");
    }
    
    char messageString2[MESSAGE_MAX_LEN];
    testMessage.type = MESSAGE_RES;
    testMessage.param0 = 4;
    testMessage.param1 = 8;
    testMessage.param2 = 0;
    Message_Encode(messageString2, testMessage);
    printf("Expected Message String: $RES,4,8,0*54\n");
    printf("Second Message String:   %s\n\n", messageString2);
    if (!strcmp(messageString2, "$RES,4,8,0*54\n")){
        score++;
    }
    else{
        printf("MessageEncode() test 2 failed...\n");
    }
    
    //////////////////////////////////////////////////
    
    printf("\n****Testing Decode()****\n");
    char *testEncoded1 = "$CHA,43182*5A\n";
    char *tempPtr = testEncoded1;
    BB_Event dTestEvent;
    dTestEvent.type = BB_EVENT_NO_EVENT;
    while (dTestEvent.type == BB_EVENT_NO_EVENT){
        Message_Decode(*tempPtr, &dTestEvent);
        tempPtr++;
    }
    tempPtr = NULL;
    
    printf("Expected BB_Event:  Type: 3 | Param0: 43182\n");
    printf("Resultant BB_Event: Type: %u | Param0: %u\n\n", dTestEvent.type, dTestEvent.param0);
    if ((dTestEvent.type == 3) && (dTestEvent.param0 == 43182)){
        score++;
    }
    else{
        printf("MessageDecode() test 1 failed...\n");
    }
    
    char *testEncoded2 = "$REV,12345*5C\n";
    tempPtr = testEncoded2;
    dTestEvent.type = BB_EVENT_NO_EVENT;
    
    while (dTestEvent.type == BB_EVENT_NO_EVENT){
        Message_Decode(*tempPtr, &dTestEvent);
        tempPtr++;
    }
    tempPtr = NULL;
    
    printf("Expected BB_Event:  Type: 5 | Param0: 12345\n");
    printf("Resultant BB_Event: Type: %u | Param0: %u\n\n", dTestEvent.type, 
            dTestEvent.param0);
    if ((dTestEvent.type == 5) && (dTestEvent.param0 == 12345)){
        score++;
    }
    else{
        printf("MessageDecode() test 2 failed...\n");
    }
    
    printf("---------------------------------\n");
    printf("Total Tests Passed: %d/8\n", score);

    while(1);
}
