
#include <iostream>     //including I/O support
#include <fstream>      //including file I/O support
#include <vector>		//including support of vector container
#include <string>		//including the new C++ string class support
#include <cstring>		//including old C character array support
#include <conio.h>		//including support to use _getch()
						//	to read one char at a time from user input

#include "interface.h"	//
using namespace std;


// 1. Read in a program (lines of statements) from a file
void Interface::loadProgramFromFile()
{
	char fileName[MaxLengthFileName];	
	// this char array stores the name of the file the program reads from.

	cout<<"Name of file to upload program from (ex. file.txt): ";
	cin >> fileName;
	while( strlen(fileName) == 0 )
	{
		cin >> fileName;
	}

	ifstream inFile;
	inFile.open( fileName );
	if ( inFile.fail() )
	{ cout << endl << "Cannot open this file!" << endl;
	return;
	}

	lines.clear();  //Delete all the lines of statements
	string inputBuffer;
	while( !inFile.eof() )
	{
		getline(inFile, inputBuffer);
		while( !inFile.eof() && inputBuffer.size() == 0 )
		{
			getline(inFile, inputBuffer);
		}

		if (!inFile.eof())
		{
  			cout<< "Line["
				<< lines.size()
				<< "] read:"
				<< inputBuffer << endl;
			lines.push_back(inputBuffer);
		}

	}

	inFile.close();
}

// 2. Display the lines of statements
void Interface::displayProgram()
{
	for (int a = 0; a < lines.size(); a++)
	{
		cout << "Line[" << a << "] read:"
			<< lines[a] << endl;
	}
	return;
}

void Interface::LEX_SCANNER()
{

	vectOfTokenVects myVectOfTokenVects;
	vectOfCategoryVects myVectOfCategoryVects;

	LexicalScanner::getLexicalInfo(lines, myVectOfTokenVects, myVectOfCategoryVects);
	LexicalScanner::displayLexicalInfo(myVectOfTokenVects, myVectOfCategoryVects);

	
}

void ExpressionEvaluator::fixings()
{
	cout << "******************************************************************" << endl;
	cout << "Test 1: on the implementation of infix-to-postfix conversion and " << endl
		 << "the postfix-expression evaluation." << endl;
	cout << "******************************************************************" << endl;
	floatVarValueTable varTable;
	string rawExpressionString;
	expVector infixExp, postfixExp;

	while (rawExpressionString != ".")
	{
		float expValue;
		cout << "Give me an infix expression that may have variables s,t,u,v,w,x,y,z in it."
			<< endl << "OR type in an expression of a single period . to quit" << endl << endl;
		getline(cin, rawExpressionString);
		LexicalScanner::getPerLineTokenVectFromOneStringObject
			(rawExpressionString,
			infixExp
			);

		cout << endl
			<< "The infix expression:" << rawExpressionString << endl
			<< "# of tokens in it:" << infixExp.size() << endl
			<< "Token of this infix expression:" << endl;

		for (expVector::iterator pos = infixExp.begin();
			pos != infixExp.end();
			pos++
			)
			cout << '\t' << *pos << endl;

		cout << endl << endl;

		ExpressionEvaluator::infixToPostfixConversion(infixExp, postfixExp);

		cout << "# of tokens in the corresponding postfix expression:"
			<< postfixExp.size() << endl
			<< "Tokens of this postfix expression:" << endl;
		for (expVector::iterator pos = postfixExp.begin();
			pos != postfixExp.end();
			pos++
			)
			cout << '\t' << *pos << endl;

		if (ExpressionEvaluator::postfixEvaluator(postfixExp, varTable, expValue))
			cout << "The value of this expression is " << expValue << endl;
		else
			cout << "This expression is not a valid expression" << endl;
		postfixExp.clear();
	}
}

// 3. Enter new lines of code to the end of the program
void Interface::appendLines()
{
	displayProgram();
	cout << "Add to the program, end with ." << endl;

	while (true)
	{
		//continually adding lines
		cout << "["<<lines.size() << "]";
		lines.resize(lines.size() + 1);
		getline(cin, lines[lines.size() - 1]);
		//checks for end
		if (lines[lines.size() - 1] == ".")
		{
			lines.resize(lines.size() - 1);
			return;
		}
	}
}
// 4. Insert new lines after an existing line
void Interface::insertLines()
{	
	displayProgram(); //show lines to choose from
	//creating vector for user to input 
	vector<string>holder;
	vector<string>::iterator a = lines.begin();
	bool end = false;

	int numEdit;

	cout << "What line do you want to add after? "; 
	cin >> numEdit;
	while (numEdit > lines.size() || numEdit < 0)
	{
		cout << "Put a valid number" << endl;
		cin >> numEdit;
	}

	cout << "Be sure to end with ." << endl;

	cin.ignore();//flushing cin

	while (!end)
	{
		//continually expanding vector for expansive use
		holder.resize(holder.size() + 1);
		getline(cin, holder[holder.size() - 1]);
		//checking for end
		if (holder[holder.size() - 1] == ".")
		{
			holder.resize(holder.size() - 1);
			end = true;
		}
	}

	for (int x = 0; x < holder.size(); x++)
	{
		lines.insert(a + numEdit + 1, holder[x]);
		numEdit++;
	}
	return;
}


// 5. Delete a range of existing line
void Interface::deleteLines()
{
	displayProgram(); // show lines to choose from
	int start, end, b, c = 0;
	int originalSize = lines.size();
	cout << "What line to start at [ includes that line to delete ]: ";
	cin >> start;

	while (start > lines.size() - 1 || start < 0)
	{
		cout << "Try again, number between 0 and " << lines.size() - 1 << endl;
		cin >> start;
	}

	cout << "What line to end at [ includes that line to delete ]: ";
	cin >> end;
	while (end < start || end > lines.size() || end < 0)
	{
		cout << "Try again, number between 0 and " << lines.size() - 1 << endl;
		cin >> end;
	}

	if (start == end && lines.size() == 1)
	{
		lines.clear();
		lines.resize(0);
		return;
	}
	//deletes selected range
	for (int x = start; x <= end; x++)
	{
		cout << "deleting only 1" << endl;
		lines[x].erase();
	}

	//if end of range extends to end of file
		if (end == lines.size() - 1)
		{
			b = originalSize - start;

			lines.resize(originalSize - b);
		}
	//otherwise default case
		else
		{
			end++;
			vector<string> a;
			int sizeofA = originalSize - end;
			int finalSize = start + sizeofA;
			a.resize(sizeofA);

			for (int x = end; x < lines.size(); x++)
			{
				a[c] = lines[x];
				c++;
			}

			lines.resize(start);

			for (int x = 0; x < a.size(); x++)
			{
				lines.push_back(a[x]);
			}

		}
	return;
}


// 6. Replace the contents of an existing line
void Interface::replaceOneLine()
{
	int numEdit;
	char choice;

	displayProgram(); //show lines to choose from

	cout << "Edit line: ";
	cin >> numEdit;

	cout << "Line[" << numEdit << "]" << lines[numEdit] << "\n This is the line you are about to edit! Continue? ( Y / N )\n";
	cin >> choice;

	cin.ignore();

	switch (choice)
	{
	case'y':case'Y':cout << "Enter new line\n"; getline(cin, lines[numEdit]);
		break;
	case'n':case'N':return;
	}
}


// 7. Save program to a file
void Interface::saveProgramIntoFile()
{
	ofstream output;
	string name;

	cout << "What is the name of the file? [ add .txt to end of name ]" << endl;
	cin >> name;
	output.open(name);

	for (int x = 0; x < lines.size(); x++)
	{
		output << lines[x] << endl;
	}

	output.close();
	return;
}

//An auxilliary message displaying function
void Interface::endOfService(const string service)
{
	cout<<endl << service << " done." << endl << "Press any key to continue.";
	_getch();
	cout << endl;
	cout << "****************************************************" <<endl<< endl;

}



// This menu() function is the interface
void Interface::startInterface() {

	map <string, float> ready;
	string name;
	float value;

	string kats;
	ExpressionEvaluator a;
	interpretation b;

	// "Then I suppose now is the time for me to say something profound....
	// Nothing comes to mind. Let's do it."

	bool inMenu = true;	// while the user is using the program this var is set to true and the menu keeps looping.
						// when the user wants to quit, it is set to false and the loop ends.
	char keyPressed;	// This var stores which menu selection the user picks.

	while ( inMenu )
	{
		cout<< endl << endl << endl;
		cout << "****************************************************" << endl;
		cout  <<"**  MENU:(press a character to select an option)  **" << endl;
 		cout << "****************************************************" << endl;
		cout<<"Q. [QUIT]     Quit."<<endl;
		cout<<"L. [LOAD}     Read in a program (lines of statements) from a file"<<endl;
  		cout<<"S. [SAVE]     Save lines of statement as a program into a file"<<endl;
		cout << endl;

		cout<<"D. [DISPLAY]  Display the source code as lines of statements"<<endl;
		cout << endl;

		cout<<"A. [APPEND]   Append new lines to the end of the program"<<endl;
		cout<<"I. [INSERT}   Insert new lines before an existent line"<<endl;
		cout<<"X. [DELETE]   Delete a range of existent lines"<<endl;
		cout<<"R. [REPLACE]  Replace the contents of an existent line"<<endl;
		cout << endl;

		cout<<"P. [PARSE]    Parse and ident the code" << endl;
		cout<<"E. [EXECUTE]  Execute (run) the program"<<endl;
		cout<<"T. [TOGGLE]   Toggle the execution debug mode"<<endl;

		cout << "B. [LEX (LUTHOR) SCANNER] ACTIVE LEX_SCANNER" << endl;
		cout << "F. POSTFIX INFIX FIXINGS" << endl;
		cout << "N. i(N)terpreter" << endl;

		cout << "****************************************************" <<endl<< endl;

		cout<<"Your choice is: ";
		keyPressed = _getch();
		cout<<keyPressed << endl;

		switch(keyPressed) {
		case 'Q': case 'q'://Quit
			cout << "[QUIT]:" << endl;
			inMenu = false;
			break;

		case 'L': case 'l':// 1. Read in a program (lines of statements) from a file
 			cout << "[LOAD PROGRAM]:" << endl;
			loadProgramFromFile();
			endOfService("[LOAD PROGRAM]");
			break;

		case 'D': case 'd':// 2. Display the lines of statements
			cout << "[DISPLAY PROGRAM]:" << endl;
			displayProgram();
			endOfService("[DISPLAY PROGRAM]");
			break;

		case 'A': case 'a':// 3. Append new lines of code to the end
			cout << "[APPEND]:" << endl;
			appendLines();
			endOfService("[APPEND LINES]");
			break;

		case 'I': case 'i':// 4. Insert a new line after an existing line
			cout << "[INSERT LINES]:" << endl;
			insertLines();
			endOfService("[INSERT LINES]");
			break;

		case 'X': case 'x':// 5. Delete an existing line
			cout << "[DELETE LINES]:" << endl;
			deleteLines();
			endOfService("[DELETE LINES]");
			break;

		case 'R': case 'r':// 6. Replace the contents of an existing line
			cout << "[REPLACE]:" << endl;
			replaceOneLine();
			endOfService("[REPLACE ONE LINE]");
			break;

		case 'S': case 's':// 7. Save program to a file
			cout << "[SAVE]:" << endl;
			saveProgramIntoFile();
			endOfService("[SAVE CODE INTO A FILE]");
			break;

		case 'P': case 'p':// 8. Parse and Indent the program
			cout << "[PARSE AND INDENT SOURCE CODE]:" << endl;
			endOfService("[PARSE AND INDENT SOURCE CODE]");
			break;

		case 'E': case 'e':// 9. Run program
			cout << "[EXECUTE TO RUN PROGRAM]:" << endl;
			b.executeProgram(lines);
			endOfService("[EXECUTE TO RUN PROGRAM]");
			break;

		case 'T': case 't':// Toggle the debug mode
			cout << "[TOGGLE the debug mode]:" << endl;
			break;

		case'B':case'b':LEX_SCANNER();
			break;

		case'f':case'F':
			cout << "infix and postfix" << endl;
			a.fixings();
			break;


		case'w':case'W':

			cin >> name;
			cin >> value;
			ready.insert(make_pair(name, value));
			
			endOfService("Ayy Lmao");
			break;
		case'/':
			cin >> kats;
			cout << ready[kats] << endl;

			endOfService("Ayy Lmao");
			break;
		case'n':case'N':b.executeProgram(lines);
			break;

		default:
			cout << "[?? UNKNOWN COMMAND]:" << endl;
			// Unrecognized key, do nothing
			break;
		}

	} // End of while loop.
}
