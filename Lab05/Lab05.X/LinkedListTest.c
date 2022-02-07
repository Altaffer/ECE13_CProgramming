
/* 
 * File:   LinkedListTest.c
 * Author: Luca Altaffer taltaffe@ucsc.edu
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "LinkedList.h"

// 
#define REMOVE_TEST_SIZE 5


ListItem* test_LinkedListNew(ListItem* node, char* str);
ListItem* test_LinkedListCreateAfter(ListItem *node, char *str);
void test_LinkedListPrint(ListItem* node);
void test_LinkedListSize(ListItem* node, int e_size);
void test_LinkedListGetFirst(ListItem* node, ListItem* head);
void test_LinkedListGetLast(ListItem* node, ListItem* tail);
void test_LinkedListSwapData(ListItem* first, ListItem* second, char* str_1, char* str_2);
void test_LinkedListRemove(ListItem* to_remove, char* expected_str);

int main(void) 
{
    BOARD_Init();

    printf("\n\nWelcome to taltaffe's LinkedListTest.c, compiled on %s %s.\n", __DATE__, __TIME__);

    ListItem *new_tptr = malloc(sizeof (ListItem));
    ListItem *CA_tptr = malloc(sizeof (ListItem));
    ListItem *CA_tptr_head = malloc(sizeof (ListItem));
    ListItem *CA_tptr_tail = malloc(sizeof (ListItem));
    ListItem *swap_tptr_1 = malloc(sizeof (ListItem));
    ListItem *swap_tptr_2 = malloc(sizeof (ListItem));
    ListItem *rm_tptr_1 = malloc(sizeof (ListItem));
    ListItem *rm_tptr_2 = malloc(sizeof (ListItem));
    ListItem *rm_tptr_3 = malloc(sizeof (ListItem));
    ListItem *rm_tptr_4 = malloc(sizeof (ListItem));
    ListItem *rm_tptr_5 = malloc(sizeof (ListItem));


    printf("\n\n********************************************************************************");
    printf("\n*********************TESTING FUNCTIONALITY OF LINKED LIST.**********************");

    // Test New
    printf("\n\nTesting LinkedListNew()");
    new_tptr = NULL;
    printf("\nTesting with null string:");
    new_tptr = test_LinkedListNew(new_tptr, "");
    printf("\nTesting with random string:");
    new_tptr = test_LinkedListNew(new_tptr, "This is a test string for LinkedListNew");

    // Test CreateAfter
    printf("\n\nTesting LinkedListCreateAfter()");
    CA_tptr = NULL;
    CA_tptr_head = NULL;
    CA_tptr_tail = NULL;
    printf("\nTesting CreateAfter with null ptr:");
    CA_tptr = test_LinkedListCreateAfter(NULL, "NULL TEST");
    printf("\nCreating a list of 5 items:");
    CA_tptr_head = test_LinkedListCreateAfter(CA_tptr_head, "Steve");
    CA_tptr_tail = CA_tptr_head;
    CA_tptr_tail = test_LinkedListCreateAfter(CA_tptr_tail, "Morgan");
    CA_tptr_tail = test_LinkedListCreateAfter(CA_tptr_tail, "Matt");
    CA_tptr_tail = test_LinkedListCreateAfter(CA_tptr_tail, "Adam");
    CA_tptr = test_LinkedListCreateAfter(CA_tptr_head, "Nick");

    // Test Print
    printf("\n\nTesting LinkedListPrint()");
    printf("\nTest 1: print list given head");
    test_LinkedListPrint(CA_tptr_head);
    printf("\nTest 2: print list given tail");
    test_LinkedListPrint(CA_tptr_tail);
    printf("\nTest 3: print list given random pointer within the list");
    test_LinkedListPrint(CA_tptr);
    printf("\nTest 4: print list given NULL pointer");
    test_LinkedListPrint(NULL);

    // Test GetSize
    printf("\n\nTesting LinkedListSize()");
    printf("\nTest 1: get size of list given null pointer");
    test_LinkedListSize(NULL, 0);
    printf("\nTest 2: get size given head of previously created list of 5 items");
    test_LinkedListSize(CA_tptr_head, 5);
    printf("\nTest 3: get size given tail of previously created list of 5 items");
    test_LinkedListSize(CA_tptr_tail, 5);
    printf("\nTest 4: get size given random pointer to previously created list of 5 items");
    test_LinkedListSize(CA_tptr, 5);
    printf("\nTest 5: get size given list of 1");
    test_LinkedListSize(new_tptr, 1);

    // Test GetFirst 
    printf("\n\nTesting LinkedListGetFirst()");
    printf("\nTest 1: get first given null pointer");
    test_LinkedListGetFirst(NULL, NULL);
    printf("\nTest 2: get first given head of previously created list of 5 items");
    test_LinkedListGetFirst(CA_tptr_head, CA_tptr_head);
    printf("\nTest 3: get first given tail of previously created list of 5 items");
    test_LinkedListGetFirst(CA_tptr_tail, CA_tptr_head);
    printf("\nTest 4: get first given random pointer to previously created list of 5 items");
    test_LinkedListGetFirst(CA_tptr, CA_tptr_head);
    printf("\nTest 5: get first given list of 1");
    test_LinkedListGetFirst(new_tptr, new_tptr);

    // Test GetLast
    printf("\n\nTesting LinkedListGetLast()");
    printf("\nTest 1: get last given null pointer");
    test_LinkedListGetLast(NULL, NULL);
    printf("\nTest 2: get last given head of previously created list of 5 items");
    test_LinkedListGetLast(CA_tptr_head, CA_tptr_tail);
    printf("\nTest 3: get last given tail of previously created list of 5 items");
    test_LinkedListGetLast(CA_tptr_tail, CA_tptr_tail);
    printf("\nTest 4: get last given random pointer to previously created list of 5 items");
    test_LinkedListGetLast(CA_tptr, CA_tptr_tail);
    printf("\nTest 5: get last given list of 1");
    test_LinkedListGetLast(new_tptr, new_tptr);

    // Test Swap
    printf("\n\nTesting LinkedListSwapData()");
    printf("\nTest 1: swap given null pointer");
    swap_tptr_1 = NULL;
    char* swap_str_0 = "Not NULL";
    swap_tptr_2 = LinkedListNew(swap_str_0);
    test_LinkedListSwapData(swap_tptr_1, swap_tptr_2, "", swap_str_0);
    printf("\nTest 2: swap two non null pointers");
    char* swap_str_1 = "From 1 to 2";
    char* swap_str_2 = "From 2 to 1";
    swap_tptr_1 = LinkedListNew(swap_str_1);
    swap_tptr_2 = LinkedListNew(swap_str_2);
    test_LinkedListSwapData(swap_tptr_1, swap_tptr_2, swap_str_1, swap_str_2);

    // Test Remove
    printf("\n\nTesting LinkedListRemove()");
    char* items_array[] = {"Steve", "Nick", "Morgan", "Matt", "Adam", NULL};
    // populate the list with nodes to be removed
    rm_tptr_1 = LinkedListNew(items_array[0]);
    rm_tptr_2 = LinkedListCreateAfter(rm_tptr_1, items_array[1]);
    rm_tptr_3 = LinkedListCreateAfter(rm_tptr_2, items_array[2]);
    rm_tptr_4 = LinkedListCreateAfter(rm_tptr_3, items_array[3]);
    rm_tptr_5 = LinkedListCreateAfter(rm_tptr_4, items_array[4]);

    printf("\nTest 1: removing Nick from the middle of the list");
    test_LinkedListRemove(rm_tptr_2, items_array[1]);
    printf("\nTest 2: removing Matt from the middle of the list");
    test_LinkedListRemove(rm_tptr_4, items_array[3]);
    printf("\nTest 3: removing Steve from the head of the list");
    test_LinkedListRemove(rm_tptr_1, items_array[0]);
    printf("\nTest 4: removing Adam from the tail of the list");
    test_LinkedListRemove(rm_tptr_5, items_array[4]);
    printf("\nTest 5: removing Morgan, the only item remaining in the list");
    test_LinkedListRemove(rm_tptr_3, items_array[2]);

    printf("\n*********************************TESTING COMPLETE*********************************");
    
    BOARD_End();
    while (1);
    return 0;
}

ListItem* test_LinkedListNew(ListItem* node, char* str) 
{
    node = LinkedListNew(str);
    if (!node) {
        printf("\nError!\n\tLinkedListNew returned NULL.");
        return NULL;
    }
    if (strcmp(node->data, str)) {
        printf("\nError!\n\tNode's data is incorrect.");
        printf("\n\tExpected: %s, Got: %s", str, node->data);
        return NULL;
    }
    if (node->previousItem) {
        printf("\nError!\n\tNode's previous ptr is not NULL.");
        return NULL;
    }
    if (node->nextItem) {
        printf("\nError!\n\tNode's next ptr is not NULL.");
        return NULL;
    }
    if (!strcmp(node->data, str) && !node->previousItem && !node->nextItem) {
        printf("\nPASSED!\n\tNode created with correct data and null pointers.");
        printf("\n\tExpected: %s\n\tGot     : %s", str, node->data);
        return node;
    }
    
    return NULL;
}

ListItem* test_LinkedListCreateAfter(ListItem *node, char *str) 
{
    ListItem* next = malloc(sizeof (ListItem));
    ListItem* prev = malloc(sizeof (ListItem));
    if (!next || !prev) {
        printf("\nError while allocating memory for next and prev.");
        return NULL;
    }
    if (node) {
        next = node->nextItem;
        prev = node->previousItem;
    } else {
        next = NULL;
        prev = NULL;
    }
    int error = 0;
    ListItem* test = malloc(sizeof (ListItem));
    if (test) {
        test = LinkedListCreateAfter(node, str);
        if (test) {
            if (test->nextItem != next) {
                printf("\nError!\n\ttest->next is incorrect");
                error++;
            }
            if (test->previousItem != node) {
                printf("\nError!\n\ttest->prev is incorrect");
                error++;
            }
            if (node && node->nextItem != test) {
                printf("\nError!\n\ttest node is not the original node's next item.");
                error++;
            }
            if (test->nextItem && test->nextItem->previousItem != test) {
                printf("\nError!\n\ttest->next->prev is not pointing back to test.");
                error++;
            }
            if (strcmp(test->data, str)) {
                printf("\nError!\n\tstrings do not match.");
                printf("\n\tExpected: %s\n\tGot     : %s", str, test->data);
                error++;
            }
            if (!error) {
                printf("\nPASSED!\n\tNew node successfully created after the given node.");
                printf("\n\tExpected: %s\n\tGot     : %s", str, test->data);
                return test;
            }
        } else {
            printf("\nError!\n\tLinkedListCreateAfter failed to create the pointer.");
        }
    } else {
        printf("\nError!\n\tTest pointer allocation failed.");
    }
    free(prev);
    free(next);
    free(test);
    return NULL;
}

void test_LinkedListPrint(ListItem* node)
{
    int print_result = LinkedListPrint(node);

    if ((node && print_result == SUCCESS) || (!node && print_result == STANDARD_ERROR)) {
        printf("\nPASSED!");
    } else {
        printf("\nPrint Failed");
    }
}

void test_LinkedListSize(ListItem* node, int e_size)
{
    int size_result = LinkedListSize(node);
    if (size_result == e_size) {
        printf("\nPASSED!");
    } else {
        printf("\nError!\n\tExpected: %d\n\tGot     : %d", e_size, size_result);
    }
}

void test_LinkedListGetFirst(ListItem* node, ListItem* head) {
    if (LinkedListGetFirst(node) == head) {
        printf("\nPASSED!");
    } else {
        printf("\nError!\n\tGetFirst returned the wrong pointer.");
    }
}
 
void test_LinkedListGetLast(ListItem* node, ListItem* tail)
{
    if (LinkedListGetLast(node) == tail) {
        printf("\nPASSED!");
    } else {
        printf("\nError!\n\tGetLast returned the wrong pointer.");
    }
}

void test_LinkedListSwapData(ListItem* first, ListItem* second, char* str_1, char* str_2)
{
    int swap_restult = LinkedListSwapData(first, second);

    if (swap_restult == STANDARD_ERROR) {
        if (first && second) {
            printf("\nError!\n\tFunction returned error but neither pointer is NULL");
        } else if (!first || !second) {
            printf("\nPASSED!");
        }
    }

    if (swap_restult == SUCCESS) {
        if (!strcmp(first->data, str_2) && !strcmp(second->data, str_1)) {
            printf("\nPASSED!");
        } else {
            printf("\nError!\n\tFunction returned success but the swap was incorrect.");
        }
    }

}

void test_LinkedListRemove(ListItem* to_remove, char* expected_str) 
{
    printf("\nCurrent list size: %d", LinkedListSize(to_remove));
    printf("\nCurrent list:");
    LinkedListPrint(to_remove);
    printf("\nRemoving: %s", to_remove->data);
    char* remove_result = LinkedListRemove(to_remove);
    if (!strcmp(remove_result, expected_str)) {
        printf("\nPASSED!");
    } else {
        printf("\nError!\n\tItem removed does not match expected data.");
    }
}