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
using namespace std;

string receive_pipe = "indexRequest";
string send_pipe = "indexReply";

#define logging // enable log file
#define LOG_FILENAME "/home/class/csc3004/tmp/samreed-indexclient.log"
#include "logfile.h"

/* Server main line,create name MAP, wait for and serve requests */
int main() {
#ifdef logging
	logFile.open(LOG_FILENAME, ios::out);
#endif

	LookupResult result;
	string message = "empty";
	Verse verse;
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	webBible.generateMap();

	//make sure map is generated
	if (!webBible.isGenerated()) {
		cout << "error generating map" << endl;
		log("error generating map");
		exit(1);
	}

	/* Create the communication fifos */

	//recfifo format should be book:chapter:verse
	Fifo recfifo(receive_pipe);
	//sendfifo will be in the format book:chapter:verse&&verse text&&Status
	Fifo sendfifo(send_pipe);

	recfifo.openread(); // only open once

	while (true) {
		/* Get a message from a client */
		string requestStr = recfifo.recv();
		Ref ref;

		if (requestStr == "next") {
			cout << "getting next verse" << endl;
			log("getting next verse");
			ref = ref.next();
			log("ref is now: " + to_string(ref.getBook()) + ":" + to_string(ref.getChap()) + ":" + to_string(ref.getVerse()));
			cout << "ref is now: " << to_string(ref.getBook()) << ":" << to_string(ref.getChap()) << ":" << to_string(ref.getVerse()) << endl;
			verse = webBible.nextVerse(ref, result);
			cout << "looked up successfully" << endl;

			Ref refFromVerse = verse.getRef();

			message = to_string(refFromVerse.getBook()) + ":" + to_string(refFromVerse.getChap()) + ":" + to_string(refFromVerse.getVerse() - 1);
			message += "&& " + verse.getVerse() + "&&" + to_string(result);

			cout << "sending message: " << message << endl;
			//Send message through reply pipe
			sendfifo.openwrite();
			sendfifo.send(message);
			sendfifo.fifoclose();

			//skip the rest of the loop
			continue;
		}

		ref = Ref(requestStr);

		//Add reference to message with delimiter
		message = requestStr + "&& ";

		// Use the Bible object to retrieve the verse by reference
		cout << "Looking up reference: ";
		log("looking up reference:" + requestStr);
		ref.display();
		cout << ":" << ref.getVerse() << endl;

		verse = webBible.lookup(ref, result);
		cout << "Result status: " << result << endl;
		log("Result status: " + result);

		//Error handling
		if (result != SUCCESS) {
			cout << webBible.error(result) << endl;
			log(webBible.error(result));
		}

		//Add verse text and status to reply pipe if there were no errors
		if (result == SUCCESS) {
			message += verse.getVerse() + "&&" + to_string(result);
		}
		else {
			message += "error" + string("&&") + to_string(result);
		}

		cout << "sent message: " << message << endl;
		log("sent message: " + message);

		//Send message through reply pipe
		sendfifo.openwrite();
		sendfifo.send(message);
		sendfifo.fifoclose();

	}
}