// Ref class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include <iostream>
#include <string>
using namespace std;

// GetNextToken - general routine for getting next token from a string
// Parameters
//    str - string to search. search is destructive, i.e., token is removed.
//    delimiters - string, a list of characters to use a delimiters between tokens
// Returns: string token (removed from str)

string GetNextToken(string& str, const string& delimiters = " ") {
  // Skip delimiters at beginning                                          
  string::size_type startPos = str.find_first_not_of(delimiters, 0);
  // Find position of delimiter at end of token                                           
  string::size_type endPos = str.find_first_of(delimiters, startPos);

  // Found a token, remove it from string, and return it                       
  string next = str.substr(startPos, endPos - startPos);
  string rest = str.substr(endPos - startPos + 1, string::npos);
  str = rest;
  return(next);
}

// Ref member functions

Ref::Ref() {book = 0; chap = 0; verse = 0;}  	// Default constructor

Ref::Ref(const string s) { // Parse constructor - receives a line "34:5:7 text"
    string rtext = s; // make local copy of string to avoid modifying parameter
    // parse the reference - notice, currently there is no error checking!
    // Get book number
    string strbook = GetNextToken(rtext,":");
    book = atoi(strbook.c_str());
    // Get the chapter number
    string strchap = GetNextToken(rtext,":");
    chap = atoi(strchap.c_str());
    // Get the verse number
    string strverse = GetNextToken(rtext," ");
    verse = atoi(strverse.c_str());
}

Ref::Ref(const int b, const int c, const int v) { 	// Construct Ref from three integers
	book = b;
	chap = c;
	verse = v;
}

// Accessors
int Ref::getBook() const {return book;}	 // Access book number
int Ref::getChap() const {return chap;}	 // Access chapter number
int Ref::getVerse() const {return verse;}; // Access verse number

// TO DO: implement comparison functions
// REQUIRED: == comparison
bool Ref::operator==(const Ref r) {
    if (r.book == book && r.chap == chap && r.verse == verse) {
        return true;
    }
    return false;
}

// != comparison
bool Ref::operator!=(const Ref r) {
    if (r.book == book && r.chap == chap && r.verse == verse) {
        return false;
    }
    return true;
}

// OPTIONAL: define < and > comparisons
bool operator<(const Ref& r1, const Ref& r2) {
    //true if r1.book is greater
    if (r1.getBook() < r2.getBook()) {
        return true;
    }
    //true if r1.book is same and r1.chap is greater
    if (r1.getBook() == r2.getBook() && r1.getChap() < r2.getChap()) {
        return true;
    }
    //true if r1.book and r1.chap are same and r1.verse is greater
    if (r1.getBook() == r2.getBook() && r1.getChap() == r2.getChap() && r1.getVerse() < r2.getVerse()) {
       return true;
    }
    return false;
}

//bool Ref::operator>(const Ref& r2) {
    //true if r1.book is greater
  //  if (book > r2.book) {
    //    return true;
    //}
    //true if r1.book is same and r1.chap is greater
    //if (book == r2.book && chap > r2.chap) {
     //   return true;
    //}
    //true if r1.book and r1.chap are same and r1.verse is greater
    //if (book == r2.book && chap == r2.chap && verse > r2.verse) {
      //  return true;
    //}
    ///return false;
//}

// TO DO: modify display function to show book name instead of book number
void Ref::display() { 	// Display Reference
    string books[66] = {
        "Genesis",
        "Exodus",
        "Leviticus",
        "Numbers",
        "Deuteronomy",
        "Joshua",
        "Judges",
        "Ruth",
        "1 Samuel",
        "2 Samuel",
        "1 Kings",
        "2 Kings",
        "1 Chronicles",
        "2 Chronicles",
        "Ezra",
        "Nehemiah",
        "Esther",
        "Job",
        "Psalms",
        "Proverbs",
        "Ecclesiastes",
        "Song of Solomon",
        "Isaiah",
        "Jeremiah",
        "Lamentations",
        "Ezekiel",
        "Daniel",
        "Hosea",
        "Joel",
        "Amos",
        "Obediah",
        "Jonah",
        "Micah",
        "Nahum",
        "Habakkuk",
        "Zephaniah",
        "Haggai",
        "Zechariah",
        "Malachi",
        "Matthew",
        "Mark",
        "Luke",
        "John",
        "Acts",
        "Romans",
        "1 Corinthians",
        "2 Corinthians",
        "Galatians",
        "Ephesians",
        "Phillipians",
        "Colossians",
        "1 Thessalonians",
        "2 Thessalonians",
        "1 Timothy",
        "2 Timothy",
        "Titus",
        "Philemon",
        "Hebrews",
        "James",
        "1 Peter",
        "2 Peter",
        "1 John",
        "2 John",
        "3 John",
        "Jude",
        "Revelation"
    };
     cout << books[book - 1] << " " << chap;
}

Ref Ref::next() {
    Ref next;
    int nextV = verse + 1;
    next = Ref(book, chap, nextV);
    return (next);
}
