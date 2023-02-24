// Class Ref
// Computer Science, MVNU
//
// Ref is a class for representing a Bible reference consisting of
//    * integer book   - the book from 1 (Genesis) to 66 (Revalation)
//    * integer chap   - the chapter number >1
//    * integer verse  - he verse numner >1
//

#ifndef Ref_H
#define Ref_H
#include <string> 
#include <stdlib.h>
using namespace std;

// GetNextToken returns a token from str, which contains all characters
// up to the first character from delimiters
string GetNextToken(string& str, const string& delimiters);

class Ref {
private:
   short book, chap, verse;	// Reference information
public:
   Ref();  	// Default constructor
   Ref(string s); 	// Parse constructor - example parameter "43:3:16"
   Ref(const int,const int,const int); 	// Construct from three integers;
   // Accessors
   int getBook() const;	// Access book number
   int getChap() const;	// Access chapter number
   int getVerse() const;	// Access verse number
   // REQUIRED Comparison: determine if two references are equal
   bool operator==(const Ref);
   bool operator!=(const Ref);
   // OPTIONAL: < and > comparisons for Ref objects might also be useful

   //bool operator>(const Ref& r2);
   // REQUIRED: Display the reference on cout, example output: John 3:16
   void display();
   // Your version of display should show the book name
   // corresponding to the stored book number.
   Ref next();	//gives the reference that comes after the current reference
};
bool operator<(const Ref& r1, const Ref& r2);
#endif //Ref_H
