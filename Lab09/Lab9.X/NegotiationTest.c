/*
 * File:   NegotiationTest.c
 * Author: Luca Altaffer taltaffe@ucsc.edu
 *
 * Created on March 10, 2022, 9:46 AM
 */


#include "xc.h"
#include "Negotiation.h"
#include "BOARD.h"
#include <stdio.h>

void main(void)
{

    BOARD_Init();

    printf("Welcome to Negotiation Test!\n\n");

    //test negotiation hash
    printf("Beginning Negotiation Hash test...\n");

    printf("Testing Negotiation Hash with value 3.\nExpected return = 9\n");

    if(NegotiationHash(3) == 9){

        printf("Test passed! Negotiation hash return expected value.\n\n");

    }else{

        printf("Test failed. Negotiation hash did not return the expected output.\n\n");

    }

    printf("Testing Negotiation Hash with value 0.\nExpected return = 0\n");

    if(NegotiationHash(0) == 0){

        printf("Test passed! Negotiation hash return expected value.\n\n");

    }else{

        printf("Test failed. Negotiation hash did not return the expected output.\n\n");

    }

    printf("Testing Negotiation Hash with value 12345.\nExpected return = 43182\n");

    if(NegotiationHash(12345) == 43182){

        printf("Test passed! Negotiation hash return expected value.\n\n");

    }else{

        printf("Test failed. Negotiation hash did not return the expected output.\n\n");

    }


    //test negotiation verify 
    printf("Beginning Negotiation Verify Test...\n");

    printf("Testing Negotiation Verify with secret 3 and commitment 9.\n");
    printf("Expected return = TRUE\n");

    if(NegotiationVerify(0x0003,0x0009) == TRUE){

        printf("Test passed! Negotiation verify returned true.\n\n");

    }else{

        printf("Test failed. Negotiation verify did not return the expected output.\n\n");

    }

    printf("Testing Negotiation Verify with secret 3 and commitment 10.\n");
    printf("Expected return = FALSE\n");

    if(NegotiationVerify(0x0003,0x000A) == FALSE){

        printf("Test passed! Negotiation verify returned true.\n\n");

    }else{

        printf("Test failed. Negotiation verify did not return the expected output.\n\n");

    }


    //test negotiate coin flip
    printf("Beginning Negotiate Coin Flip Test...\n");

    printf("Testing negotiate coin flip with 0x00FF and 0x00FE.\n");
    printf("Expected return = HEADS\n");

    if(NegotiateCoinFlip(0x00FF, 0x00FE) == HEADS){

        printf("Test passed! Negotiate coin flip returned HEADS.\n\n");

    }else{

        printf("Test failed. Negotiate coin flip did not return the expected output.\n\n");

    }

    printf("Testing negotiate coin flip with 0x00FF and 0x00FF.\n");
    printf("Expected return = TAILS\n");

    if(NegotiateCoinFlip(0x00FF, 0x00FF) == TAILS){

        printf("Test passed! Negotiate coin flip returned TAILS.\n\n");

    }else{

        printf("Test failed. Negotiate coin flip did not return the expected output.\n\n");

    }



    while(1);
}

