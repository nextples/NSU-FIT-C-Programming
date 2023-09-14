#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ASCII_SIZE 256
#define BUFFER_SIZE 1000
#define BITS_IN_LONG_LONG 63

FILE *in;
FILE *out;

typedef struct TreeNode {
    int weight;
    unsigned char character;
    bool empty;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct Tree {
    TreeNode list[ASCII_SIZE];
    int length;
} Tree;

void Swap(TreeNode *node1, TreeNode *node2) {
    TreeNode tmp = *node1;
    *node1 = *node2;
    *node2 = tmp;
}

void AddToTree(Tree *nodeList, TreeNode node) {
    nodeList->length++;
    nodeList->list[nodeList->length] = node;
}

void SortTree(Tree *nodeList) { // ПО УБЫВАНИЮ
    for (int i = 0; i <= nodeList->length; i++) {
        for (int j = 0; j < nodeList->length; j++) {
            if (nodeList->list[j].weight < nodeList->list[j + 1].weight) {
                Swap(&nodeList->list[j], &nodeList->list[j + 1]);
            }
        }
    }
}

TreeNode Pop(Tree *nodeList) {
    if (nodeList->length < 0) {
        exit(EXIT_SUCCESS); // ПУСТОЕ ДЕРЕВО
    }
    return nodeList->list[nodeList->length--];
}

TreeNode Pick(Tree *nodeList) {
    if (nodeList->length < 0) {
        exit(EXIT_SUCCESS); // ПУСТОЕ ДЕРЕВО
    }
    return nodeList->list[nodeList->length];
}

void RecalculateTree(Tree *nodeList) {
    SortTree(nodeList);
    TreeNode *left = malloc(sizeof(TreeNode));
    TreeNode *right = malloc(sizeof(TreeNode));
    *left = Pop(nodeList);
    *right = Pop(nodeList);

    TreeNode newNode = (TreeNode) {left->weight + right->weight, 0, true, left, right};
    AddToTree(nodeList, newNode);
}

Tree CreateTreeFromTable(const int frequencyTable[], const unsigned char characters[], int charactersNumb) {
    Tree nodeList;
    nodeList.length = -1;

    for (int i = 0; i < charactersNumb; i++) {
        TreeNode node = (TreeNode){frequencyTable[(int) characters[i]], characters[i], false, NULL, NULL};
        AddToTree(&nodeList, node);
    }

    if (charactersNumb == 1) {
        TreeNode emptyNode = (TreeNode) {0, 0, false, NULL, NULL};
        AddToTree(&nodeList, emptyNode);
    }
    return nodeList;
}

int NextCharacters(unsigned char buffer[]) {
    return (int) fread(buffer, sizeof(unsigned char), BUFFER_SIZE, in);
}

unsigned long long WriteZeroToCode(unsigned long long code, char *codeLength) {
    code &= ~(1llu << (BITS_IN_LONG_LONG - *codeLength));
    (*codeLength)++;
    return code;
}

unsigned long long WriteOneToCode(unsigned long long code, char *codeLength) {
    code |= (1llu << (BITS_IN_LONG_LONG - *codeLength));
    (*codeLength)++;
    return code;
}

void GetCharacterCode(unsigned long long rootCode, TreeNode node, char codeLength, unsigned long long codesMap[], char codesMapLength[], char bitToWrite) {
    if (bitToWrite == 0) {
        rootCode = WriteZeroToCode(rootCode, &codeLength);
    }
    if (bitToWrite == 1) {
        rootCode = WriteOneToCode(rootCode, &codeLength);
    }

    if (node.empty == false) {
        codesMap[(int) node.character] = rootCode;
        codesMapLength[(int) node.character] = codeLength;
    }
    else {
        if (node.left != NULL) {
            GetCharacterCode(rootCode, *node.left, codeLength, codesMap, codesMapLength, 0);
        }
        if (node.right != NULL) {
            GetCharacterCode(rootCode, *node.right, codeLength, codesMap, codesMapLength, 1);
        }
    }
}

void GetAllCodes(Tree *nodeList, unsigned long long codesMap[], char codesMapLength[]) {
    unsigned long long code = 0;
    GetCharacterCode(code, nodeList->list[0], 0, codesMap, codesMapLength, -1);
}

int CountBitLengthOfEncodedText(unsigned char buffer[], const char mapOfCodeLength[]) {
    int sum = 0;
    int readLength;
    while ((readLength = NextCharacters(buffer))) {
        for (int i = 0; i < readLength; i++)
            sum += mapOfCodeLength[(int) buffer[i]];
    }
    fseek(in, 1, SEEK_SET);
    return sum;
}

void AddBitToByteAndWrite(char *bitPosition, unsigned char *byte, int bit) {
    if (bit == 1) {
        *byte |= 1 << (7 - *bitPosition); // ВСТАВИТЬ 1 на bitPosition
    }
    if (bit == 0) {
        *byte &= ~(1 << (7 - *bitPosition)); // ВСТАВИТЬ 0 на bitPosition
    }
    (*bitPosition)++;

    if (*bitPosition == 8) {
        fprintf(out, "%c", *byte);
        (*bitPosition) = 0;
    }
}

bool BitInLength(int length, int shift) {
    return length & (1u << (31 - shift));
}

void WriteBitLengthOfEncodedText(int length, char *currentBitNumber, unsigned char *currentByte) {
    for (int shift = 24; shift < 32; shift++) {
        if (BitInLength(length, shift) == 1) {
            AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
        }
        else {
            AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
        }
    }
}

bool BitInCharacterOfNode(TreeNode node, int shift) {
    return node.character & (1 << (7 - shift));
}

void WriteEncodedTree(TreeNode node, char *currentBitNumber, unsigned char *currentByte) {
    if (node.empty == false) {
        AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
        for (int shift = 0; shift < 8; shift++) {
            if (BitInCharacterOfNode(node, shift) == 1) {
                AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
            }
            else {
                AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
            }
        }
    }
    else {
        AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
        if (node.left != NULL) {
            WriteEncodedTree(*node.left, currentBitNumber, currentByte);
        }
        if (node.right != NULL) {
            WriteEncodedTree(*node.right, currentBitNumber, currentByte);
        }
    }
}

bool BitInCode(const unsigned char buffer[], const unsigned long long mapOfCodes[], int shift, int i) {
    return mapOfCodes[(int) buffer[i]] & (1llu << (BITS_IN_LONG_LONG - shift));
}

void WriteEncodedText(unsigned char buffer[], const unsigned long long mapOfCodes[], const char mapOfCodesLength[], char *currentBitNumber, unsigned char *currentByte) {
    int readLength;
    while ((readLength = NextCharacters(buffer))) {
        for (int i = 0; i < readLength; i++) {
            for (int shift = 0; shift < mapOfCodesLength[(int) buffer[i]]; shift++) {
                if (BitInCode(buffer, mapOfCodes, shift, i) == 1) {
                    AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
                }
                else {
                    AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
                }
            }
        }
    }
    fseek(in, 1, SEEK_SET);
}

void CreateFrequencyTable(unsigned char buffer[], int frequencyTable[], unsigned char characters[], int *charactersNumb) {
    int readLength;
    while ((readLength = NextCharacters(buffer))) {
        for (int i = 0; i < readLength; i++) {
            if (!frequencyTable[(int) buffer[i]]) {
                characters[*charactersNumb] = buffer[i];
                (*charactersNumb)++;
            }
            frequencyTable[(int) buffer[i]]++;
        }
    }
}

void DestructTreeDFS(TreeNode *root) {
    if (!root) {
        return;
    }
    DestructTreeDFS(root->left);
    DestructTreeDFS(root->right);
    free(root);
}

void Encoding() {
    int frequencyTable[ASCII_SIZE] = {0};
    unsigned char buffer[BUFFER_SIZE] = {0};
    unsigned char characters[ASCII_SIZE] = {0};
    int charactersNumb = 0;

    CreateFrequencyTable(buffer, frequencyTable, characters, &charactersNumb);

    fseek(in, 1, SEEK_SET);
    Tree nodeList = CreateTreeFromTable(frequencyTable, characters, charactersNumb);
    while (nodeList.length > 0) {
        RecalculateTree(&nodeList);
    }

    unsigned long long codesMap[ASCII_SIZE] = {0};
    char codesMapLength[ASCII_SIZE] = {0};

    GetAllCodes(&nodeList, codesMap, codesMapLength);

    char currentBitNumber = 0;
    unsigned char currentByte = 0;
    int bitLengthOfEncodedText = CountBitLengthOfEncodedText(buffer, codesMapLength);

    /*
     * 1 byte for bitLengthOfEncodedText % 8
     * bits of encoded tree (which is rounded to bytes)
     * bits of encoded text
     */

    // length % 8
    WriteBitLengthOfEncodedText(bitLengthOfEncodedText % 8, &currentBitNumber, &currentByte);

    // tree
    WriteEncodedTree(Pick(&nodeList), &currentBitNumber, &currentByte);
    if (currentBitNumber != 0) {
        fprintf(out, "%c", currentByte);
        currentBitNumber = 0;
    }

    // text
    WriteEncodedText(buffer, codesMap, codesMapLength, &currentBitNumber, &currentByte);
    if (currentBitNumber != 0) {
        fprintf(out, "%c", currentByte);
        currentBitNumber = 0;
    }

    // FREE MEMORY
    DestructTreeDFS(Pick((&nodeList)).left);
    DestructTreeDFS(Pick((&nodeList)).right);

}

unsigned char ReadLength() {
    fseek(in, 1, SEEK_SET);
    unsigned char rest;
    if (!fscanf(in, "%c", &rest)) {
        return 0;
    }
    return rest;
}

unsigned char ReadCharacter(const unsigned char buffer[], int *bitNumber) {
    unsigned char character = 0;
    character |= (buffer[*bitNumber / 8] << (*bitNumber % 8));
    (*bitNumber) += 8;
    character |= (buffer[*bitNumber / 8] >> (8 - (*bitNumber % 8)));
    return character;
}

bool BitInEncodedTree(const unsigned char buffer[], const int *bitNumber) {
    return (buffer[*bitNumber / 8] << (*bitNumber % 8)) & (unsigned char) (1 << 7);
}

TreeNode* ReadNodesOfTree(unsigned char buffer[], int *bitNumber) {
    if (BitInEncodedTree(buffer, bitNumber)) {
        (*bitNumber)++;
        unsigned char character = ReadCharacter(buffer, bitNumber);
        TreeNode *node = malloc(sizeof(TreeNode));
        *node = (TreeNode) {0, character, false, NULL, NULL};
        return node;
    }
    else {
        (*bitNumber)++;
        TreeNode *leftChild = ReadNodesOfTree(buffer, bitNumber); // СОЗДАЕМ ЛЕВОГО РЕБЕНКА
        TreeNode *rightChild = ReadNodesOfTree(buffer, bitNumber); // СОЗДАЕМ ПРАВОГО РЕБЕНКА
        TreeNode *node = malloc(sizeof(TreeNode));
        *node = (TreeNode) {0, 0, true, leftChild, rightChild}; // СОЗДАЕМ КОРЕНЬ
        return node;
    }
}

bool BitInCurrentByte(unsigned char currentByte, const int *bitNumber) {
    return (currentByte << (*bitNumber % 8)) & (1 << 7);
}

void DecodeText(TreeNode *currentNode, TreeNode *root, int *bitNumber, int bitInLastByte) {
    unsigned char currentByte = 0;
    unsigned char nextByte = 0;

    if (bitInLastByte == 0) {
        bitInLastByte = 8;
    }
    if (!fscanf(in, "%c", &currentByte)) {
        return;
    }

    bool isEnd = false;
    while (!isEnd) {
        int readTo;
        if (fscanf(in, "%c", &nextByte) != EOF) {
            readTo = 8;
        }
        else {
            readTo = bitInLastByte;
            isEnd = true;
        }

        for (int i = 0; i < readTo; i++) {
            if (BitInCurrentByte(currentByte, bitNumber) == 1) {
                if (currentNode->right != NULL) {
                    currentNode = currentNode->right;
                    (*bitNumber)++;
                }
            }
            else {
                if (currentNode->left != NULL) {
                    currentNode = currentNode->left;
                    (*bitNumber)++;
                }
            }
            if (currentNode->empty == false) {
                fprintf( out, "%c", currentNode->character);
                currentNode = root;
            }
        }
        currentByte = nextByte;
    }
}

void Decoding() {
    unsigned char bitInLastByte = ReadLength();
    int bitNumber = 0;
    unsigned char buffer[BUFFER_SIZE];
    if (!NextCharacters(buffer))
        return;

    TreeNode *treeRoot = ReadNodesOfTree(buffer, &bitNumber);

    fseek(in, (long int) (2 + ceil((double) bitNumber / 8)), SEEK_SET); // ПЕРЕНОС КУРСОРА НА 0-ой ЭЛЕМЕНТ text
    bitNumber = 0;
    DecodeText(treeRoot, treeRoot, &bitNumber, bitInLastByte);

    DestructTreeDFS(treeRoot);
}


int main() {
    out = fopen("C:\\Users\\User\\labs\\labs 1st course\\lab5\\src\\123.txt", "wb");
    in = fopen("out.txt", "rb");

    char mode = 0;

    if (!fscanf(in, "%c", &mode)) {
        return 0;
    }
    if (mode == 'c') {
        Encoding();
    }
    if (mode == 'd') {
        Decoding();
    }
    //fprintf(out, "\n");
    //Decoding();

    fclose(in);
    fclose(out);
}