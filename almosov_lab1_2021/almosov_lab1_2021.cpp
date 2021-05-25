#include "CDLL.h"

int getNum() {
	char* word;
	int num;
	while (true) {
		word = getWord();
		if (atoi(word) || *word == '0') { num = atoi(word); return num; }
		else printf("> Wrong value! Enter it again please.\n"
			"> Value: ");
	}
}

void find(node** _list) {
	if (*_list == NULL || _list == NULL) {
		printf("> List is empty!\n");
		return;
	}

	int code, upper_border;
	node** found;

	printf("> What node of list do you want to find?\n"
		"> Value: "), found = searchForNode(*_list, getWord());
	printf("> Found values:\n ");
	if (found == NULL || *found == NULL) { printf("| nothing |\n"); return; }
	else nodePointerArrayOutput(found);

	upper_border = nodePointerArrayCount(found);

	printf("> What to do with found nodes?\n"
		"0 - delete one of them.\n"
		"1 - delete all.\n"
		"2 - nothing.\n"), code = getNum();

	while (code != 0 && code != 1 && code != 2)
		printf("> Wrong value! Enter it again please.\n"
			"> Value: "), code = getNum();

	switch (code) {
	case 0:
		printf("> Specify index of node you want to delete.\n"
			"> Value: ");
		code = getNum();
		
		while (code < 1 || code > upper_border)
			printf("> Wrong value! Enter it again please.\n"
				"> Value: "), code = getNum(); 
		deleteNodeByArray(_list, found, code);
		break;

	case 1: deleteNodeByArray(_list, found); break;
	case 2: break;
	default: break;
	}
	delete found;
}

node* input() {
	node* output = NULL;
	int code, push;

	/// INPUT TYPE ///
	printf("> How do you want to input data to list?\n"
		"0 - stop operation.\n"
		"1 - manually.\n" 
		"2 - from file.\n"
		"> Value: "), code = getNum();

	
	while (code != 0 && code != 1 && code != 2)
		printf("> Wrong value! Enter it again please.\n"
			"Value: "), code = getNum();
	if (code == 0) return NULL;

	/// PUSH TYPE ///
	printf("> Method of adding nodes to the list:\n"
		"0 - stop operation.\n"
		"1 - add new to the end (Push Back).\n"
		"2 - add new to the beginning (Push Forward).\n"
		"> Value: "),	push = getNum();
	
	while (push != 0 && push != 1 && push != 2)
		printf("> Wrong value! Enter it again please.\n"
			"> Value: "), push = getNum();
	if (push == 0) return NULL;

	switch (code) {

	/// MANUAL INPUT ///
	case 1:
manual_input:
		printf("> Enter amount of nodes you want to input: "), code = getNum();
		if (code < 0) { printf("> Wrong value! Please enter value again.\n"); goto manual_input; }

		for (; code > 0; code--)
			if (push == 1) pushBack(&output, getWord());
			else pushForward(&output, getWord());

		return output;

	/// FILE INPUT ///
	case 2:
file_input:
		{ //for C2361, to free 'file'
			printf("> Enter name of the file: ");
			FILE* file = fopen(getWord(), "r");
			if (file)
			{
				output = readFromFile(file, (push == 1) ? true : false);
				fclose(file);
				printf("> Data read successful.\n");
				return output;
			}
		}
		printf("> File I/O error!\n"
					"0 - Stop operation."
					"1 - Enter data manually.\n"
					"2 - Enter name of the file again.\n");

		while (code != 0 && code != 1 && code != 2)
			printf("> Wrong value! Enter it again please.\n"
				"> Value: "), code = getNum();

		if (code == 0) return NULL;
		if (code == 1) goto manual_input;
		goto file_input;

	/// DEFAULT ///
	default: return NULL;
	}
}


void output(node* _list) {
	if (_list == NULL) {
		printf("> List is empty!\n");
		return;
	}

	int num;

	printf("> Reverse input?\n"
		"1 - yes.\n"
		"2 - no. \n"
		"> Value: "), num = getNum();

	while (num != 1 && num != 2)
		printf("> Wrong value! Enter it again please.\n"
			"> Value: "), num = getNum();

	printf("> List data:"); outputList(_list, (num == 1) ? true : false);
}

void fileOuput(node * _list) {
	if (_list == NULL) {
		printf("> List is empty!\n");
		return;
	}

	int num = 0;
	int check;
	char* filename;
	FILE* file = NULL;

	// file output //
fout:
	check = 0; //for ftell

	printf("Enter file name:\n"
		"Value: "), filename = getWord();

	file = fopen(filename, "r+"); //needed for ftell work.

	if (!file) file = fopen(filename, "w");
		//checking size of file by looking at the file iterator place ('ftell')
	else fseek(file, 0, SEEK_END), check = ftell(file); 


	//if read successful and file is empty (file iterator is on 0)
	if (file && check == 0)
		write: outputToFile(file, _list), printf("> Data save successful.\n"), num = 0;
	else if (check != 0) {
		printf("> File not empty!\n"
			"0 - stop operation.\n"
			"1 - Enter name of the file again.\n"
			"2 - Override file.\n"
			"Value: "), num = getNum();

		while (num != 0 && num != 1 && num != 2)
			printf("> Wrong value! Enter it again please.\n"
				"> Value: "), num = getNum();
		if (num == 2) {
			fclose(file), fopen(filename, "w");
			goto write;
		}
	}
	else
		printf("> File I/O error!\n"
			"0 - stop operation.\n"
			"1 - Enter name of the file again.\n"
			"Value: "), num = getNum();

	while (num != 0 && num != 1)
		printf("> Wrong value! Enter it again please.\n"
			"> Value: "), num = getNum();

	fclose(file);
	if (num == 0) return;
	if (num == 1) goto fout;
	
	return;
}

void output_menu() {
	printf("\n> Avaliable commands:\n"
		"0 - exit program.\n"
		"1 - output menu again.\n"
		"2 - input data into list.\n"
		"3 - find data in list.\n"
		"4 - delete list.\n"
		"5 - output full list data.\n"
		"6 - delete duplicates from list.\n"
		"7 - sort list.\n"
		"8 - save list to the file.\n"
		"9 - display amount of elements.\n");
}
//
// main //
//
int main()
{
	node* _list = NULL; // circular double linked list
	char* word = NULL;	// input string
	int num; //input int

	output_menu();
menu:
	printf("> Value: ");
	num = getNum();
	switch(num)
	{
	// exit //
	case 0: return 0;

	// output menu //
	case 1: output_menu(); goto menu;

	// input data //
	case 2:
	{
		node* temp;
		temp = input();
		if (temp == NULL) //'input()' outputs NULL if nothing input, but we can already have data in list.
			goto menu;
		_list = temp;
		goto menu;
	}

	// find data //
	case 3: find(&_list); goto menu;

	// delete list //
	case 4: if (_list == NULL) deleteList(&_list); goto menu;

	// output data //
	case 5: output(_list); goto menu;

	// delete duplicates //
	case 6: deleteDuplicates(_list); goto menu;

	// sort list //
	case 7: sortList(_list, false); goto menu;

	// save to file //
	case 8: fileOuput(_list); goto menu;

	// amount of elements //
	case 9: printf("> Amount of elements: %d\n", countElements(_list)); goto menu;

	// err input //
	default:
		printf("> Wrong value! Try again.\n");
		goto menu;
	}
}