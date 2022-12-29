/*
 * Project3.cpp
 *	Collaborators: Nora, Loki
 *  Sep 15, 2013
 *  copyright Thor the dog
 */


int main() {
    s=t[5]+u/(v*(w+yi]));   /* removed 2nd open square bracket */

	for (int i = 0; i < allLines.size(); i++) { //Then any duplicate lines are erased for simplicity
		if (i != allLines.size() - 1 && allLines[i].compare(allLines[i + 1]) == 0) { //The tested line must not be the last line of the series or else an error occurs when it tries to test the string after it that does not exist
			allLines[i].erase();
		}
	}

	return 0;
}
