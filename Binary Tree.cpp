#include <stdio.h>
#include <stdlib.h>
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
void addNode(int value, struct Tree* tree){
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
struct Node* search(int value, struct Tree* tree){
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
void deleteVal(int value, struct Tree* tree){
	struct Node* curr = search(value, tree);
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
	deleteVal(replacementValue,tree);
	curr->value = replacementValue;
	return;
}
void find(int value, struct Tree* tree){
	if(search(value,tree)){
		printf("%d is in the tree\n",value);
		return;
	}
	printf("%d is NOT in the tree\n",value);
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
}
int main(){
	struct Tree* tree = initialize();
	addNode(12,tree);
	addNode(9,tree);
	addNode(15,tree);
	addNode(16,tree);
	addNode(10,tree);
	addNode(8,tree);
	addNode(14,tree);
	deleteVal(15,tree);
	find(27,tree);
	find(16,tree);
	print(tree);
	return 0;
}
