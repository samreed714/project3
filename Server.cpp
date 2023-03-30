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

/* Server main line,create name MAP, wait for and serve requests */
int main() {
	LookupResult result;
	string message = "empty";
	Verse verse;
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	webBible.generateMap();

	//make sure map is generated
	if (!webBible.isGenerated()) {
		cout << "error generating map";
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
		Ref ref = Ref(requestStr);

		// Use the Bible object to retrieve the verse by reference
		cout << "Looking up reference: ";
		ref.display();
		cout << ":" << ref.getVerse() << endl;

		verse = webBible.lookup(ref, result);
		cout << "Result status: " << result << endl;

		//Error handling
		if (result != SUCCESS) {
			cout << webBible.error(result) << endl;
			return 0;
		}

		//Send verse through reply pipe
		sendfifo.openwrite();
		message = verse.getVerse();
		sendfifo.send(message);
		sendfifo.fifoclose();

	}
}