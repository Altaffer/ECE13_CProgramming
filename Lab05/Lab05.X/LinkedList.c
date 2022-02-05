#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "LinkedList.h"

ListItem *LinkedListNew(char *data) {
    struct ListItem *newItem = (struct ListItem*) malloc(sizeof (struct ListItem));
    if (newItem == NULL) {
        return NULL;
    }
    newItem->data = data;
    newItem->nextItem = NULL;
    newItem->previousItem = NULL;
    return newItem;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    LinkedListNew(&data);

    struct ListItem *newItem = LinkedListNew(&data);
    if (newItem == NULL) {
        return NULL;
    }
    if (item == NULL) {
        return newItem;
    }
    if (item->nextItem == NULL) {
        item->nextItem = newItem;
        newItem->previousItem = item;
    } else {
        item->nextItem->previousItem = newItem;
        item->nextItem = newItem;
        newItem->previousItem = item;
    }
    return newItem;
}

char *LinkedListRemove(ListItem * item) {
    if (item == NULL) {
        return NULL;
    }
    char* dat = item->data;
    if ((item->previousItem == NULL) || (item->nextItem == NULL)) {
        free(item);
    } else {
        item->nextItem->previousItem = item->previousItem;
        item->previousItem->nextItem = item->nextItem;
        free(item);
    }

    return dat;
}

int LinkedListSize(ListItem * list) {
    if (list = NULL){
        return NULL;
    }
    struct ListItem *head = LinkedListGetFirst(list);
    int count = 0;
    while (head != NULL){
        count++;
        head = head->nextItem;
    }
    return count;
}

ListItem * LinkedListGetFirst(ListItem * list) {
    if (list == NULL) {
        return NULL;
    }
    if (list->previousItem == NULL){
        return list;
    }else{
        while(list->previousItem != NULL){
            list = list->previousItem;
        }
    }
    return list;
}

ListItem * LinkedListGetLast(ListItem * list) {
    if (list == NULL){
        return NULL;
    }
    if (list->nextItem = NULL){
        return list;
    }else{
        while(list->nextItem != NULL){
            list = list->nextItem;
        }
    }
    return list;
}   

int LinkedListSwapData(ListItem *firstItem, ListItem * secondItem) {

}

int LinkedListPrint(ListItem * list) {
    if(list == NULL){
        return STANDARD_ERROR;
    }
    struct ListItem *head = LinkedListGetFirst(list);
    int count = 0;
    while (head != NULL){
        count++;
        printf("%d, ", head);
        head = head->nextItem;
    }
    return SUCCESS;

}
