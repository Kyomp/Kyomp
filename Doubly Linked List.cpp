#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct Node{
	int value;
	struct Node* next;
	struct Node* previous;
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
		Added->previous = NULL;
		list->start = Added;
		
		return;
	}
	last->next = Added;
	Added->previous = last;
	
}

void PushList(struct List* list, int value){
	struct Node* Added = (struct Node*) malloc(sizeof(struct Node));
	Added->value = value;
	Added->previous = NULL;
	struct Node* After = list->start;
	After->previous = Added;
	Added->next = After;
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
	Added->previous=Curr;
	if(Curr->next == NULL){
		list->end=Added;
		Added->next=NULL;
		Curr->next = Added;
		return;
	}
	Added->next=Curr->next;
	Curr->next->previous=Added;
	Curr->next = Added;
}

int popFirst(struct List* list){
	struct Node* target = list->start;
	list->start = target->next;
	target->next->previous = NULL;
	int value = target->value;
	free(target);
	list->size--;
	return value;
}

int popLast(struct List* list){
	struct Node* target = list->end;
	list->end=target->previous;
	target->previous->next=NULL;
	int value = target->value;
	free(target);
	list->size--;
	return value;
}

int popMiddle(struct List* list, int index){
	if(index >= list->size||index<0)return -1;
	if(index == 0)return popFirst(list);
	if(index == list->size-1)return popLast(list);
	struct Node* target = list->start;
	for(int i = 1; i<=index; i++){
		target = target->next;
	}
	struct Node* previous = target->previous;
	struct Node* After = target->next;
	previous->next=target->next;
	After->previous=target->previous;
	int value = target->value;
	free(target);
	list->size--;
	return value;
}

void Reverse(struct Node* node){
	if(node->next!=NULL){
		Reverse(node->next);
	}
	struct Node* temp;
	temp = node->next;
	node->next = node->previous;
	node->previous = temp;
}

void ReverseList(struct List* list){
	Reverse(list->start);
	struct Node* temp =  list->end;
	list->end = list->start;
	list->start = temp;
}

int getValueStart(struct Node* node, int index){
	struct Node* Curr = node;
	for(int i = 0; i<index; i++){
		Curr = Curr->next;
	}
	return Curr->value;
}

int getValueEnd(struct Node* node, int index){
	struct Node* Curr = node;
	for(int i = 0; i<index-1; i++){
		Curr = Curr->previous;
	}
	return Curr->value;
}

int getValue(struct List list, int index){
	if(abs(index)>=list.size)return NULL;
	if(index<0){
		return getValueEnd(list.end,-index);
	}
	return getValueStart(list.start,index);
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
	printf("%d",getValue(list,-1));
	ReverseList(&list);
	printList(list);
	
	return 0;
}
