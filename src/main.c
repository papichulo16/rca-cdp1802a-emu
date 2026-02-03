#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int opcode;
	const char *mnemonic;
} Instruction;

struct Node {
	Instruction instr;
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

struct Node* createNode(Instruction instr) {
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->instr = instr;
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

struct Node* insert(struct Node* node, Instruction instr) {
    if (node == NULL) {
        return createNode(instr);
    }

    if (instr.opcode < node->instr.opcode) {
        node->left = insert(node->left, instr);
    } else if (instr.opcode > node->instr.opcode) {
        node->right = insert(node->right, instr);
    } else {
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);

    // Left Left
    if (balance > 1 && instr.opcode < node->left->instr.opcode)
        return rRotate(node);

    // Right Right
    if (balance < -1 && instr.opcode > node->right->instr.opcode)
        return lRotate(node);

    // Left Right
    if (balance > 1 && instr.opcode > node->left->instr.opcode) {
        node->left = lRotate(node->left);
        return rRotate(node);
    }

    // Right Left
    if (balance < -1 && instr.opcode < node->right->instr.opcode) {
        node->right = rRotate(node->right);
        return lRotate(node);
    }

    return node;
}

Instruction* findByOpcode(struct Node* root, int opcode) {
	if (!root) {
		return NULL;
	}
	if (opcode == root->instr.opcode) {
		return &root->instr;
	} else if (opcode < root->instr.opcode) {
		return findByOpcode(root->left, opcode);
	} else {
		return findByOpcode(root->right, opcode);
	}
}

Instruction* findByMnemonic(struct Node* root, const char* mnemonic) {
	if (!root) {
		return NULL;
	}

	struct Node* queue[256];
	int front = 0;
	int back = 0;

	queue[back++] = root;

	while (front < back) {
		struct Node* curr = queue[front++];

		if (strcmp(curr->instr.mnemonic, mnemonic) == 0) {
			return &curr->instr;
		}

		if (curr->left) {
			queue[back++] = curr->left;
		}

		if (curr->right) {
			queue[back++] = curr->right;
		}
	}
	return NULL;
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
    //Instruction insts[] = {
    //    {0x00, "BAL"},
    //    {0x67, "LSS"},
    //    {0x69, "HI"},
    //    {0x17, "RAH"},
    //    {0x13, "FIH"}
    //};

    //for (int i = 0; i < 5; i++) {
	//	root = insert(root, insts[i]);
	//}

    return 0;
}

	
