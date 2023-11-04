/*
	2022, Oguzhan Topaloglu, C19052025
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct Flight {
	int code, runwayNumber, info;
	char from[20], time[20], to[20];
	struct Flight* next;
} Flight;


FILE* openFile(const char*, const char*);
Flight* parseFile(const char*);
void saveRunwayFiles(Flight**);
void swapStr(char*, char*);
void swapInt(int*, int*);
void printList(Flight**);
void sortList(Flight**);



int main() {
	const char* pathInput = "input.txt";
	Flight* head = parseFile(pathInput);
	sortList(&head);
	printList(&head);
	saveRunwayFiles(&head);
	
	return 0;
}



Flight* parseFile(const char* path){
	FILE* file = openFile(path, "r");
	
	Flight* head = NULL;
	char line[1024];
	char* p;
	while(fgets(line, 1024, file)){
		Flight* flight = (Flight*) malloc(sizeof(Flight));
		flight->next = NULL;
		p = strtok(line, " ");
		int i = 0;
		while(p != NULL){
	    	if(i == 0) flight->code = atoi(p);
	    	if(i == 1) strcpy(flight->from, p);
	    	if(i == 2) strcpy(flight->to, p);
	    	if(i == 3) strcpy(flight->time, p);
	    	if(i == 4) flight->info = atoi(p);
	    	if(i == 5) flight->runwayNumber = atoi(p);
			
			i++;
	    	p = strtok(NULL, " ");
		}
		
		if(head == NULL) {
			head = flight;
		}
		else {
			Flight* last;
			for(last = head; last->next != NULL; last = last->next);
			last->next = flight;
		}
	}
    
	fclose(file);
	return head;
}


void sortList(Flight** flights){
	Flight* ref;
	int sorted = 0;
	while(!sorted){
		sorted = 1;
		ref = *flights;
		while(ref->next != NULL){
			if(ref->code > ref->next->code){
				sorted = 0;
				
				swapInt(&ref->code, &ref->next->code);
				swapInt(&ref->runwayNumber, &ref->next->runwayNumber);
				swapInt(&ref->info, &ref->next->info);
				swapStr(&ref->from, &ref->next->from);
				swapStr(&ref->to, &ref->next->to);
				swapStr(&ref->time, &ref->next->time);
			}
			ref = ref->next;			
		}
	}
}


void printList(Flight** flights){
	Flight* t = *flights;
	while(t != NULL){
		printf("%d %s %s %s %d %d\n", t->code, t->from, t->to, t->time, t->info, t->runwayNumber);
		t = t->next;
	}
}


void saveRunwayFiles(Flight** flights){
	Flight* temp = *flights;
	FILE* file;
	int doneNumbers[2000] = {0}; // count sort gibimsi
	while(temp != NULL){
		if(doneNumbers[temp->runwayNumber] == 0){
			doneNumbers[temp->runwayNumber] = 1;
			// open file with temp->code
			char fileName[20];
			sprintf(fileName, "pist%d.txt", temp->runwayNumber);
			file = openFile(fileName, "a+");
			
			// print other ones
			Flight* t = temp;
			while(t != NULL){
				if(t->runwayNumber == temp->runwayNumber){
					fprintf(file, "%d %s %s %s %d %d\n", t->code, t->from, t->to, t->time, t->info, t->runwayNumber);
				}
				t = t->next;
			}
			
			fclose(file);
		}
		
		temp = temp->next;
	}
}


FILE* openFile(const char* path, const char* mode){
	FILE* file = fopen(path, mode);
	if(file == NULL){
		printf("File (%s, %s) couldn't be opened\n", path, mode);
		exit(1);
	}
	return file;			
}


void swapInt(int* a, int* b){
	int x = *a;  *a = *b;  *b = x;
}


void swapStr(char* a, char* b){
	char str[20]; strcpy(str, a); strcpy(a, b); strcpy(b, str);
}




