											/* Justin Fairbanks - Raster Recursion */
													/* Updated 11.17.2022 */
using namespace std;

#include <iostream>
#include <string>  //For Strings
#include <fstream> //For File Usage


/* Prototypes */

	void readFile(string, string); //Reads ASCII G/T Grid

	void recursion(int**, int, int, int&); //Recursive Function



/* Beginning of Program */
int main(int argc, char* argv[])
{
/* Variables */
	string file_path;
	string output;

	ifstream temp; //For file validation before passed to 'readFile'



	switch (argc) //Argument Count
	{

/* No Arguments */

	case 1: 

		cout << "No Command Line Arguments entered..." << endl;
		cout << "Running Test File" << endl << endl;

		file_path = "Ch5p_fa.asc"; //Hardcoded Test File

		readFile(file_path, "NULL"); //Test file_path & ouputs to console

		break;

/* Input File Given in Command Line */

	case 2:

		file_path = argv[1];


	/* Input File Validation */

		temp.open(file_path);

		while (!temp)
		{
			cout << "Input file not found, please try again: " << endl;
			getline(cin, file_path);
			temp.open(file_path);
		}

		temp.close();

		output = "NULL"; //No output file given

	/* Read File Function - Leads to Recursive Function */

		readFile(file_path, output); //Function to read file into int 2D arr 
									//and output original input to console

		break;


/* Input and Output File Given in Command Line */

	case 3:

		file_path = argv[1];

		output = argv[2];

	/* Input File Validation */

		temp.open(file_path); //Open Temp Input File

		while (!temp)
		{
			cout << "Input file not found, please try again: " << endl;
			getline(cin, file_path);
			temp.open(file_path);
		}

		temp.close(); //Close Temp Input File



	/* Output File Validation */

		temp.open(output); //Open Temp Output File

		while (!temp)
		{
			cout << "Output file not found, please try again: " << endl;
			getline(cin, output);
			temp.open(output);
		}

		temp.close(); //Close Temp Output File


	/* Read File Function - Leads to Recursive Function */
		readFile(file_path, output); //Function for reading the input file and will write to output file

		cout << "Output wrote to: " << output << endl;

		break;


/* Too many command line args entered */

	default:

		cout << endl << "Too Many Command Line Arguments!" << endl;

		break;
	}


/* Closing of Program */

	cin.clear();
	cout << endl << endl << "Press Enter to Close..." << endl;
	cin.get();

	return 0;
}

/* Reads File and Translates into int Array */
/* Then sent to recursion function to find possible clusters */
void readFile(string in, string out)
{
/* Local Vars */
	ifstream input;
	ofstream output;

	int r = 0, c = 0; //Grid size given at top of file

	char ch; //Char used for reading file




/* Open Input File */
	input.open(in);

/* Open Output File */
	if (out != "NULL")
		output.open(out);


/* Reads in grid size at top of file */

	input.get(ch);

	while (ch != '=')
	{
		input.get(ch);
	}

	input >> r; //Gets the number of rows at the top of the file

	input.get(ch); //Reads in next char

	while (ch != '=')
	{
		input.get(ch);
	}

	input >> c; //Gets the number of columns at the top of the file


/* New Int 2D Dynamically Allocated Array */

	int** clusters = new int* [r]; //Dynamically allocates the rows

	for (int i = 0; i < r; i++) //Dynamically allocates each column off of the rows 
		clusters[i] = new int[c];

	input.get(ch); //Reads in new line of file


/* Load File into 2D Array */

			
	/* Only outputs inputted file if output is going to console */
	if (out == "NULL")
		cout << "== Inputted File ==" << endl << endl;

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			do
			{
				input.get(ch);
			} while (ch == '\n' || ch == ' ');

			if (out == "NULL")
				cout << ch << " "; //Outputs original file contents

			if (ch == 'g')
			{
				clusters[i][j] = 0; //Loads 'g' points to int arr as 0s 
			}
			else
			{
				clusters[i][j] = -1; //Loads 't' points to int arr as -1s 
			}
		}
		if (out == "NULL")
			cout << endl; //If new row of the grid
	}

/* File Size Outputted */

	int size = 0; //Used for multiplying rows and columns

	if (out == "NULL") //If no output file given
	{
		cout << endl << "Rows: " << r << endl; //r is number of rows read from file 
		cout << "Columns: " << c << endl; //c is number of columns read from file

		size = c * r;

		cout << "Characters in File: " << size << endl;
	}

	else
	{
		output << endl << "Rows: " << r << endl; //r is number of rows read from file 
		output << "Columns: " << c << endl; //c is number of columns read from file

		size = c * r;

		output << "Characters in File: " << size << endl;
	}


/* Tree Clusters */



	/* Var for Recursion Count */
	int recur = 0;


	/* Iterating through file until Tree Found */
	/* When Tree found enters recursion for possible cluster */

	for (int x = 1; x < r; x++)
	{
		for (int y = 1; y < c; y++)              
		{

			if (clusters[x][y] == -1) //If tree found searches for cluster
			{
				recur++; //increments recursive counter
				recursion(clusters, x, y, recur);	//arr passed and x and y for array incrementers
			}
		}
	}



	/* Outputting the number of tree clusters (stands) */

	if (out == "NULL")
		cout << endl << endl << "== Tree Clusters ==" << endl << endl;
	else 
		output << endl << endl << "== Tree Clusters ==" << endl << endl;

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			if (out == "NULL")
				cout << clusters[i][j] << " ";
			else
				output << clusters[i][j] << " ";
		}
		if (out == "NULL")
			cout << endl;
		else
			output << endl;
	}


/* Deletes Allocated Array */

	delete[] clusters;

	/* Closes Both Files */
	input.close();

	if (out != "NULL")
		output.close();

}


/* Recursive Function */
void recursion(int** clusters, int i, int j, int& recur) //i - row incrementor j - column incrementor
{
/* Temp Vars */
	bool tree = false;


/* Above Cell Check for Tree */


	if (clusters[i - 1][j] == -1) //Checks above cell
	{
		clusters[i][j] = recur;
		clusters[i - 1][j] = recur;

		tree = true;
		i--;
		recursion(clusters, i, j, recur);

	}


/* Below Cell Check for Tree */


	if (clusters[i + 1][j] == -1) //Checks below cell
	{
		clusters[i][j] = recur;
		clusters[i + 1][j] = recur;

		tree = true;
		i++;
		recursion(clusters, i, j, recur);
	}


/* Left of Cell Check for Tree */


	if (clusters[i][j - 1] == -1) //Checks to left of cell
	{
		clusters[i][j] = recur;
		clusters[i][j - 1] = recur;

		tree = true;
		j--;
		recursion(clusters, i, j, recur);

	}



/* Right of Cell Check for Tree */


	if (clusters[i][j + 1] == -1) //Checks to right of cell
	{
		clusters[i][j] = recur;
		clusters[i][j + 1] = recur;

		tree = true;
		j++;
		recursion(clusters, i, j, recur);
	}


	if (clusters[i][j] == -1 && tree == false) //Tree by itself..not in cluster
	{
		clusters[i][j] = recur;
		return;
	}
}