#include "CDLL.h"

int getNum() {
	char* word;
	int num;
	while (true) {
		word = getWord();
		if (atoi(word)) { num = atoi(word); return num; }
		else printf("> ЫWrong value! Enter it again please.\n"
			"Value: ");
	}
}

node* find() {
	node* output = NULL;
	int code;
}

node* getNodes() {
	node* output = NULL;
	int code, push;

	/// INPUT TYPE ///
	printf("> How do you want to input data to list?\n"
		"1 - manually\n" 
		"2 - from file\n"
		"Value: "), code = getNum();

	while (code != 1 && code != 2)
		printf("> Wrong value! Enter it again please.\n"
			"Value: "), code = getNum();
	
	/// PUSH TYPE ///
	printf("> Method of adding nodes to the list:\n"
		"1 - add new to the end (Push Back)\n"
		"2 - add new to the beginning (Push Forward)\n"
		"Value: ");
	push = getNum();
	while (push != 1 && push != 2)
		printf("> Wrong value! Enter it again please.\n"
			"Value: "), push = getNum();

	switch (code) {

	/// MANUAL INPUT ///
	case 1: manual_input:
		printf("> Enter amount of nodes you want to input: "), code = getNum();

		for (; code > 0; code--)
			if (push == 1) pushBack(&output, getWord());
			else pushForward(&output, getWord());

		return output;

	/// FILE INPUT ///
	case 2: file_input:
		printf("> Enter name of the file: ");
		FILE* f = fopen(getWord(), "r");
		if (f)
		{
			output = readFromFile(f, (push == 1) ? true : false); 
			fclose(f);
			printf("Data read successful.\n");
			return output;
		}
		else printf("> File I/O error!\n"
					"0 - Enter data manually."
					"1 - Enter name of the file again.\n");

		while (code!= 0 && code!= 1)
			printf("> Wrong value! Enter it again please.\n"
				"Value: "), code = getNum();
		if (code == 0) goto manual_input;
		else goto file_input;
	}
}
//
// main //
//
int main()
{
	node** foundIters; // for searchForNode();
	node* _list = NULL; // circular double linked list

	char* word = NULL;	// input string
	int num; //input int
	FILE* file; // input/output file

	_list = getNodes();


	printOut(_list);

	return 0;
}

