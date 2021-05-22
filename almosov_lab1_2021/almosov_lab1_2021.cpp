#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node { //we don't need to type "struct" to define a value, so it should be no name here, BUT
	char* data;
	node* next = NULL; //next node
	node* prev = NULL; //prev node
}node; //we only need typedef name in global space to simply create this struct with "node example;"

//
// circular double linked list functions //
//

node* createNode(char* data);

void deleteNode(node** _node, bool returnNext = true);
void deleteList(node** _list);

void pushBack(node** first_node, char* data);
void pushForward(node** first_node, char* data);
void pushNodeBack(node** _node, char* data);
void pushNodeForward(node** _node, char* data);

//input//

char* getWord();

//output//

void printOut(node* _node, bool reverse = false, char format = ' ');
void nodePointerArrayOutput(node** array);

//algos//

node** searchForNode(node* _node, char* data);
void sortList(node* _list);
void deleteDuplicates(node* _list);
int countElements(node* _list);

//other//

void nodePointerArrayDelete(node** array);

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

	word = getWord();

	node** foundIters = searchForNode(_first, word);
	nodePointerArrayOutput(foundIters);

	nodePointerArrayDelete(foundIters);
	_first = foundIters[0];

	deleteDuplicates(_first);

	printOut(_first, true);
	printf("\n%d", countElements(_first));
	return 0;
}

//
// other functions //
//

//Returns a string from console input
char* getWord() {

	int* length = new int{ 0 }; //string length (also used as iterator gradually increased to the size of string later)

	int* memory_size = new int{ 8 }; //default amount of characters to allocate for string.

	char* string = new char[*memory_size]; //pointer to the start of a string
	char* temp_string = NULL;

	char current_char = getchar(); //reading the first character

	 // reading characters until word is over //
	while (current_char != '\n' /*&& current_char != ' '*/) { //someone said space must be the word, so commented.
		string[(*length)++] = current_char; //adding character to a string, increasing length

		// if the length changed over "memory_size", increasing "memory_size" //
		if (*length > *memory_size) { // not "*length + 1", becuase end of string '\0' added after memory reallocation after loop

			temp_string = new char[(*memory_size) * 2]; //// ? C26451 ? ////
			for (int i = 0; i < *memory_size; i++)
				temp_string[i] = string[i]; // moving current string to the next

			delete[] string; //deallocating previous string
			string = temp_string; //updating string pointer

			*memory_size *= 2; //increasing memory twice
		}
		current_char = getchar(); //reading the next character
	}

	//It's possible the situation when u have almost twice memory than needed, so we deallocating this excess memory
	*length += 1;
	temp_string = new char[*length]; //last symbol always will be the '\n' or ' ' and doesnt save into array, so we can not increase length by 1.
	for (int i = 0; i < *length - 1; i++)
		temp_string[i] = string[i];

	delete[] string; //deallocating previous string
	string = temp_string; //updating string pointer

	string[*length - 1] = '\0'; // kind of "EOF", but for our string. //// ? C6386 ? ////

	delete(length);
	delete(memory_size);

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
void deleteList(node** _list) {
	//we need to change actual pointer that goes from higher function so we have pointer to a pointer in arguments
	while (*_list != NULL)
		deleteNode(_list);
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

		(*_node)->prev->next = new_node;
		(*_node)->prev = new_node;
	}
}

//Prints list out
void printOut(node* _node, bool reverse, char format) {
	node* iter = _node; //because list is circular, we need to have an iterator to know if we went through the list
	if (_node == NULL)
		return;
	else if (reverse) {
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

node** searchForNode(node* _node, char* data) {
	int* length = new int{ 0 };

	int* memory_size = new int{ 2 };

	node** found = new node * [*memory_size];
	node** temp_array = NULL;

	node* iter = _node;
	do {
		if (strcmp(data, iter->data) == 0) {
			found[(*length)++] = iter;
			if (*length >= *memory_size) {
				temp_array = new node * [(*memory_size) * 2];
				for (int i = 0; i < *memory_size; i++)
					temp_array[i] = found[i]; // moving current array to the next, with more space

				delete[] found; //deallocating previous array
				found = temp_array; //updating array pointer

				*memory_size *= 2; //increasing memory twice
			}
		}
		iter = iter->next;
	} while (iter != _node);

	temp_array = new node * [(*length)];
	for (int i = 0; i < *length; i++)
		temp_array[i] = found[i];
	found[*length] = NULL; //last is NULL

	delete(length);
	delete(memory_size);
	return found;
}

void nodePointerArrayDelete(node** array) {
	for (int i = 0;; i++) {
		if (array[i] == NULL)
			break;
		deleteNode(&array[i]);
	}
}

void nodePointerArrayOutput(node** array) {
	for (int i = 0;; i++) {
		if (array[i] == NULL)
			break;
		printf("| #%d | %s |\n", i + 1, array[i]->data);
	}
}

void sortList(node* _list) {

}

void deleteDuplicates(node* _list) {
	node* iter = _list;
	node* jter;
	do {
		jter = iter->next;
		while (jter != iter) {
			printf("I:%s J:%s\n", iter->data, jter->data);

			if (strcmp(iter->data, jter->data) == 0)
			{
				deleteNode(&jter);
				continue;
			}
			jter = jter->next;
		}
		printf("\n");
		iter = iter->next;
	} while (iter != _list);
}

int countElements(node* _list) {
	if (_list == NULL)
		return 0;

	int count = 1;
	for (node* iter = _list; iter->next != _list; iter = iter->next)
		count++;
	return count;
}