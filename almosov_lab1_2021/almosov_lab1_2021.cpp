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

void printOut(node* _node, bool reverse = false, char format = ' ', char end = '\n');
void nodePointerArrayOutput(node** array);

//algos//

node** searchForNode(node* _node, char* data);
void sortList(node* _list);
void deleteDuplicates(node* _list);
int countElements(node* _list);

//other//

void deleteNodeByArray(node** _list, node** array, int elem = 0);
int strcmp_custom(char* str1, char* str2) {

	while (*str1 != '\0') {
		if (*str2 == '\0')	return  0;
		if (*str1 < *str2)	return -1;
		if (*str1 > *str2)	return  1;

		str1++;
		str2++;
	}

	if (*str2 != '\0') return -1;

	return 0;
}

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
	printf("List:"); printOut(_first);

	printf("\nfind:"); word = getWord();
	node** foundIters = searchForNode(_first, word);
	nodePointerArrayOutput(foundIters);
	deleteNodeByArray(&_first, foundIters);
	printf("Found nodes deleted!\n");

	printf("List:"); printOut(_first);

	printf("additional elements:\n");
	for (int i = 0; i < 4; i++) {
		word = getWord();
		pushForward(&_first, word);
	}
	deleteDuplicates(_first);

	printf("\n");
	printOut(_first, false);

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

	string[*length - 1] = '\0'; // kind of "EOF", but for our string.

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

	// we can change given pointer in two ways //
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
//Format -> character between elemnts
//End -> character at the end
void printOut(node* _node, bool reverse, char format, char end) {
	node* iter = _node; //because list is circular, we need to have an iterator to know if we went through the list
	if (_node == NULL) {
		printf("%c", end);
		return;
	}
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
	printf("%c", end);
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
				temp_array = new node * [(*memory_size) * 2]; //// ? C26451 ? ////
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
	found[*length] = NULL; //last is NULL to know where 'found' array ends

	delete(length);
	delete(memory_size);
	return found;
}

//'int elem' - input # of element, not index!
void deleteNodeByArray(node** list, node** array, int elem) {
	for (int i = 1; *array != NULL; array++)
		if (elem < 1)
			if (list[0]->data = array[0]->data) deleteNode(list); //changing our list pointer if it points to a pointer in a NodeArray to delete
			else deleteNode(array);
		else if (i == elem)
			if (list[0]->data = array[0]->data) deleteNode(list);
			else deleteNode(array); //changing our list pointer if it points to a pointer in a NodeArray to delete
			deleteNode(array);
}

void nodePointerArrayOutput(node** array) {
	if (*array == NULL)
		return;
	for (int i = 1;*array != NULL; array++, i++)
		printf("| #%d |  %s <- %s -> %s | \n", i, (*array)->prev->data, (*array)->data, (*array)->next->data);
}

void sortList(node* _list) {

}

void deleteDuplicates(node* _list) {
	if (_list == NULL)
		return;
	if (_list->next == _list)
		return;

	node* iter = _list;
	node* jter;
	do {
		jter = iter->next; //comparing from next after 'iter', to not delete it.
		while (jter != iter) {
			if (strcmp(iter->data, jter->data) == 0)
			{
				deleteNode(&jter, true); //if we found duplicate node, delete and check next
				continue; //'deleteNode()' changes 'jter' to the next node, so we skip while loop iteration
			}
			jter = jter->next;
		}
		iter = iter->next;
	} while (iter != _list); //iterating until we made a cycle in a list
}

int countElements(node* _list) {
	if (_list == NULL)
		return 0;
	if (_list->next == _list)
		return 1;

	int count = 1;
	for (node* iter = _list; iter->next != _list; iter = iter->next)
		count++;
	return count;
}