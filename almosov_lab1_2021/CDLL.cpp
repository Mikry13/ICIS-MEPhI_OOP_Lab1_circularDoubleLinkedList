#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 26451) //multiplying data got from pointer warning. Everything's fine.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "CDLL.h"

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
		delete ((*_node)->data);
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
	delete ((*_node)->data); //delete char*
	delete(*_node); //delete structure
	*_node = output; //update pointer
}

//Deletes full list
void deleteList(node** _list) {
	//we need to change actual pointer that goes from higher function so we have pointer to a pointer in arguments
	while (*_list != NULL)
		deleteNode(_list, true);
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

//
// input //
//

//Returns a string from console input
char* getWord(FILE* file) {

	int* length = new int{ 0 }; //string length (also used as iterator gradually increased to the size of string later)

	int* memory_size = new int{ 8 }; //default amount of characters to allocate for string.

	char* string = new char[*memory_size]; //pointer to the start of a string
	char* temp_string = NULL;

	char* current_char = new char; //pointer, because 'fscanf' doesnt support regular char

	if (file != NULL) {
		if (fscanf(file, "%c", current_char) < 1) {
			return NULL;
		}
	}
	else
		*current_char = getchar();

	// reading characters until word is over //
	while (*current_char != '\n' /*&& current_char != ' '*/) {
		string[(*length)++] = *current_char; //adding character to a string, increasing length

		// if the length changed over "memory_size", increasing "memory_size" //
		if (*length >= *memory_size) { // >= to not overread the unused memory

			temp_string = new char[(*memory_size) * 2];
			for (int i = 0; i < *memory_size; i++)
				temp_string[i] = string[i]; // moving current string to the next

			delete[] string; //deallocating previous string
			string = temp_string; //updating string pointer

			*memory_size *= 2; //increasing memory twice
		}
		//reading the next character
		if (file != NULL) {
			if (fscanf(file, "%c", current_char) < 1) {
				return NULL;
			}
		}
		else
			*current_char = getchar();
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

node* readFromFile(FILE* file, bool insertAtEnd) {
	node* output = NULL;
	char* word = getWord(file);	
	if (file != NULL && word != NULL)
		for (; word != NULL; word = getWord(file))
			if (insertAtEnd)
				pushBack(&output, word);
			else pushForward(&output, word);
	return output;
}

//
// output //
//

//Prints list out
//Format -> character between elemnts
//End -> character at the end
void outputList(node* _node, bool reverse, char format, char end) {
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

void nodePointerArrayOutput(node** array) {
	if (*array == NULL)
		return;
	
	for (int i = 1; array[i - 1] != NULL; i++)
		printf("| #%d |  %s <- %s -> %s | \n", 
			i, array[i - 1]->prev->data, array[i - 1]->data, array[i - 1]->next->data);
}

void outputToFile(FILE* f, node* _list)
{
	if (f && (_list != NULL)) {
		fprintf(f, "%s\n", _list->data); //we start from '_list->next' and to not break the 'for' loop, we write _list first.
		for (node* iter = _list->next; iter != _list; iter = iter->next)
			fprintf(f, "%s\n", iter->data);
	}
}

//
// algos //
//

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
void deleteNodeByArray(node** array, node** list, int elem) {
	if (array == NULL || *array == NULL || list == NULL || *list == NULL)
		return;
	
	for (int i = 1; array [i - 1]!= NULL; i++)
		if (elem < 1) {
			if (strcmp((*list)->data, array[i - 1]->data) == 0)
			{
				deleteNode(list, true);
				return;
			}//changing our list pointer if it points to a pointer in a NodeArray to delete
			else deleteNode(&array[i - 1], true);
		}
		else if (i == elem)
			//don't know why i need to pass array instead of list
			if (strcmp((*list)->data, array[i - 1]->data) == 0) {
				deleteNode(list, true);
				return;
			}
			else { deleteNode(&array[i - 1], true); return; } //changing our list pointer if it points to a pointer in a NodeArray to delete
}


void sortList(node* _list, bool descending) {
	if (_list == NULL)
		return;
	if (_list->next == _list)
		return;

	node* iter = _list;
	node* jter;
	char* temp;
	do {
		jter = iter->next; //comparing from next after 'iter', to not delete it.
		while (jter != iter) {
			if (descending && strcmp(iter->data, jter->data) > 0) goto swap;
			if (!descending && strcmp(iter->data, jter->data) < 0) goto swap;

			if (false) { 
			swap: 
				temp = iter->data; 
				iter->data = jter->data; 
				jter->data = temp; 
			continue; }

			jter = jter->next;
		}
		iter = iter->next;
	} while (iter != _list); //iterating until we made a cycle in a list
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

//
// other //
//

int nodePointerArrayCount(node** array) {
	int count = 0;
	for (; array[count] != NULL; count++);
	return count;
}

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