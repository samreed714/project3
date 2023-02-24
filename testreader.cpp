// testreader.cpp
// main function for Project 1

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main (int argc, char **argv) {
	if (argc < 4) {
		cerr << "Usage: " << argv[0] << " [Book Number] [Chapter Number] [Verse Number] \n" 
			<< "[optional: Number of Verses to Display]" << endl;
		exit(2);
	}
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	
	Verse verse;
	int b, c, v, v2;

	LookupResult result;
	bool multiple = (argc == 5);	//true if more than one verse is being looked up
	cout << "Using Bible from: ";
	webBible.display();
	// TODO: your final program should get input from command line arguments instead.
	b = atoi(argv[1]);
	c = atoi(argv[2]);
	v = atoi(argv[3]);
	if (multiple) {
		v2 = atoi(argv[4]);
	}

	// Create a reference from the numbers
	Ref ref(b, c, v);

	if (ref.getBook() > 66 || ref.getBook() < 1) {
		cerr << "Error: Book must be between 1 and 66" << endl;
		return 0;
	}

	// Use the Bible object to retrieve the verse by reference
	cout << "Looking up reference: ";
	ref.display();
	cout << ":" << ref.getVerse();
	
	verse = webBible.lookup(ref, result);
	cout << endl << "Result status: " << result << endl;
	
	//Error handling
	if (result != SUCCESS) {
		cout << webBible.error(result) << endl;
		return 0;
	}
	verse.display();

	//Print the rest of the required verses if necessarry
	Ref currentRef = ref.next();
	Verse currentVerse;
		if (multiple) {			
			for (int i = 1; i < v2; i++) { // i = 1 becase one verse has already been displayed
				currentVerse = webBible.nextVerse(currentRef, result);
				currentVerse.displayText();
				currentRef = currentRef.next();
			}
		}
	cout << endl;
}
