// Bible class function definitions
// Computer Science, MVNU

#include "Bible.h" 

using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	ifstream stream;
	map<string, int> mp;
	bool isOpen = false;
	bool generated = false;
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
	infile = s;
	ifstream stream;
	map<string, int> mp;
	bool isOpen = false;
	bool generated = false;
}

//Make map
void Bible::generateMap() {
	int position;         /* location of line in the file */
	int count = 0;            /* number of verses added to map*/
	string line;
	Ref currentRef;

	/* open text file */
	stream.open(infile, ios::in);
	/* process text file */
	while (getline(stream, line)) {
		/* Create reference from line*/
		currentRef = Ref(line);
		/* Get the file position at beginning of line */
		position = stream.tellg();
		/* put Ref into map*/
		mp.insert({ currentRef, position - line.length() - 1});
	}
	cout << "\ngenerated map containing " << mp.size() << " references\n";
	cout << "last byte offset: " << mp[currentRef] << endl;
	generated = true;
	stream.close();
}

bool Bible::isGenerated()
{
	return generated;
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
	if (!generated) {
		generateMap();
	}
	if (!stream.is_open()) {
		stream.open(infile, ios::in);
	}

    // TODO: scan the file to retrieve the line that holds ref ...
	string line = "error";
	int pos = mp[ref];
	stream.seekg(pos);

    // update the status variable
	status = SUCCESS; // default
	getline(stream, line);
	
	// error checking
	Ref genesis1_1 = Ref(1, 1, 1);
	if (ref != genesis1_1) {
		if (mp[ref] == 0) {
			status = NO_CHAPTER;
			Ref refErr = Ref(ref.getBook(), ref.getChap(), 1);
			if (mp[refErr] != 0) {
				status = NO_VERSE;
			}
		}
	}


	// create and return the verse object
	Verse aVerse;   // default verse, to be replaced by a Verse object
	                // that is constructed from a line in the file.
	aVerse = Verse(line);
    return(aVerse);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(Ref ref, LookupResult& status) {
	string line;
	Ref currentRef;
	isOpen = stream.is_open();
	if (isOpen) {
		//if the end of the file has not been reached, return the next verse
		getline(stream, line);
		currentRef = Ref(line);
		//print name of new book
		if (currentRef.getBook() > ref.getBook()) {
			cout << endl;
			currentRef.display();
			cout << endl;
		}
		//print name of new chapter
		if (currentRef.getVerse() == 1 && currentRef.getBook() == ref.getBook()) {
			cout << "\nChapter " << currentRef.getChap() << endl;
		}
	}
	else {
		//If the end of the file has been reached, start at the beginning
		currentRef = Ref(1, 1, 1);
		return lookup(currentRef, status);
	}
	Verse aVerse = Verse(line);
	return (aVerse);
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
	if (status == NO_BOOK) {
		return("Error: invalid book");
	}
	if (status == NO_CHAPTER) {
		return ("Error: invalid chapter");
	}
	if (status == NO_VERSE) {
		return ("Error: invalid verse");
	}
	return ("none");
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
//Ref Bible::next(const Ref ref, LookupResult& status) {}

// OPTIONAL: Return the reference before the given ref
//Ref Bible::prev(const Ref ref, LookupResult& status) {}
