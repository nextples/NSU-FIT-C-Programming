#include <stdlib.h>
#include <stdio.h>


int max(int a, int b) {
    if (a > b){
        return a;
    }
    return b;
}


struct AVLTreeNode {
    int value;
    int height;
    struct AVLTreeNode* left;
    struct AVLTreeNode* right;
};


struct AVLTree {
    int index;
    struct AVLTreeNode* ptr;
};


struct AVLTree* CreateTree(int n) {
    struct AVLTree* tree = (struct AVLTree*)malloc(sizeof(struct AVLTree));
    tree->ptr = (struct AVLTreeNode*)malloc(sizeof(struct AVLTreeNode) * n);
    tree->index = 0;
    return tree;
}


struct AVLTreeNode* CreateTreeNode(struct AVLTree* tree, int value) {
    struct AVLTreeNode* root = tree->ptr + tree->index++;
    root->value = value;
    root->left = NULL;
    root->right = NULL;
    root->height = 1;
    return root;
}


int CalculateHeight(struct AVLTreeNode* root) {
    if (root != NULL) {
        return root->height;
    }
    return 0;
}


int CalculateBalanceFactor(struct AVLTreeNode* root) {
    return CalculateHeight(root->right) - CalculateHeight(root->left);
}


void FixHeight(struct AVLTreeNode* root) {
    int height_left = CalculateHeight(root->left);
    int height_right = CalculateHeight(root->right);
    root->height = max(height_left, height_right) + 1;
}


struct AVLTreeNode* TurnLeft(struct AVLTreeNode* root) {
    struct AVLTreeNode* new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    FixHeight(root);
    FixHeight(new_root);
    return new_root;
}


struct AVLTreeNode* TurnRight(struct AVLTreeNode* root) {
    struct AVLTreeNode* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    FixHeight(root);
    FixHeight(new_root);
    return new_root;
}


struct AVLTreeNode* SetBalance(struct AVLTreeNode* root) {
    FixHeight(root);
    if (CalculateBalanceFactor(root) == 2) {
        if (CalculateBalanceFactor(root->right) < 0) {
            root->right = TurnRight(root->right);
        }
        return TurnLeft(root);
    }
    if (CalculateBalanceFactor(root) == -2) {
        if (CalculateBalanceFactor(root->left) > 0) {
            root->left = TurnLeft(root->left);
        }
        return TurnRight(root);
    }
    return root;
}


struct AVLTreeNode* InsertNode(struct AVLTree* tree, struct AVLTreeNode* root, int value) {
    if (!root) {
        return CreateTreeNode(tree, value);
    }
    if (value > root->value) {
        root->right = InsertNode(tree, root->right, value);
    }
    else {
        root->left = InsertNode(tree, root->left, value);
    }
    return SetBalance(root);
}


void DestructTree(struct AVLTree* tree) {
    free(tree->ptr);
    free(tree);
}


int main() {
    int n = 0;
    int value = 0;
    if (!scanf("%d", &n)) {
        return 0;
    }
    struct AVLTree* tree = CreateTree(n);
    struct AVLTreeNode* root = NULL;
    for (int i = 0; i < n; i++) {
        if (!scanf("%d", &value)) {
            return 0;
        }
        root = InsertNode(tree, root, value);
    }
    printf("%d", CalculateHeight(root));
    DestructTree(tree);
    return 0;
}
