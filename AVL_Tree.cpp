#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

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
void rerootTree(struct Tree* tree){
	struct Node* start = tree->root;
	while(start->parent != NULL){
		start = start->parent;
	}
	tree->root = start;
}
int checkDepth(struct Node* root){
	if(root == NULL)return 0;
	int left = checkDepth(root->left);
	int right = checkDepth(root->right);
	if(left>right) return 1+left;
	return 1+right;
}
void rightRotation(struct Node* unbalanced){
	struct Node* replacement = unbalanced->left;
	unbalanced->left = replacement->right;
	if(replacement->right)replacement->right->parent = unbalanced;
	replacement->right = unbalanced;
	replacement->parent = unbalanced->parent;
	unbalanced->parent = replacement;
	if(replacement->parent == NULL){
		return;
	}
	if(replacement->parent->left == unbalanced){
		replacement->parent->left = replacement;
		return;
	}
	replacement->parent->right = replacement;
}
void leftRotation(struct Node* unbalanced){
	struct Node* replacement = unbalanced->right;
	unbalanced->right = replacement->left;
	if(replacement->left)replacement->left->parent = unbalanced;
	replacement->left = unbalanced;
	replacement->parent = unbalanced->parent;
	unbalanced->parent = replacement;
	
	if(replacement->parent == NULL){
		return;
	}
	if(replacement->parent->left == unbalanced){
		replacement->parent->left = replacement;
		return;
	}
	replacement->parent->right = replacement;
}
void rebalance(struct Node* unbalanced, bool firstLeft){
	bool secondLeft;
	struct Node* curr = firstLeft? unbalanced->left:unbalanced->right;
	int BF;
	BF = checkDepth(curr->left) - checkDepth(curr->right);
	secondLeft = BF > 0?true:false;
//	secondLeft = BF == 0?firstLeft:secondLeft;
	if(firstLeft&&secondLeft){//LL
		puts("LL");
		rightRotation(unbalanced);
		return;
	}
	if(!firstLeft&&!secondLeft){//RR
		puts("RR");
		leftRotation(unbalanced);
		return;
	}
	if(firstLeft&&!secondLeft){//LR
		puts("LR");
		leftRotation(curr);
		rightRotation(unbalanced);
		printf("Node: %d Left: %d Right: %d\n",unbalanced->parent->value,unbalanced->parent->left?unbalanced->parent->left->value:0,unbalanced->parent->right?unbalanced->parent->right->value:0);
		return;
	}
	if(!firstLeft&&secondLeft){//RL
		puts("RL");
		rightRotation(curr);
		leftRotation(unbalanced);
		printf("Node: %d Left: %d Right: %d\n",unbalanced->parent->value,unbalanced->parent->left?unbalanced->parent->left->value:0,unbalanced->parent->right?unbalanced->parent->right->value:0);
		return;
	}
}
void checkBalance(struct Node* start){
	while(start->parent != NULL){
		int BF =  checkDepth(start->left) - checkDepth(start->right);
		if(abs(BF)>1){
			rebalance(start,BF>0);//if BF greater than 0, left subtree is deeper than right subtree
		}
		start = start->parent;
	}
	int BF =  checkDepth(start->left) - checkDepth(start->right);
	if(abs(BF)>1){
		rebalance(start,BF>0);//if BF greater than 0, left subtree is deeper than right subtree
	}
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

	struct Node* node = createNode(value,previous);
	if(isLeft){
		previous->left = node;
	}
	else{
		previous->right = node;
	}
	checkBalance(node);
	rerootTree(tree);
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
		tree->length-=1;
		if(curr == tree->root){
			tree->root = NULL;
			free(curr);
			return;
		}
		if(curr->parent->left==curr){
			curr->parent->left=NULL;
		}
		else{
			curr->parent->right=NULL;
		}
		checkBalance(curr->parent);
		rerootTree(tree);
		free(curr);
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
	tree->root = NULL;
	tree->length = 0;
}
void preOrder(struct Node* curr){
	if(!curr){
		printf("  ");
		return;
	}
	printf("%d ",curr->value);
	printf("L");
	preOrder(curr->left);
	printf("R");
	preOrder(curr->right);
}
void print(struct Tree* tree){
	preOrder(tree->root);
	puts("");
}
int main(){
	struct Tree* tree = initialize();
	insert(tree,11,2,76,17,21,6,28,79,53,91,44,14);
	print(tree);
	deleteValVar(tree,4,17,76,53,21);
	print(tree);
	deleteTree(tree);
	free(tree);
	return 0;
}
