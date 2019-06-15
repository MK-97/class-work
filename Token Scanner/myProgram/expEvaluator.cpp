//cont/map1.cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include "lexScanner.h"
#include "expEvaluator.h"
using namespace std;


//*********************************************************************************
//*********************************************************************************
int ExpressionEvaluator::precedenceLevel(const string & opToken)
//return the precedence level of an operator token
{
				if ( opToken == "||" )
					return 0;

				if ( opToken == "&&" )
					return 1;


				if ( opToken == "<" )
					return 2;
				if ( opToken == "==" )
					return 2;
				if ( opToken == ">" )
					return 2;
				if ( opToken == ">=" )
					return 2;
				if ( opToken == "<=" )
					return 2;
				if (opToken == "!=")
					return 2;

				if ( opToken == "+" )
					return 3;
				if ( opToken == "-" )
					return 3;

				if ( opToken == "*" )
					return 4;
				if ( opToken == "/" )
					return 4;
				if (opToken == "%")
					return 4;

				if ( opToken == "!" )
					return 5;

				//Otherwise
				cout << "unknown op:" << opToken << ". precedence level?? " << endl;
				return -1;

}


//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::postfixEvaluator
(	const expVector & postfixExp, 
	floatVarValueTable & varTable, 
	float & expValue 
)
//Evaluate the value of a postfix expression postfixExp
//with respect to the variables and their current values  in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{
	
	vector<float> stack;
	for ( expVector::const_iterator pos= postfixExp.begin();
		  pos != postfixExp.end();
		  pos++
		)
	{
		string token;

		token = *pos;
		if ( token.size()< 1)
		{	
			expValue = 0;
			cout << "Empty Token, not valid." << endl;
			return false;
		}

		if ( LexicalScanner::isNUMERICAL_LITERAL(token) )		
			//A numerical constant, convert it into a float value, and
			//store push it onto the stack
			stack.push_back(	atof( token.c_str() )		);
		else if ( LexicalScanner::isID_NAME(token) )		
			//An ID_NAME, try to look up its current value from the table.
			//If succeeds, push the value onto the stack.
			{
				if (varTable.find(token) == varTable.end())
						return false; //Invalid name
				stack.push_back(	varTable[token]	);
			}
		else if (	LexicalScanner::isNUMERICAL_OP(token)  ||  
					LexicalScanner::isRELATIONAL_OP(token) || 
					LexicalScanner::isLOGICAL_OP(token)
				)
			//A numerical or relational operator
			//Try to pop two operands from the stack apply the operator.
			//If succeeds, push te result back to the stack.
			{
				float op1, op2; 

				//Try to pop op2 first
				if ( !stack.empty() )
					{	op2 = stack.back();
						stack.pop_back();
					}
				else
				{	cout << "No matching op2 for the op: " << token << endl;
					return false; //op2 not available, Not a valid expression
				}

				
				//Determine whether this is a bianry operator or a unary operator
				bool isUnaryOP, isBinaryOP;
				if	(	token == "!"	)	//unary logical negation operator
					{	isUnaryOP = true; isBinaryOP = false;
					}
				else
					{	isUnaryOP = false; isBinaryOP = true;
					}

				if (isUnaryOP) //When the operator is a unary operator
				{	//the only case at this moment: the logical negation operator ! 
					if (token == "!")
						stack.push_back(!op2);
					else
					{		cout << "Unknown unary op: " << token << endl;
							return false;
					}
				} //end the processing of a unary operator

				if (isBinaryOP)  //When the operator is a binary operator
				{
					if ( stack.empty() )
						return false; //op1 not available, Not a valid expression

					op1 = stack.back();
					stack.pop_back();
				
					//Apply the binary operator and push back the result to the stack
					if ( token == "+" )
						stack.push_back(op1+op2);
					else if ( token == "-" )
						stack.push_back(op1-op2);
					else if ( token == "*" )
						stack.push_back(op1*op2);
					else if (token == "/")
						stack.push_back(op1 / op2);
					else if (token == "%")
					{
						int a, b;
						a = static_cast<int>(op1);
						b = static_cast<int>(op2);
						stack.push_back(a % b);
					}
					else if ( token == "<" )
						stack.push_back(op1<op2);
					else if (token == "==")
						stack.push_back(op1 == op2);
					else if (token =="!=")
						stack.push_back(op1 != op2);
					else if ( token == ">" )
						stack.push_back(op1>op2);
					else if ( token == ">=" )
						stack.push_back(op1>=op2);
					else if ( token == "<=" )
						stack.push_back(op1<=op2);

					else if ( token == "&&" )
						stack.push_back(op1&&op2);
					else if ( token == "||" )
						stack.push_back(op1||op2);
					else
						{	cout << "Unkown binary op: " << token << endl;
							return false;
						}
				} //end the processing of a binary operator

			}//end the processing of the case that the token is an operator

		else
			{	cout << "Unkown token type (Not an op or operand): " << token << endl;
				return false;
			}
			// Every token in the expression must be either be a numerical constant, 
			// an ID_NAME or an operator; Otherwise it is NOT a valid expression.

	}//end for


	if (stack.size() == 1)
	//The evaluated value of the expression should be the only value left on the stack
		{	expValue = stack.back();
			return true;
		}
	else
	//Other wise it is not a valis expression.
		return false;

}


//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixToPostfixConversion
(const expVector & infixExp, expVector & postfixExp)
//Convert the infix expression in infixExp into a corresponding postfix expression.
//Store the corresponding postfix expression in postfixExp.
//If infixExp is not a valid infix expression, return false; otherwsie return true.
{
	/*
	only acceptable ID is num_lit, rel_op, num_op, parentheses,
	CHECKING BELOW
	*/
	perLineTokenVector check;
	perLineTokenVector waitingRoom;
	perLineTokenVector doctorsOffice;
	perLineCategoryVector result;
	int hOPs = 0;
	bool noPatients = false;
	bool finalRound = false;
	bool patientPendingforPair = false;
	bool p_Found = false;
	int counter = 0;


	//feed infixExp into perLineTokenVector "check"
	for (int x = 0; x < infixExp.size(); x++)
	{
		check.push_back(infixExp[x]);
	}
	//derive "result" from "check"
	LexicalScanner::getCategoryVectorFromTokenVector(check, result);
	//checking
	for (int x = 0; x < check.size(); x++)
	{
		if (check.size() - x == 1)
			finalRound = true;


		//preliminary checking
		if (
			result[x] != NUMERICAL_LITERAL &&
			result[x] != NUMERICAL_OP &&
			result[x] != RELATIONAL_OP &&
			result[x] != LOGICAL_OP &&
			result[x] != LEFT_PARENTHESIS &&
			result[x] != RIGHT_PARENTHESIS &&
			result[x] != ID_NAME
			)
			return false;
		// counter for parenthesis
		if (result[x] == LEFT_PARENTHESIS)
			counter++;
		if (result[x] == RIGHT_PARENTHESIS)
			counter--;

		//default checking pattern
		if (!finalRound)
		{
			if (result[x] == LOGICAL_OP && result[x + 1] == LOGICAL_OP)
				return false;
			if (result[x] == RELATIONAL_OP && result[x + 1] == RELATIONAL_OP)
				return false;
			if (result[x] == NUMERICAL_OP && result[x + 1] == NUMERICAL_OP)
				return false;
			if (result[x] == LEFT_PARENTHESIS && result[x + 1] == RIGHT_PARENTHESIS)
				return false;
			if (result[x] == ID_NAME && result[x + 1] == ID_NAME)
				return false;
		}
		//during last round of for loop
		else
		{
			if (result[x] == LOGICAL_OP)
				return false;
			if (result[x] == NUMERICAL_OP)
				return false;
			if (result[x] == LEFT_PARENTHESIS)
				return false;
			if (result[x] == RELATIONAL_OP)
				return false;
		}
	}
	//if parenthesis goes unfinished, invalid
	if (counter != 0)
		return false;
	/*




	INFIX TO POSTFIX CONVERSION

		infixExp[x]-> waitingRoom if [x] is an Op
		infixExp[x] -> doctorsOffice if [x] is a NUM_LIT

	*/

	for (int x = 0; x < check.size(); x++)
	{
		//finding parenthesis
		if (result[x] == LEFT_PARENTHESIS)
			p_Found = true;
		else if (result[x] == RIGHT_PARENTHESIS)
		{
			if (p_Found)
				p_Found = false;
		}
		//NUM_LITs get precedence - going straight to doctorsOffice.
		if (result[x] == NUMERICAL_LITERAL || result[x] == ID_NAME)
		{
			doctorsOffice.push_back(check[x]);
			if (patientPendingforPair)
			{
				if (noPatients)
				{
					noPatients = false;
					patientPendingforPair = true;
				}
				else
				{
					patientPendingforPair = false;
				}
			}
			//
			else
			{
				patientPendingforPair = true;
			}
		}
		//Ops gotta go through the waiting room
		else if (result[x] == NUMERICAL_OP || result[x] == LOGICAL_OP || result[x] == RELATIONAL_OP)
		{
			waitingRoom.push_back(check[x]);
			if (ExpressionEvaluator::precedenceLevel(check[x]) >= 4)
				patientPendingforPair = true;
			//checking for parenthesis vs highprecedence_Ops
			if (ExpressionEvaluator::precedenceLevel(check[x]) <=3 && !patientPendingforPair && p_Found)
			{
				doctorsOffice.push_back(waitingRoom.back());
				waitingRoom.pop_back();
				patientPendingforPair = true;
				noPatients = true;
			}
			//checking for highprecedence_Operators
			else if (ExpressionEvaluator::precedenceLevel(check[x]) >= 4 && !patientPendingforPair && !p_Found )
			{
				doctorsOffice.push_back(waitingRoom.back());
				waitingRoom.pop_back();
				patientPendingforPair = true;
				noPatients = true;
				hOPs++;
			}
		}
		//moving lowRank operators to doctorsOffice
		if (!patientPendingforPair && waitingRoom.size() != NULL)
		{
			if (ExpressionEvaluator::precedenceLevel(waitingRoom.back()) >= 4)
			{
				hOPs++;
			}
			doctorsOffice.push_back(waitingRoom.back());
			waitingRoom.pop_back();
			patientPendingforPair = true;
			noPatients = true;
		}

		if (hOPs == 2)
		{
			doctorsOffice.push_back(waitingRoom.back());
			waitingRoom.pop_back();
			hOPs = 0;
		}


	}//end of for loop

	// anything left in the waitingRoom is kicked out into the doctorsOffice

	while (waitingRoom.size() != NULL)
	{
		doctorsOffice.push_back(waitingRoom.back());
		waitingRoom.pop_back();
	}

	for (int x = 0; x < doctorsOffice.size(); x++)
		postfixExp.push_back(doctorsOffice[x]);

	doctorsOffice.clear();
	waitingRoom.clear();
	return true;
}



//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixEvaluator
(	const expVector & infixExp, 
	floatVarValueTable & varTable, 
	float& expValue
)
//Evaluate the value of the infix expression in postfixExp
//with respect to the variables and their current values in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{	expVector postfixExp;
	infixToPostfixConversion(infixExp, postfixExp);
	return postfixEvaluator(postfixExp, varTable, expValue);
}



//*********************************************************************************
//*********************************************************************************
bool ExpressionEvaluator::infixEvaluator	
(	expVector::iterator begin, 
	expVector::iterator end, 
	floatVarValueTable & varTable, 
	float &expValue
)
//Evaluate the value of an infix expression 
//that is embedded within an expression vector 
//The two expVector::iterator iterators: begin and end specify 
//where the infix expression is embedded.
//
//Evaluate the value of an infix expression 
//with respect to the variables and their current values in varTable.
//finally store the value of the expresson in expValue.
//If postfixExp is not a valid postfix expression, return false; otherwsie return true.
{	expVector infixExp(begin,end);
	return infixEvaluator(infixExp, varTable, expValue);
}