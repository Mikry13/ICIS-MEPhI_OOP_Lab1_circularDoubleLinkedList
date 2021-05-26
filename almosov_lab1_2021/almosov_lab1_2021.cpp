#include "CDLL.h"
#include <ctype.h> // isdigit()

//> Interface to get a number from user.
int getNum() {
	char* word;
	
	//atoi() returns '39' if input is 'ab3a...e9' (any combination of characters with these numbers in correct order)
	//so we need to check if string has a character. To do so, we need a buffer char below.
	char buff;

	//atoi() doesnt output negative int, so we need some flag
	bool negative = false;

	int num; //output
	while (true) {
		word = getWord();

		if (*word == '-') negative = true; //if first character is a minus, then it's negative
		else if (!isdigit(*word)) { delete word; goto err; }//if it's not minus and not an int, then it's not a number
		else negative = false;

		buff = word[1]; //placing 2nd character (1st is checked before). I did it because of an algorythm of detecting a literal in string.
		for (char* iter = &word[1]; buff != '\0' && buff != NULL; iter++, buff = *iter) //word from 'getWord()' always end with '\0'
			if (!isdigit(buff)) { delete word; goto err; }

		//"*word == '0'" is needed, because "if('0')" equals "if(false)"
		if (atoi(word) || *word == '0') { num = atoi(word); delete word; return num; }

		err:
		printf("> Wrong value! Enter it again please.\n"
			"> Value: ");
	}
}

//> Interface to find elents in a list.
void find(node** _list) {
	if (*_list == NULL || _list == NULL) {
		printf("> List is empty!\n");
		return;
	}

	int code, upper_border;
	node** found;
	char* word; //to find by key

	printf("> What node of list do you want to find?\n"
		"> Value: "), found = searchForNode(*_list, word = getWord());
	delete word;

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
		deleteNodeByArray(found, _list, code);
		break;

	case 1: deleteNodeByArray(found, _list); break;
	case 2: break;
	default: break;
	}
	delete found;
}

//> Interface to input data to the list.
void input(node** _list) {
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
	if (code == 0) return;

	/// PUSH TYPE ///
	printf("> Method of adding nodes to the list:\n"
		"0 - stop operation.\n"
		"1 - add new to the end (Push Back).\n"
		"2 - add new to the beginning (Push Forward).\n"
		"> Value: "), push = getNum();

	while (push != 0 && push != 1 && push != 2)
		printf("> Wrong value! Enter it again please.\n"
			"> Value: "), push = getNum();
	if (push == 0) return;

	switch (code) {

		/// MANUAL INPUT ///
	case 1:
	manual_input:
		printf("> Enter amount of nodes you want to input: "), code = getNum();
		if (code < 0) { printf("> Wrong value! Please enter value again.\n"); goto manual_input; }

		{char* word = NULL; // for pushBack/Forward
			for (; code > 0; code--)
				if (push == 1) pushBack(_list, word = getWord());
				else pushForward(_list, word = getWord());
			if (!word) delete word;
		}

		return;

		/// FILE INPUT ///
	case 2:
	file_input:
	{ //for C2361, to free 'file'
		char* word = NULL; //for fopen
		printf("> Enter name of the file: ");
		FILE* file = fopen(word = getWord(), "r");
		if (file)
		{
			readFromFile(file, _list, (push == 1) ? true : false);
			fclose(file);
			printf("> Data read successful.\n");
			delete word;
			return;
		}
		delete word;
	}
	printf("> File I/O error!\n"
		"0 - Stop operation.\n"
		"1 - Enter data manually.\n"
		"2 - Enter name of the file again.\n"
		"> Value: "), code = getNum();

	while (code != 0 && code != 1 && code != 2)
		printf("> Wrong value! Enter it again please.\n"
			"> Value: "), code = getNum();

	if (code == 0) return;
	if (code == 1) goto manual_input;
	goto file_input;

	/// DEFAULT ///
	default: return;
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

void outputFile(node* _list) {
	if (_list == NULL) {
		printf("> List is empty!\n");
		return;
	}

	int num = 0;
	int check;
	char* filename; //don't forget to delete allocated string!
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
#pragma warning(disable: 6031)
			fclose(file), fopen(filename, "w");
#pragma warning(default: 6031)
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

#pragma warning(disable: 6387)
	fclose(file);
#pragma warning(default: 6387)

	if (num == 0) return;
	if (num == 1) { delete filename; goto fout; } //deleting already allocated string and enter it again

	return;
}

void output_menu() {
	printf("> Avaliable commands:\n"
		"0 - exit program.\n"
		"1 - output menu again.\n"
		"2 - input data into list.\n"
		"3 - find data in list.\n"
		"4 - delete list.\n"
		"5 - output full list data.\n"
		"6 - delete duplicates from list.\n"
		"7 - sort list.\n"
		"8 - save list to the file.\n"
		"9 - display amount of elements.\n"
		"10 - clear console.\n");
}

//
// main //
//

//> Basic interface for calling functions of the program. Main Function
int main()
{
	node* _list = NULL; // circular double linked list
	char* word = NULL;	// input string
	int num; //input int

	output_menu();
menu:
	printf("> Value: ");
	num = getNum();
	switch (num)
	{
		// exit //
	case 0: return 0;

		// output menu //
	case 1: output_menu(); goto menu;

		// input data //
	case 2: { input(&_list); goto menu; }

		// find data //
	case 3: find(&_list); goto menu;

		// delete list //
	case 4: if (_list != NULL) deleteList(&_list);
		  else printf("List is empty!\n");
		goto menu;

		// output data //
	case 5: output(_list); goto menu;

		// delete duplicates //
	case 6: deleteDuplicates(_list); goto menu;

		// sort list //
	case 7:
		printf("> Descending or Ascending Sort?!\n"
			"0 - Descending.\n"
			"1 - Ascending.\n"
			"Value: "), num = getNum();

		while (num != 0 && num != 1)
			printf("> Wrong value! Enter it again please.\n"
				"> Value: "), num = getNum();

		if (num == 1) sortList(_list, false);
		if (num == 0) sortList(_list, true);
		goto menu;

		// save to file //
	case 8: outputFile(_list); goto menu;

		// amount of elements //
	case 9: printf("> Amount of elements: %d\n", countElements(_list)); goto menu;
	case 10: system("CLS"); output_menu(); goto menu;

		// err input //
	default:
		printf("> Wrong value! Try again.\n");
		output_menu();
		goto menu;
	}
}