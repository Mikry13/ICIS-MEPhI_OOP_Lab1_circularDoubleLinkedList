#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma once

//
// circular double linked list functions //
//

typedef struct node { //we don't need to type "struct" to define a value, so it should be no name here, BUT
	char* data;
	node* next = NULL; //next node
	node* prev = NULL; //prev node
}node; //we only need typedef name in global space to simply create this struct with "node example;"


// basic //

node* createNode(char* data);

void deleteNode(node** _node, bool returnNext = true);
void deleteList(node** _list);

void pushBack(node** first_node, char* data);
void pushForward(node** first_node, char* data);
void pushNodeBack(node** _node, char* data);
void pushNodeForward(node** _node, char* data);

//input//

char* getWord(FILE* file = NULL);
node* readFromFile(FILE* file, bool insertAtEnd);


//output//

void outputList(node* _node, bool reverse = false, char format = ' ', char end = '\n');
void nodePointerArrayOutput(node** array);
void outputToFile(FILE* f, node* _list);


//algos//

node** searchForNode(node* _node, char* data);
void deleteNodeByArray(node** _list, node** array, int elem = 0);

void sortList(node* _list, bool descending);
void deleteDuplicates(node* _list);
int countElements(node* _list);

//other//

int nodePointerArrayCount(node** array);
int strcmp_custom(char* str1, char* str2);