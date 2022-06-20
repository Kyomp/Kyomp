#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
struct Node{
    int value;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int height;
};
struct Tree{
    struct Node* root;
    int length;
};
struct Tree* initialize(){
    struct Tree* tree = (struct Tree*) malloc(sizeof(struct Tree));
    tree->root = NULL;
    tree->length = 0;
    return tree;
}
struct Node* createNode(int value, struct Node* parent){
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->parent = parent;
    newNode->left = newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}
int max(int X, int Y){
    return X>Y?X:Y;
}
int getHeight(struct Node* node){
    return node==NULL?0:node->height;
}
void heightUpdate(struct Node* node){
    struct Node* curr = node;
    while(curr!=NULL){
        curr->height = 1 + max(getHeight(curr->left),getHeight(curr->right));
        curr = curr->parent;
    }
}
void reroot(struct Tree* tree){
	struct Node* start = tree->root;
	while(start->parent != NULL){
		start = start->parent;
	}
	tree->root = start;
}
void rightRotation(struct Node* unbalanced){
	struct Node* replacement = unbalanced->left;
	unbalanced->left = replacement->right;
	if(replacement->right)replacement->right->parent = unbalanced;
	replacement->right = unbalanced;
	replacement->parent = unbalanced->parent;
	unbalanced->parent = replacement;
	if(replacement->parent == NULL){
	}
	else if(replacement->parent->left == unbalanced){
		replacement->parent->left = replacement;
	}
	else{
		replacement->parent->right = replacement;
	}
//	puts("right rotation");
//	printf("Node: %d(%d) Left: (%d) Right: (%d)\n",replacement->value,replacement->height,getHeight(replacement->left),getHeight(replacement->right));
    heightUpdate(unbalanced);
//	printf("Node: %d(%d) Left: (%d) Right: (%d)\n",replacement->value,replacement->height,getHeight(replacement->left),getHeight(replacement->right));
    
}
void leftRotation(struct Node* unbalanced){
	struct Node* replacement = unbalanced->right;
	unbalanced->right = replacement->left;
	if(replacement->left)replacement->left->parent = unbalanced;
	replacement->left = unbalanced;
	replacement->parent = unbalanced->parent;
	unbalanced->parent = replacement;
	
	if(replacement->parent == NULL){
	}
	else if(replacement->parent->left == unbalanced){
		replacement->parent->left = replacement;
	}
	else{
		replacement->parent->right = replacement;	
	}
//	puts("left rotation");
//	printf("Node: %d(%d) Left: (%d) Right: (%d)\n",replacement->value,replacement->height,getHeight(replacement->left),getHeight(replacement->right));
    heightUpdate(unbalanced);
//	printf("Node: %d(%d) Left: (%d) Right: (%d)\n",replacement->value,replacement->height,getHeight(replacement->left),getHeight(replacement->right));

}
void rebalance(struct Node* unbalanced, bool firstLeft){
    struct Node* curr = firstLeft? unbalanced->left: unbalanced->right;
    bool secondLeft = getHeight(curr->left)>getHeight(curr->right);
    if(firstLeft&&secondLeft){//LL
		// puts("LL");
		rightRotation(unbalanced);
		return;
	}
	if(!firstLeft&&!secondLeft){//RR
		// puts("RR");
		leftRotation(unbalanced);
		return;
	}
	if(firstLeft&&!secondLeft){//LR
		// puts("LR");
		leftRotation(curr);
		rightRotation(unbalanced);
		// printf("Node: %d Left: %d Right: %d\n",unbalanced->parent->value,unbalanced->parent->left?unbalanced->parent->left->value:0,unbalanced->parent->right?unbalanced->parent->right->value:0);
		return;
	}
	if(!firstLeft&&secondLeft){//RL
		// puts("RL");
		rightRotation(curr);
		leftRotation(unbalanced);
		// printf("Node: %d Left: %d Right: %d\n",unbalanced->parent->value,unbalanced->parent->left?unbalanced->parent->left->value:0,unbalanced->parent->right?unbalanced->parent->right->value:0);
		return;
	}
}
void checkBalance(struct Node* node,struct Tree* tree){
    struct Node* curr = node;
    while(curr != NULL){
        int BF = getHeight(curr->left) - getHeight(curr->right);
//		printf("%d BF: %d\n",curr->value,BF);
        if(abs(BF)>1){
            bool firstLeft = BF>0?true:false;
            rebalance(curr,firstLeft);
            reroot(tree);
        }
		curr = curr->parent;
    }
}
void addNode(int value, struct Tree* tree){
    if(tree->root == NULL){
        struct Node* newNode = createNode(value, NULL);
        tree->root = newNode;
		tree->length += 1;
        return;
    }
    struct Node* curr = tree->root;
    struct Node* previous;
    bool Left;
    while(curr!=NULL){
        previous = curr;
        if(value<curr->value){
            Left = true;
            curr = curr->left;
            continue;
        }
        if(value>curr->value){
            Left = false;
            curr = curr->right;
            continue;
        }
        if(value == curr->value){
            puts("This is a duplicate");
            return;
        }
    }
    struct Node* newNode = createNode(value, previous);
    if(Left){
        previous->left = newNode;
    }
    else{
        previous->right = newNode;
    }
	tree->length += 1;
    heightUpdate(newNode);
    checkBalance(newNode,tree);
}
void insert(struct Tree* tree, int size, ...){
	va_list ptr;
	va_start(ptr,size);
	for(int i = 0; i<size; i++){
		addNode(va_arg(ptr,int),tree);
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
        heightUpdate(curr->parent);
		checkBalance(curr->parent,tree);
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
	int input;
	do{
		puts("Choose:");
		puts("1. Insert");
		puts("2. Delete");
		puts("3. View");
		puts("4. Exit");
		printf("Size of tree: %d\n",tree->length);
		printf("Input: ");
		scanf("%d",&input);getchar();
		switch(input){
			case 1:{
				int value;
				printf("Inserted Number: ");
				scanf("%d",&value);getchar();
				addNode(value,tree);
				break;
			}
			case 2:{
				int value;
				printf("Deleted Number: ");
				scanf("%d",&value);getchar();
				deleteVal(tree,value);
				break;
			}
			case 3:{
				print(tree);
				break;
			}
		}
	}while(input!=4);
	deleteTree(tree);
	free(tree);
	return 0;
}
