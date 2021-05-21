﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node{ //we don't need to type "struct" to define a value, so it should be no name here, BUT
	char* data;
	node* next = NULL; //next node
	node* prev = NULL; //prev node
}node; //we only need typedef name in global space to simply create this struct with "node example;"

//
// circular double linked list functions //
//

node* createNode(char* data);

void deleteNode(node** _node, bool returnNext = true);
void deleteList(node** _node);

void pushBack(node** first_node, char* data);
void pushForward(node** first_node, char* data);
void pushNodeBack(node** _node, char* data);
void pushNodeForward(node** _node, char* data);

void printOut(node* _node, bool reverse = false, char format = ' ');

node** searchForNode(node* _node, char* data);

//
// other functions //
//

char* getWord();

//
// main //
//
int main()
{
	node* _first = NULL;
	char* word = getWord();
	pushForward(&_first, word);
	for (int i = 0; i < 4; i++) {
		word = getWord();
		pushForward(&_first, word);
	}
	printf("\nfind:");

	printOut(_first, true);

	return 0;
}

//
// other functions //
//

//Returns a string from console input
char* getWord() {
	
	int* length = new int{ 0 }; //string length (also used as iterator gradually increased to the size of string later)

	int* memory_size = new int{ 8 }; //default amount of characters to allocate for string.

	char* string = new char[*memory_size]; //pointer to the start of a string //c6280 on realloc
	//char* string = (char*)malloc(sizeof(char) * (memory_size + 1)); //C6308 on realloc

	char current_char = getchar(); //reading the first character

	 // reading characters until word is over //
	while (current_char != '\n' && current_char != ' ') {
		string[(*length)++] = current_char; //adding character to a string

		// if the length changed over "memory_size", increasing "memory_size" //
		if (*length + 1 > *memory_size) { //"=" because we need extra character to text kind of "EOF", but for our string.
			*memory_size *= 2; //increasing memory twice
			string = (char*)realloc(string, *memory_size * sizeof(char)); // recreating string with twice the memory
			//NOTE: | "realloc()" can move memory block with a new size to the new place			|
			//		| if it cant fully allocate it after given pointer. So there're no memory leak!	|
			//		| (means we dont need to free memory ourselves)									|
		}
		current_char = getchar(); //reading the next character
	}
	string[*length] = '\0'; // kind of "EOF", but for our string.

	/* /// doesn't work! ///
	char* temp;
	for (int i = *length + 1; i < *memory_size + 1; i++) //getting rid of excess memory if there're so
		free(&string[i]);
	*/

	delete(length);
	return string;
}

//
// circular double linked list //
//

//Creates a new node
node* createNode(char* data) { //creates new node with given data
	node* new_node = new node{ data };
	new_node->next = new_node;
	new_node->prev = new_node;
	return new_node;
}

//Deletes a node
void deleteNode(node** _node, bool returnNext) {

	if (*_node == NULL)
		return;

	// if node the last, delete and return 0 //
	else if ((*_node)->next == *_node)
	{
		delete(*_node);
		*_node = NULL;
		return;
	}

	node* output;

	if (returnNext)
		output = (*_node)->next;
	else
		output = (*_node)->prev;

	// pointing nodes around "_node" to each other //
	(*_node)->prev->next = (*_node)->next;
	(*_node)->next->prev = (*_node)->prev;

	// and delete the node
	delete(*_node);
	*_node = output;
}

//Deletes full list
void deleteList(node** _node) {
	//we need to change actual pointer that goes from higher function so we have pointer to a pointer in arguments
	while (*_node != NULL)
		deleteNode(_node);
}

//Adds node to the end of a list
void pushBack(node** first_node, char* data) {
	//we need pointer to pointer only because there're can be NULL
	if (*first_node == NULL) {
		*first_node = createNode(data);
	}
	else {
		// creating new node //
		node* new_node = createNode(data);

		// saving the first and last nodes (new node will point to both obviously) //
		new_node->next = *first_node;
		new_node->prev = (*first_node)->prev;

		(*first_node)->prev->next = new_node; //pointing last to the new
		(*first_node)->prev = new_node; //pointing first to the new
	}
}

//Adds node to the beginning of a list
void pushForward(node** first_node, char* data) {
	// almost the same as "pushBack", but we change "first_node" at the end.
	// we need to change a pointer to a first node so we have "node**" in arguments of a function
	if (*first_node == NULL)
		*first_node = createNode(data);
	else {
		// creating new node //
		node* new_node = createNode(data);

		// saving the first and last nodes (new node will point to both obviously) //
		new_node->next = *first_node;
		new_node->prev = (*first_node)->prev;

		//
		(*first_node)->prev->next = new_node; //pointing last to the new
		(*first_node)->prev = new_node; //pointing first to the new

		(*first_node) = new_node; //new_node now the first
	}
}

//Adds node after the given iterator
void pushNodeBack(node** _node, char* data) {
	//we need pointer to pointer only because there're can be NULL
	if (*_node == NULL)
		*_node = createNode(data);
	else {
		node* new_node = createNode(data);

		new_node->next = (*_node)->next;
		new_node->prev = *_node;

		(*_node)->next->prev = new_node;
		(*_node)->next = new_node;
	}
}

//Adds node before the given iterator
void pushNodeForward(node** _node, char* data) {
	//if we want to push node forward, it will change its position relative to first node, so we need "node**"
	if (*_node == NULL)
		*_node = createNode(data);
	else {
		node* new_node = createNode(data);

		new_node->next = *_node;
		new_node->prev = (*_node)->prev;

		(*_node)->prev->next= new_node;
		(*_node)->prev= new_node;
	}
}

//Prints list out
void printOut(node* _node, bool reverse, char format) {
	node* iter = _node; //because list is circular, we need to have an iterator to know if we went through the list
	if (_node == NULL)
		return;
	else if (reverse){
		_node = _node->prev;
		iter = _node;
		while (iter->prev != _node) {
			printf("%s%c", iter->data, format);
			iter = iter->prev;
		}
		printf("%s%c", iter->data, format);
	}
	else {
		while (iter->next != _node) {
			printf("%s%c", iter->data, format);
			iter = iter->next;
		}
		printf("%s%c", iter->data, format);
		
	}
}