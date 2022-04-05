#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
struct Node{
	int value;
	struct Node* left;
	struct Node* right;
	struct Node* parent;
};
struct Tree{
	struct Node* root;
	int length;
};
struct Tree* initialize(){
	struct Tree* target = (struct Tree*) malloc(sizeof(struct Tree));
	target->root = NULL;
	target->length = 0;
	return target;
}
struct Node* createNode(int value, struct Node* parent){
	struct Node* newNode=(struct Node*) malloc(sizeof(struct Node));
	newNode->left = newNode->right = NULL;
	newNode->value = value;
	newNode->parent = parent;
	return newNode;
}
void addNode(struct Tree* tree,int value){
	if(tree->root == NULL){
		struct Node* newNode = createNode(value,NULL);
		tree->root = newNode;
		return;
	}
	struct Node* previous;
	struct Node* curr = tree->root;
	bool isLeft;
	do{
		previous = curr;
		if(value<previous->value){
			curr = previous->left;
			isLeft = true;
		}
		else if(value>previous->value){
			curr = previous->right;
			isLeft = false;
		}
		else return;
	}while(curr!=NULL);
	tree->length+=1;
	if(isLeft){
		previous->left = createNode(value,previous);
		return;
	}
	previous->right = createNode(value,previous);
	return;
}
void insert(struct Tree* tree, int size, ...){
	va_list ptr;
	va_start(ptr,size);
	for(int i = 0; i<size; i++){
		addNode(tree, va_arg(ptr,int));
	}
	va_end(ptr);
}
struct Node* search(struct Tree* tree,int value){
	struct Node* curr = tree->root;
	while(curr!=NULL){
		if(value>curr->value)curr=curr->right;
		else if(value<curr->value)curr = curr->left;
		else break;
	}
	return curr;
}
int findSuccessor(struct Node* curr){
	struct Node* succ = curr->right;
	if(succ==NULL)return -1;
	while(succ->left!=NULL)succ=succ->left;
	return succ->value;
}
int findPredecessor(struct Node* curr){
	struct Node* pred = curr->left;
	if(pred == NULL)return -1;
	while(pred->right!=NULL)pred=pred->right;
	return pred->value;
}
void deleteVal(struct Tree* tree,int value){
	struct Node* curr = search(tree, value);
	if(curr==NULL)return;
	if(curr->left==NULL&&curr->right==NULL){
		if(curr->parent->left==curr)curr->parent->left=NULL;
		else curr->parent->right=NULL;
		free(curr);
		tree->length-=1;
		return;
	}
	int replacementValue = findPredecessor(curr);
	if(replacementValue==-1)replacementValue = findSuccessor(curr);
	deleteVal(tree,replacementValue);
	curr->value = replacementValue;
	return;
}
void deleteValVar(struct Tree* tree, int size, ...){
	va_list ptr;
	va_start(ptr,size);
	for(int i = 0; i<size; i++){
		deleteVal(tree,va_arg(ptr,int));
	}
	va_end(ptr);
}
void find( struct Tree* tree,int value){
	if(search(tree,value)){
		printf("%d is in the tree\n",value);
		return;
	}
	printf("%d is NOT in the tree\n",value);
}
void popAll(struct Node* curr){
	if(curr!=NULL){
		popAll(curr->right);
		popAll(curr->left);
		free(curr);
	}
}
void deleteTree(struct Tree* tree){
	popAll(tree->root);
	tree->root=NULL;
}
void preOrder(struct Node* curr){
	if(!curr){
		return;
	}
	printf("%d ",curr->value);
	preOrder(curr->left);
	preOrder(curr->right);
}
void print(struct Tree* tree){
	preOrder(tree->root);
	puts("");
}
int main(){
	struct Tree* tree = initialize();
	insert(tree,11,70,50,100,90,200,120,20,40,15,75,25);
	print(tree);
	deleteValVar(tree,4,70,50,15,20);
	print(tree);
	return 0;
}
