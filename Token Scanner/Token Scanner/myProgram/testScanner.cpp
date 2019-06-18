
// In testScanner.cpp, we demonstrate how you can use the static methods of
// the LxxicalScanner class to (i) break a statement into basic 
// components (tokens) and determine their categories, and 
// (ii)display such lexical information 




#include <iostream>  // for basic iostream operations
#include <cstring>   // for basic C-style character arrayt manipulation
#include "lexScanner.h"
using namespace std;



void main()
{
 //Set up a vector of strings to store a source program of 3 lines


 vector<string> myLinesOfStatements;
 myLinesOfStatements.push_back("X1=if(x  <=y) X1=X1-2;");
 myLinesOfStatements.push_back("if(a<=b) x==x");
 myLinesOfStatements.push_back("while (X1== (X2-4) )X2 = X1 % a; //test");

 cout << endl << "A program with 3 lines of statements." << endl;
 for (size_t i=0; i< myLinesOfStatements.size(); i++)
	 cout	<<	"Line["
			<<	i
			<<	"].\t"
			<<	myLinesOfStatements[i]
			<<	endl;

 cout << endl << "Lexical analysis of the program line by line." << endl << endl;


 //Set up a vector of token vectors and
 //	a vector of category vectors to store the lexical information
 vectOfTokenVects myVectOfTokenVects;
 vectOfCategoryVects myVectOfCategoryVects;

 //Get the lexical information 
 LexicalScanner::getLexicalInfo(
							myLinesOfStatements, 
							myVectOfTokenVects, 
							myVectOfCategoryVects);

 //Display the lexical information 
 LexicalScanner::displayLexicalInfo(
							myVectOfTokenVects, 
							myVectOfCategoryVects);



 //Ask the user to type in another source program for analysis

 cout	<< endl
		<< "********************************************************************"
		<<endl;
 cout	<< endl << "Enter a program (as lines of statements), one line at a time," 
		<< endl	<< "with a line of single . to signal the end of the program input."
		<< endl;
 string buffer = " ";

 myLinesOfStatements.clear();
 myVectOfTokenVects.clear();
 myVectOfCategoryVects.clear();
 while (buffer != ".")
 {
	 getline(cin, buffer);
	 if (buffer.size() > 0 && buffer != ".")
		myLinesOfStatements.push_back(buffer);
 }

 for (size_t  i=0; i< myLinesOfStatements.size(); i++)
	 cout	<<	"Line["
			<<	i
			<<	"].\t"
			<<	myLinesOfStatements[i]
			<<	endl;

 cout << endl << "Lexical analysis of the program line by line." << endl << endl;


 //Get the lexical information 
 LexicalScanner::getLexicalInfo(
							myLinesOfStatements, 
							myVectOfTokenVects, 
							myVectOfCategoryVects);

 //Display the lexical information 
 LexicalScanner::displayLexicalInfo(
							myVectOfTokenVects, 
							myVectOfCategoryVects);

 cout << "Press Q to quit " ;
 char quit;
 cin >> quit;
}

