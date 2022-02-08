/* 
 * File:   LinkedList.c
 * Author: Luca Altaffer taltaffe@ucsc.edu
 */

#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "LinkedList.h"

ListItem *LinkedListNew(char *data) {
    ListItem *node = (struct ListItem*) malloc(sizeof (struct ListItem));
    if (!node) {
        return NULL;
    }
    node->data = data;
    node->nextItem = NULL;
    node->previousItem = NULL;
    return node;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
  ListItem* node = LinkedListNew(data);
    if (node) {
        if (!item) {
            return node;
        } else {
            node->nextItem = item->nextItem;
            node->previousItem = item;
            item->nextItem = node;
            if (node->nextItem) {
                node->nextItem->previousItem = node;
            }
            return node;
        }
    } else {
        return NULL;
    }
}

char *LinkedListRemove(ListItem * item) {
    if (!item) {
        return NULL;
    }
    char* dat = item->data;
    if ((!item->previousItem) || (!item->nextItem)) {
        free(item);
    } else {
        item->nextItem->previousItem = item->previousItem;
        item->previousItem->nextItem = item->nextItem;
        free(item);
    }

    return dat;
}

int LinkedListSize(ListItem * list) {
    int length = 1;
    if (!list) {
        return 0;
    }
    ListItem* curr = malloc(sizeof (ListItem));
    if (curr) {
        curr = LinkedListGetFirst(list);
        while (curr->nextItem) {
            length++;
            curr = curr->nextItem;
        }
    }
    return length;
}

ListItem * LinkedListGetFirst(ListItem * list) {
    if (!list) {
        return NULL;
    }
    ListItem* curr = malloc(sizeof (ListItem));
    if (curr) {
        curr = list;
        while (curr->previousItem) {
            curr = curr->previousItem;
        }
        return curr;
    } else {
        return NULL;
    }
}

ListItem * LinkedListGetLast(ListItem * list) {
    if (!list) {
        return NULL;
    }
    ListItem* curr = malloc(sizeof (ListItem));
    if (curr) {
        curr = list;
        while (curr->nextItem) {
            curr = curr->nextItem;
        }
        return curr;
    } else {
        return NULL;
    }
}

int LinkedListSwapData(ListItem *firstItem, ListItem * secondItem) {
    if (!firstItem || !secondItem) {
        return STANDARD_ERROR;
    } else {
        char * temp = firstItem->data;
        firstItem->data = secondItem->data;
        secondItem->data = temp;
        return SUCCESS;
    }
}

int LinkedListPrint(ListItem * list) {
    if (!list) {
        return STANDARD_ERROR;
    }
    ListItem* curr = malloc(sizeof (ListItem));
    if (curr) {
        curr = LinkedListGetFirst(list);
        while (curr) {
            printf("\n\t%s.", curr->data);
            curr = curr->nextItem;
        }
        free(curr);
        return SUCCESS;
    } else {
        free(curr);
        return STANDARD_ERROR;
    }
}
