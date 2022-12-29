												/* Justin Fairbanks Chapter 3 Project */
												/*		Delimeter Testing Project     */
												/*      Last Updated: 10.30.2022      */

#include <iostream>
#include <string>
#include <fstream> //For File Usage
#include "Stack.h" //Includes my stack header file

using namespace std;


/* Prototypes */
	string testFile(string);
	int lineCount(int, char);
	void manualEntry();

/* Start of Program */
int main(int argc, char* argv[])
{
/* Local Vars */
	fstream errorFile;

	string error;

	string file_path;
	string output_path;


	switch (argc) //According to how many arguments there are in the command line 
	{

/* 1 ARGUMENT */
	
	case 1: //If only exe is ran (or no command line statements)

		manualEntry();
		break;


/* 2 ARGUMENTS */



	case 2: //If an input file is added to test

		file_path = argv[1];

		cout << testFile(file_path); //filepath

	break;


/* 3 ARGUMENTS */

	case 3: //If there is an input file plus an output file entered

		file_path = argv[1]; //File to check

		output_path = argv[2]; //File to write error to

		errorFile.open(output_path);

		while (!errorFile)
		{
			cout << "Output file not found, please try again" << endl;
			getline(cin,output_path);
			errorFile.open(output_path);
		}
		
		error = testFile(file_path); //Writes error to file not console
		errorFile << error; //filepath

		errorFile.close(); //Close File

		break;


	default:
		cout << "Too many command line arguments..." << endl;
		break;
	}

/* Closing of Program */
	cin.clear();
	cout << endl << endl << "Press Enter to Close..." << endl;
	cin.get();

return 0;
}


/* Tests the specified file */
string testFile(string file_path)
{

	/* Local Vars */
		ifstream file; //Declaring file obj
		Stack tempStack; //Temp Stack Var

		int lineNumber = 1; //For keeping track of line number in files
		int tempLine = 0;

		string trash; //For throwing away lines of comments

		string errorCode; //String for storing error code
		string line; //To add to error code

		char read; //Char that reads character from file 

		char temp; //Temp ptr for stack

		bool starPresent; //For telling whether or not star is there for block comment
		bool quotePresent;




	/* Opens Passed File Path */
		file.open(file_path); //Opens up specified file path


	/* Validation of File */

		while (!file)
		{
			cout << endl << endl << "File Path for Test Does Not Exist!" << endl << "Please enter valid file location: " << endl;
			getline(cin, file_path);
			file.open(file_path); //Opens up specified file path
		}




/* FILE TESTING (Delimiter Matching) */

		lineNumber = 1; //Sets line count to 1 to start

		file.get(read); //Reads in first character from file into 'read' char

		lineNumber = lineCount(lineNumber, read); //Increments line count if '\n' read in as char

		while (!file.eof())
		{

				/* Beginning Delimeters */
				if (read == '(')
				{
					tempStack.push(read); //Push to top of stack 

					file.get(read);       //Reads next character from file into 'read' char

					lineNumber = lineCount(lineNumber, read); //Increment line count if read == 'n'
				}
				else if (read == '{')
				{
					tempStack.push(read);

					file.get(read);

					lineNumber = lineCount(lineNumber, read);
				}
				else if (read == '[')
				{
					tempStack.push(read);

					file.get(read);

					lineNumber = lineCount(lineNumber, read);
				}

				/* Closing Delimeters */

				else if (read == ')' || read == ']' || read == '}')
				{
					temp = tempStack.peek(); //Looks at the top of the stack (the most recent pushed character)

					if (tempStack.isEmpty())
					{
						errorCode = "The stack is empty";

						file.close(); // Close File

						return errorCode; // Goes back to main function
					}
					else if (temp == '(' && read == ')')
					{
						tempStack.pop(); //Deletes char from top of stack

						file.get(read); //Reads new char into read var

						lineNumber = lineCount(lineNumber, read); //Increment line count if read == 'n'
					}
					else if (temp == '[' && read == ']')
					{
						tempStack.pop();

						file.get(read);

						lineNumber = lineCount(lineNumber, read);
					}
					else if (temp == '{' && read == '}')
					{
						tempStack.pop();

						file.get(read);

						lineNumber = lineCount(lineNumber, read);
					}
					else
					{
						line = to_string(lineNumber);
						errorCode = "There is no (or wrong) closing brace at line " + line;

						file.close();

						return errorCode;
					}
				}


				/* Comments */
				else if (read == '/')
				{

					file.get(read);

					lineNumber = lineCount(lineNumber, read);

					if (read == '*')
					{

						starPresent = false; //False until final end '*/' for comment found

						while (!file.eof())
						{

							file.get(read);
							lineNumber = lineCount(lineNumber, read);

							if (read == '*')
							{

								file.get(read);
								lineNumber = lineCount(lineNumber, read);

								if (read == '/')
								{

									file.get(read);
									lineNumber = lineCount(lineNumber, read);

									starPresent = true; //Closing '*/' for comment found

									file.get(read);
									lineNumber = lineCount(lineNumber, read);

									break; //Breaks out of end of file loop 
								}
							}
						}


						if (starPresent == false)
						{

							line = to_string(lineNumber);
							errorCode = "You are missing a closing '*/' to your comment on line " + line;

							file.close();
							return errorCode;
						}

					}
					else if (read == '/') //Means its a line comment
					{
						getline(file, trash); //Throws away all of comment
						lineNumber++;
						file.get(read); //Reads character from file into 'read' char
						lineNumber = lineCount(lineNumber, read);
					}

				}

				/* Quotation Validation */

				else if (read == '"') //Checking for end quote
				{
					quotePresent = true;

					tempLine = lineNumber; //Holds the intial " line number, incase next "  never found

					file.get(read);
					lineNumber = lineCount(lineNumber, read);

					while (!file.eof())
					{

						if (read == '"')
						{
							quotePresent = true;

							file.get(read);
							lineNumber = lineCount(lineNumber, read);
							break;
						}
						else
						{
							quotePresent = false;
						}
					}

					if (quotePresent == false)
					{
						line = to_string(tempLine);
						errorCode = "There is no end quote at line " + line;

						file.close();
						return errorCode;
					}
				}

				/* Single Quote Validation */

				else if (read == '\'')
				{

					tempLine = lineNumber; //Holds line number in case ' not found

					file.get(read);
					lineNumber = lineCount(lineNumber, read);
					file.get(read); //Skips 2 characters
					lineNumber = lineCount(lineNumber, read);

					if (read != '\'')
					{
						file.get(read);
						lineNumber = lineCount(lineNumber, read);

						if (read != '\'')
						{
							file.get(read);
							lineNumber = lineCount(lineNumber, read);

							if (read != '\'')
							{

								line = to_string(tempLine);
								errorCode = "Char not recognized on line " + line;

								file.close();
								return errorCode;
							}
						}
						else
						{
							file.get(read);
							lineNumber = lineCount(lineNumber, read);
						}
					}
					else
					{
						file.get(read);
						lineNumber = lineCount(lineNumber, read);

						if (read == '\'')
						{
							file.get(read);
							lineNumber = lineCount(lineNumber, read);
						}
					}

				}
				else
				{

					file.get(read);

					lineNumber = lineCount(lineNumber, read);

					if (read == '\n')
					{
						file.get(read);

						lineNumber = lineCount(lineNumber, read);
					}
				}

		} //While Loop


		if (!tempStack.isEmpty())
		{

			line = to_string(lineNumber);
			errorCode = "There is a mismatching delimeter at line " + line;

			file.close();
			return errorCode;
		}

	/* No Errors */

		else
		{
			errorCode = "There are no mismatch errors found!";
			
			file.close();
			return errorCode;
		}
}


/* Keeps Track of the Line Number */
int lineCount(int lineNum, char current)
{
	/* If New Line */
	if (current == '\n')
		lineNum++;

	return lineNum;
}


/* Manual Entry is defined by user then sent to 'testFile' Function */
void manualEntry()
{
/* Local Vars */
	string file_path;
	string error_file;
	fstream file;
	
/* Header */
	cout << endl << endl << "\t\t=== Manual Entry ===" << endl << endl << endl;

	cout << "Enter the file path:" << endl;
	getline(cin,file_path);


	cout << "Output File (enter 'n' if to console): ";
	getline(cin, error_file);


	if (error_file != "n")
	{
		file.open(error_file);

		while (!file)
		{
			cout << "File path not found...please try again: ";
			cin >> error_file;
			if (error_file == "n")
			{
				cout << testFile(file_path);
				break;
			}
			file.open(error_file);
		}

		file << testFile(file_path);

		file.close();
	}
	else
	{
		cout << endl << endl << testFile(file_path);
	}
}