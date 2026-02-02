//Still need to make this work for the opcodes, this is just my base to work off


#include <stdio.h>
#include <stdlib.h>

struct Node {
	int key;
	struct Node* left;
	struct Node* right;
	int height;
};

int getHeight(struct Node* n) {
	if (n == NULL) {
		return 0;
	}
	return n->height;
}

struct Node* createNode(int key) {
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}


int max(int a, int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

int getBalanceFactor(struct Node* n) {
	if (n == NULL) {
		return 0;
	}
	return getHeight(n->left) - getHeight(n->right);
}

//Right rotation
struct Node* rRotate(struct Node* y) {
	struct Node* x = y->left;
	struct Node* T2 = x->right;

	x->right = y;
	y->left = T2;
	
	y->height = max(getHeight(y->left), getHeight(y->right))+1;
	x->height = max(getHeight(x->left), getHeight(x->right))+1;

	return x;
}

//Left Rotation
struct Node* lRotate(struct Node* x) {
	struct Node* y = x->right;
	struct Node* T2 = y->left;

	y->left = x;
	x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right))+1;
        y->height = max(getHeight(y->left), getHeight(y->right))+1;

	return y;
}

struct Node* insert(struct Node* node, int key) {
	if (node == NULL) {
		return createNode(key);
	}

	if (key < node->key) {
		node->left = insert(node->left, key);
	} else if (key > node->key) {
		node->right = insert(node->right, key);
	} else {
		return node;
	}

	node->height = 1 + max(getHeight(node->left), getHeight(node->right));
	int balance = getBalanceFactor(node);

	//Left left
	if (balance > 1 && key < node->left->key) {
		return rRotate(node);
	}

	//Right right
	if (balance < -1 && key > node->right->key) {
		return lRotate(node);
	}

	//Left right
	if (balance > 1 && key > node->left->key) {
		node->left = lRotate(node->left);
		return rRotate(node);
	}

	//Right left
	if (balance < -1 && key < node->right->key) {
		node->right = rRotate(node->right);
		return lRotate(node);
	}

	return node;
}

//void inOrder(struct Node* root) {
//	if (root != NULL) {
//		inOrder(root->left);
//		printf("%d ", root->key);
//		inOrder(root->right);
//	}
//}

int main() {
	struct Node* root = NULL;
	//testing vals
	//root = insert(root, 6);
	//root = insert(root, 7);
	//root = insert(root, 67);
	//root = insert(root, 69);
	//root = insert(root, 1337);
	//root = insert(root, 420);

	//printf("inOrder: ");
	//inOrder(root);

	return 0;
}
	
