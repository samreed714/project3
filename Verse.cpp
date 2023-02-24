// Verse class function definitions
// Computer Science, MVNU

#include "Verse.h"
#include <iostream>
using namespace std;

Verse::Verse() {  	// Default constructor
    verseText = "Uninitialized Verse!";
    verseRef = Ref();
} 

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s) {
	// use Ref constructor to create verseRef
    verseRef = Ref(s);
	// store the remainder of the string in verseText

    int erase = 6;  //minimum no. of character to be removed is 6
    if (verseRef.getBook() >= 10) {
        erase++;    //remove another character if book has 2 digits
    }
    if (verseRef.getChap() >= 10) {
        erase++;    //remove another character if chapter has 2 digits
    }
    if (verseRef.getVerse() >= 10) {
        erase++;    //remove another character if verse has 2 digits
    }
    verseText = s;
    verseText.erase(0, erase);
}  	

// REQUIRED: Accessors
string Verse::getVerse() {
    return verseText;
}

Ref Verse::getRef() {
    return verseRef;
}

// display reference and verse
void Verse::display() {
    verseRef.display();
    cout << endl;
    cout << verseRef.getVerse() << " " << verseText;
 }

// display verse only
void Verse::displayText() {
    cout << verseRef.getVerse() << " " << verseText;
}
