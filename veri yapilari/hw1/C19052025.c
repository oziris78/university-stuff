
/*
 * Veri Yapilari dersi - Odev #1
 * Oguzhan Topaloglu, C19052025
 * 17.03.2024
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define T 5
#define L 4


typedef struct node {
    char* address;
    int counter;
    struct node* next;
    struct node* prev;
} Node;



// Functions called in main()
void appendToList(Node**, int*, char*);
void printList(Node**);
void clearList(Node**, int*);
void readFromInputTxt(Node**, int*);


// Helper functions to make code more readable
Node* findNode(Node**, char*);
void setAsNewHead(Node**, Node*);
void deleteLastNode(Node**, int*);


/**
 * @brief replaces the doubly linked list's head with the given node
 * @param headPtr doubly linked list's head pointer
 * @param newHead doubly linked list's NEW "to-be-head" pointer
 */
void setAsNewHead(Node** headPtr, Node* newHead) {
    if(*headPtr == NULL || newHead == NULL) return; // return if invalid input
    if(*headPtr == newHead) return; // return if "newHead" is already the head

    Node* head = *headPtr;

    if (newHead->prev != NULL) {
        newHead->prev->next = newHead->next;
    }
    if (newHead->next != NULL) {
        newHead->next->prev = newHead->prev;
    }

    newHead->prev = NULL;
    newHead->next = head;
    if (head != NULL) {
        head->prev = newHead;
    }
    *headPtr = newHead;
}


/**
 * @brief deletes the last node in the doubly linked list
 * @param headPtr doubly linked list's head pointer
 * @param sizePtr doubly linked list's size integer pointer
 */
void deleteLastNode(Node** headPtr, int* sizePtr) {
    Node* head = *headPtr;
    Node* last = head;
    while (last->next != NULL) {
        last = last->next;
    }
    // list: A B C   =>  set B's next as null
    last->prev->next = NULL;

    free(last);
    *sizePtr -= 1;
}


/**
 * @brief processes an address request according to the rules that were given in the homework PDF
 * @param headPtr doubly linked list's head pointer
 * @param sizePtr doubly linked list's size integer pointer
 * @param address requested address like "ABC", "A", "B" etc.
 */
void appendToList(Node** headPtr, int* sizePtr, char* address) {
    // If list is empty, literally just add it and return
    if(*headPtr == NULL) {
        Node* newNode = (Node*) malloc(sizeof(Node) * 1);
        newNode->address = (char*) malloc(strlen(address) + 1);
        strcpy(newNode->address, address);
        newNode->counter = 1;
        newNode->next = NULL;
        newNode->prev = NULL;
        *headPtr = newNode;
        *sizePtr = 1;
        printList(headPtr);
        return;
    }

    Node* existingNode = findNode(headPtr, address);

    // If node already exists:
    if(existingNode != NULL) {
        existingNode->counter += 1;
        if(existingNode->counter > T) {
            setAsNewHead(headPtr, existingNode);
        }
    }
    // If node is new:
    else {
        // When a new node comes, it will always be the new head so;
        //     its prev is always going to be NULL
        //     its next is always going the be the previous head (if it exists)
        
        Node* node = (Node*) malloc(sizeof(Node) * 1);
        node->address = (char*) malloc(strlen(address) + 1);
        strcpy(node->address, address);
        node->counter = 1;
        node->next = *headPtr;
        node->prev = NULL;

        setAsNewHead(headPtr, node);
        *sizePtr += 1;

        if(*sizePtr > L) {
            deleteLastNode(headPtr, sizePtr);
        }
    }
    
    printList(headPtr);
}


/**
 * @brief prints the doubly linked list in the terminal
 * @param headPtr doubly linked list's head pointer
 */
void printList(Node** headPtr) {
    if(*headPtr == NULL) {
        printf("Current list: <empty>\n");
        return;
    }

    Node* current = *headPtr;
    printf("Current list: ");
    while (current != NULL) {
        printf("%s (%d)", current->address, current->counter);

        if(current->next != NULL) printf(" => ");
        else printf("\n");
        
        current = current->next;
    }
}


/**
 * @brief frees/clears the list and sets its size to 0
 * @param headPtr doubly linked list's head pointer
 * @param sizePtr doubly linked list's size integer pointer
 */
void clearList(Node** headPtr, int* sizePtr) {
    if(*headPtr == NULL || *sizePtr == 0) return;

    Node* current = *headPtr;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current->address);
        free(current);
        current = next;
    }

    *headPtr = NULL;
    *sizePtr = 0;
    printf("List was succesfully deleted.\n");
}


/**
 * @brief appends all the input.txt requests/elements to the list 
 * @param headPtr doubly linked list's head pointer
 * @param sizePtr doubly linked list's size integer pointer
 */
void readFromInputTxt(Node** headPtr, int* sizePtr) {
    FILE* file = fopen("input.txt", "r");
    if (file == NULL) exit(-1); // File couldn't be opened

    char line[1024];
    char* token;
    int i = 1;
    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, " ");
        while (token != NULL) {
            // remove trailing newline if it exists
            token[strcspn(token, "\n")] = 0;

            // append the address (token) into the linkedlist
            appendToList(headPtr, sizePtr, token);

            // go to the next iteration
            token = strtok(NULL, " ");
        }
    }

    fclose(file);
}


/**
 * @brief Find and returns a node from the doubly linked list
 * @param headPtr doubly linked list's head pointer
 * @param address any string to search
 * @return Node* the pointer to the node that matches the given address or NULL if no match was found
 */
Node* findNode(Node** headPtr, char* address) {
    Node* current = *headPtr;
    while (current != NULL) {
        if(strcmp(current->address, address) == 0) {
            return current; // we have a match
        }
        current = current->next;
    }

    return NULL; // no match was found
}


int main() {
    int keepGoing = 1;
    int selected;
    
    // linkedlist variables
    Node* head = NULL;
    int currentCacheSize = 0;

    while (keepGoing) {
        printf("\nChoose option:\n");
        printf("1- Enter from file\n");
        printf("2- Enter manually\n");
        printf("3- View list\n");
        printf("4- Clear list\n");
        printf("5- Quit\n");
        printf("Selected: ");
        
        scanf("%d", &selected);
        fflush(stdin);

        switch(selected) {
            case 1:
                readFromInputTxt(&head, &currentCacheSize);
                break;
            case 2:
                printf("Enter address as a character: ");
                char inputAddress[64];
                scanf("%s", inputAddress);
                fflush(stdin);
                appendToList(&head, &currentCacheSize, inputAddress);
                break;
            case 3:
                printList(&head);
                break;
            case 4:
                clearList(&head, &currentCacheSize);
                break;
            case 5:
                keepGoing = 0;
                break;
            default:
                printf("Invalid selection, please try again.\n");
                break;
        }
    }

    return 0;
}

