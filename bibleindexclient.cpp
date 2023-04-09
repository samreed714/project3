/*
* The cgi.getElement function parses the input string, searching for the matching
* field name, and returing a "form_iterator" oject, which contains the actual
* string the user entered into the corresponding field.The actual values can be
* accessed by dereferencing the form iterator twice, e.g.** verse
* refers to the actual string entered in the form's "verse" field.
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "fifo.h"
#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <fstream>
#include <string> 
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <list>
#include "fifo.h"
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

#define logging // enable log file
#define LOG_FILENAME "/home/class/csc3004/tmp/samreed-indexclient.log"
#include "logfile.h"

string getRefFromMessage(const string message) {
    string messageCopy = message;
    string delim = "&&";
    //parse lookupStr for reference
    string refStr = messageCopy.substr(0, messageCopy.find(delim));
    //remove reference and delim from lookupStr
    messageCopy.erase(0, messageCopy.find(delim) + delim.length());
    log("Got reference: " + refStr);
    return refStr;
}

string getVerseFromMessage(string message) {
    string delim = "&&";
    string messageCopy = message;

    //parse lookupStr for reference
    string refStr = messageCopy.substr(0, messageCopy.find(delim));
    //remove reference and delim from lookupStr
    messageCopy.erase(0, messageCopy.find(delim) + delim.length());
    log("Removed reference from message: " + refStr);
    log("Remaining string to be parsed: " + messageCopy);

    //parse lookupStr for verse text 
    string verseStr = refStr + messageCopy.substr(0, messageCopy.find(delim));
    //remove verse text and delim
    messageCopy.erase(0, messageCopy.find(delim) + delim.length());
    log("Got verse text: " + verseStr);
    return verseStr;
}

string getStatusFromMessage(string message) {
    string delim = "&&";
    string messageCopy = message;

    //parse lookupStr for reference
    string refStr = messageCopy.substr(0, messageCopy.find(delim));
    //remove reference and delim from lookupStr
    messageCopy.erase(0, messageCopy.find(delim) + delim.length());
    log("removed reference from string: " + refStr);
    log("Remaining string to be parsed: " + message);

    //parse lookupStr for verse text 
    string verseStr = refStr + messageCopy.substr(0, messageCopy.find(delim));
    //remove verse text and delim
    messageCopy.erase(0, messageCopy.find(delim) + delim.length());
    log("removed verse text from string: " + verseStr);
    log("Remaining to be parsed: " + messageCopy);

    //parse lookupStr for status
    string statusStr = messageCopy.substr(0, messageCopy.find(delim));
    log("Got status: " + statusStr);
    return statusStr;
}


int main() {
    /* A CGI program must send a response header with content type
     * back to the web client before any other output.
     * For an AJAX request, our response is not a complete HTML document,
     * so the response type is just plain text to insert into the web page.
     */
    cout << "Content-Type: text/plain\n\n";

    Cgicc cgi;  // create object used to access CGI request data

    // GET THE INPUT DATA
    // browser sends us a string of field name/value pairs from HTML form
    // retrieve the value for each appropriate field name
    form_iterator st = cgi.getElement("search_type");
    form_iterator book = cgi.getElement("book");
    form_iterator chapter = cgi.getElement("chapter");
    form_iterator verse = cgi.getElement("verse");
    form_iterator nv = cgi.getElement("num_verse");
    form_iterator version = cgi.getElement("version");

    #ifdef logging
    logFile.open(LOG_FILENAME, ios::out);
    #endif

    // Convert and check input data
    bool validInput = false;
    string errorMessage = "Invalid Chapter";
    string errorDesc = " ";
    if (chapter != cgi.getElements().end()) {
        int chapterNum = chapter->getIntegerValue();
        if (chapterNum > 150) {
            //cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
            errorDesc = string("The chapter number ") + to_string(chapterNum) + string(" is too high.");
        }
        else if (chapterNum <= 0) {
            //cout << "<p>The chapter must be a positive number.</p>" << endl;
            errorDesc = "The chapter must be a positive number";
        }
        else
            validInput = true;
    }

    /* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */
    //Book should be greater than 0 and less than 66
    int b = book->getIntegerValue();
    if (b > 66) {
        validInput = false;
        errorMessage = "Invalid Book";
        //cout << "<p>There are only 66 books of the Bible.</p>";
        errorDesc = "There are only 66 books of the Bible.";
    }

    //already checked
    int c = chapter->getIntegerValue();

    //Verse should be greater than 0 and less than 176
    int v = verse->getIntegerValue();
    if (v > 176) {
        validInput = false;
        errorMessage = "Invalid Verse";
        //cout << "<p>No chapters of the Bible have more than 176 verses.</p>";
        errorDesc = "No chapters of the Bible have more than 176 verses";
    }
    if (v < 1) {
        validInput = false;
        errorMessage = "Invalid Verse";
        //cout << "<p>Verse must be greater than 0</p>";
        errorDesc = "Verse must be greater than 0";
    }

    //Number of verses should not be less than 1. If it is, only print 1 verse
    int v2 = nv->getIntegerValue();
    if (v2 < 1) {
        cout << "<p style=\"color:Red;\">Number of verses should be 1 or more. Printing 1 verse: </p>" << endl;
    }

    //Version number should 1-5
    int versionInt = version->getIntegerValue();
    if (versionInt > 5) {
        cout << "<p style=\"color:Red;\">There are only 5 versions. Using WEB version</p>";
    }
    if (versionInt < 1) {
        cout << "<p style=\"color:Red;\">Version cannot be less than 0. Using WEB version</p>";
    }

    /* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
     *        TO LOOK UP THE REQUESTED VERSES
     */

    LookupResult result;

    string bookNames[66] = {
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
    string versionNames[5] = {
        "WEB",
        "DBY",
        "KJV",
        "WBSTR",
        "YLT"
    };

    // Create a reference from the numbers
    Ref ref(b, c, v);
    //Ref currentRef = ref.next();

    //pipes
    string receive_pipe = "indexRequest";
    string send_pipe = "indexReply";

    //recfifo format should be book:chapter:verse
    Fifo recfifo(receive_pipe);
    //sendfifo will send reference&&verse text&&status
    Fifo sendfifo(send_pipe);
    //Send reference through recfifo
    recfifo.openwrite();
    log("opened receive pipe");
    string requestStr = to_string(ref.getBook()) + ":" + to_string(ref.getChap()) + ":" + to_string(ref.getVerse());
    recfifo.send(requestStr);
    log("sent: " + requestStr + " through request pipe");

    //receive reference, verse text, and status
    sendfifo.openread();
    log("opened send_pipe");
    string lookupStr = sendfifo.recv();
    log("received: " + lookupStr + " from send pipe");

    //close pipes
    recfifo.fifoclose();
    sendfifo.fifoclose();
    
    //parse lookupStr using methods
    string refStr = getRefFromMessage(lookupStr);
    string verseStr = getVerseFromMessage(lookupStr);
    string statusStr = getStatusFromMessage(lookupStr);

    //create Ref and Verse objects from parsed strings
    Ref lookedUpRef = Ref(refStr);
    log("Created reference from refStr: " + refStr);
    Verse lookedUpVerse = Verse(verseStr);
    log("Created verse from verseStr: " + lookedUpVerse.getVerse());

    //convert statusStr to integer and assign status
    switch (atoi(statusStr.c_str())) {
    case 0:
        result = SUCCESS;
        log("Assigned result to SUCCESS because statusStr was 0");
        break;
    case 1:
        result = NO_BOOK;
        log("Assigned result to NO_BOOK because statusStr was 1");
        break;
    case 2:
        result = NO_CHAPTER;
        log("Assigned result to NO_CHAPTER because statusStr was 2");
        break;
    case 3:
        result = NO_VERSE;
        log("Assigned result to NO_VERSE because statusStr was 3");
        break;
    }
    //Verse currentVerse;

    /* SEND BACK THE RESULTS
     * Finally we send the result back to the client on the standard output stream
     * in HTML text format.
     * This string will be inserted as is inside a container on the web page,
     * so we must include HTML formatting commands to make things look presentable!
     */

     //check for errors from lookup
    if (result != SUCCESS) {
        if (result == NO_BOOK) {
            errorMessage = "Invalid Book";
        }
        if (result == NO_CHAPTER) {
            errorMessage = "Invalid Chapter";
        }
        if (result == NO_VERSE) {
            errorMessage = "Invalid Verse";
        }
        validInput = false;
    }
    if (validInput) {
        cout << "Search Type: <b>" << **st << "</b>" << endl;
        cout << "<p>Your result: "
            << bookNames[b - 1] << " " << **chapter << ":" << **verse << " (" << versionNames[versionInt - 1] << "): "
            << "<em>" << lookedUpVerse.getVerse() << "</em>";

        //variables for keeping track of when the book and chapter change
        int currentBook = b;
        int currentChap = c;
        //Print the rest of the verses. i = 1 becase one verse has already been displayed
        for (int i = 1; i < v2; i++) {
            //Send reference through recfifo
            recfifo.openwrite();
            log("opened receive pipe");
            requestStr = "next";
            recfifo.send(requestStr);
            log("sent: " + requestStr + " through request pipe");

            //receive reference, verse text, and status
            sendfifo.openread();
            log("opened send_pipe");
            lookupStr = sendfifo.recv();
            log("received: " + lookupStr + " from send pipe");

            //parse lookupStr using methods
            string refStr = getRefFromMessage(lookupStr);
            string verseStr = getVerseFromMessage(lookupStr);
            string statusStr = getStatusFromMessage(lookupStr);

            lookedUpRef = Ref(refStr);
            lookedUpVerse = Verse(verseStr);
            
            //print name of book if book changes
            if (lookedUpRef.getBook() > currentBook) {
                cout << "<b> " << bookNames[lookedUpRef.getBook() - 1] << ": </b>";
                currentBook = lookedUpRef.getBook();
                log("printed new book:" + bookNames[lookedUpRef.getBook() - 1]);
            }

            //print chapter if chapter changes
            if (lookedUpRef.getChap() > currentChap) {
                cout << "<b> Chapter " << lookedUpRef.getChap() << ": </b>";
                currentChap = lookedUpRef.getChap();
                log("printed new chapter: " + lookedUpRef.getChap());
            }

            cout << " <sup><b>" << lookedUpRef.getVerse() << "</sup></b>";

            cout << "<em> " << lookedUpVerse.getVerse() << "</em>";
            
            recfifo.fifoclose();
            sendfifo.fifoclose();
        }
        cout << "</p>" << endl;

    }
    else {
        cout << "<p style=\"color:Red;\">Error: <em>" << errorMessage << "</em></p>" << endl;
        cout << "<p style=\"color:Red;\">Description: <em>" << errorDesc << "</em></p>" << endl;
    }
    return 0;
}
