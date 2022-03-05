#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Node{
	int value;
	struct Node* next;
};

struct List{
	struct Node* start;
	struct Node* end;
	int size;
};

void printList(struct List base){
  while (base.start != NULL){
    printf(" %d ", base.start->value);
    base.start = base.start->next;
  }
}

void AppendList(struct List* list, int value){
	struct Node* Added = (struct Node*) malloc(sizeof(struct Node));
	Added->value = value;
	Added->next = NULL;
	struct Node* last = list->end;
	list->end = Added;
	list->size++;
	if(list->start == NULL){
		list->start = Added;		
		return;
	}
	last->next = Added;
}

void PushList(struct List* list, int value){
	struct Node* Added = (struct Node*) malloc(sizeof(struct Node));
	Added->value = value;
	Added->next = list->start;
	list->start = Added;
	list->size++;
}

void InsertAfter(struct List* list, int index, int value){
	if(list->size<=index)return;
	list->size++;
	struct Node* Added = (struct Node*) malloc(sizeof(struct Node));
	Added->value = value;
	struct Node* Curr = list->start;
	for(int i = 0; i < index; i++){
		Curr=Curr->next;
	}
	if(Curr->next == NULL){
		list->end=Added;
	}
	Added->next=Curr->next;
	Curr->next = Added;
}

int popFirst(struct List* list){
	struct Node* target = list->start;
	if(list->size==1){
		list->start = NULL;
		list->end = NULL;
	}
	list->start = target->next;
	int value = target->value;
	free(target);
	list->size--;
	return value;
}

int popLast(struct List* list){
	struct Node* target = list->end;
	if(list->size>1){
		struct Node* previous = list->start;
		while (previous->next!=target){
			previous = previous->next;
		}
		list->end = previous;
		previous->next = NULL;
	}
	else{
		list->start = NULL;
		list->end = NULL;
	}
	int value = target->value;
	free(target);
	list->size--;
	return value;
}

int popIndex(struct List* list, int index){
	if(index >= list->size||index<0)return -1;
	if(index == 0)return popFirst(list);
	if(index == list->size-1)return popLast(list);
	struct Node* previous = list->start;
	for(int i = 1; i<=index-1; i++){
		previous = previous->next;
	}
	struct Node* target = previous->next;
	previous->next=target->next;
	int value = target->value;
	free(target);
	list->size--;
	return value;
}

void Reverse(struct Node* previous, struct Node* current){
	if(current->next != NULL){
		Reverse(current, current->next);
	}
	current->next = previous;
}

void ReverseList(struct List* list){
	Reverse(NULL,list->start);
	struct Node* temp = list->start;
	list->start = list->end;
	list->end = temp;
}

int getValue(struct List list, int index){
	struct Node* Curr = list.start;
	for(int i = 0; i<index; i++){
		Curr = Curr->next;
	}
	return Curr->value;
}

int main(){
	struct List list;
	list.start=NULL;
	list.size=0;
	AppendList(&list,1);
	AppendList(&list,2);
	PushList(&list,5);
	InsertAfter(&list,2,7);
	ReverseList(&list);
	printf("%d",popIndex(&list,3));
	printList(list);
	return 0;
}

