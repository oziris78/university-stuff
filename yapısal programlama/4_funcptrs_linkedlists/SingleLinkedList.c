/*
	2022, Oguzhan Topaloglu
*/


#include <stdlib.h>
#include <stdio.h>


typedef struct Data {
	int age;
	float num;
	char c;
} DATA;


typedef struct Node {
	DATA* data;
	struct Node* next;
} NODE;


// funcs used in main
void add(NODE** head, int age, char c, float f);
void sort(NODE** head, int(*compare)(DATA*,DATA*));
void printNode(NODE* ref);
void printList(NODE* head, const char* text);
void deleteNode(NODE** head, NODE* goner);
NODE* find(NODE** head, int age);

// helper functions
NODE* newNode(int age, char c, float f);
int compareWithAge(DATA* d1, DATA* d2);
int compareWithName(DATA* d1, DATA* d2);
void swap(NODE* n1, NODE* n2);



int main() {
	NODE* head = NULL;
	
	add(&head, 20, 'c', 3.4); add(&head, 40, 'a', 9.7);
	add(&head, 30, 'e', 2.5); add(&head, 10, 'h', 6.8);
	printList(head, "The list is");
	
	sort(&head, compareWithAge);
	printList(head, "Sorted by age");
	sort(&head, compareWithName);
	printList(head, "Sorted by char");
	
	NODE* toDelete = find(&head, 20);
	printf("This will be deleted:\n");
	printNode(toDelete);
	deleteNode(&head, toDelete);
	printList(head, "After deletion");
	
	if(find(&head, 5000) == NULL) // doesnt exist
		printf("age 5000 record doesnt exist\n");
	
	return 0;
}



///////////////////////////////////////
///////////////  FUNCS  ///////////////
///////////////////////////////////////


void add(NODE** head, int age, char c, float f){
	NODE* n = newNode(age, c, f);
	if(*head == NULL){
		*head = n;
	}
	else{
		NODE* ref = *head;
		while(ref->next != NULL){
			ref = ref->next;
		}
		ref->next = n;
	}
}


void sort(NODE** head, int(*compare)(DATA*,DATA*)){
	NODE* ref;
	int sorted = 0;
	while(sorted == 0){
		sorted = 1;
		ref = *head;
		while(ref->next != NULL){
			if(compare(ref->data, ref->next->data) > 0){
				sorted = 0;
				swap(ref, ref->next);				
			}
			ref = ref->next;
		}
	}
}


void printNode(NODE* ref){
	printf("-- age: %d, char: %c, num: %.2f\n", 
		ref->data->age, ref->data->c, ref->data->num);
}


void printList(NODE* head, const char* text){
	NODE* ref = head;
	printf("%s: \n", text);
	while(ref != NULL){
		printNode(ref);
		ref = ref->next;
	}
	printf("-------------\n");
}


void deleteNode(NODE** head, NODE* goner){
	NODE* toDelete = find(head, goner->data->age);
	if(toDelete != NULL){
		NODE* prev = *head;
		while(prev->next != toDelete){
			prev = prev->next;
		}
		prev->next = toDelete->next;
		free(toDelete);
	}
}


NODE* find(NODE** head, int age){
	NODE* ref = *head;
	while(ref != NULL){
		if(ref->data->age == age){
			return ref;
		}
		ref = ref->next;
	}
	return NULL;
}



///////////////////////////////////////
//////////////  HELPERS  //////////////
///////////////////////////////////////


NODE* newNode(int age, char c, float f){
	DATA* data = (DATA*) malloc(sizeof(DATA));
	data->age = age;
	data->c = c;
	data->num = f;
	NODE* node = (NODE*) malloc(sizeof(NODE));
	node->data = data;
	node->next = NULL;
	return node;
}


int compareWithAge(DATA* d1, DATA* d2){
	return (d1->age - d2->age > 0) ? 1 : 0;
}


int compareWithName(DATA* d1, DATA* d2){
	return (d1->c - d2->c > 0) ? 1 : 0;
}


void swap(NODE* n1, NODE* n2){
	DATA* temp = n1->data;
	n1->data = n2->data;
	n2->data = temp; 
}



