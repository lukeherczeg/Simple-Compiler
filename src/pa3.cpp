#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ctype.h>
#include <map>
#include "pa3.h"


bool isUpperCaseCharacter(char character){ // Checks if a character is within range of upper case characters.
	return (character <= 'Z' && character >= 'A') ? true : false;
}

bool isLowerCaseCharacter(char character){ // Checks if a character is within range of lower case characters.
	return (character <= 'z' && character >= 'a') ? true : false;
}

void giveError(std::vector<std::string> & syntaxErrors, std::string possibleError, std::string c){ // Pushes error to a vector and prints it.
	possibleError = c; 					   // Make the possibleError the one given as c.
	syntaxErrors.push_back(possibleError); // Add the error to the syntaxErrors vector.
	std::cout << possibleError << " ";	   // Print the error.
}

template <class T> bool alreadyContains(const T &value, const std::vector<T> &array) // Uses an <algorithm> function find() to check if a vector contains an element already.
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

////////////////////////////////////////////////////////////////////////////// SIMPLE STACK TEMPLATE DEFINITIONS

template <class T> bool Stack<T>::isEmpty(){
	return stackList.empty();
}

template <class T> void Stack<T>::push(T item){
	stackList.push_back(item);
}

template <class T> void Stack<T>::pop(){
	stackList.pop_back();
}

template <class T> void Stack<T>::clear(){
	while(!stackList.empty()){
		stackList.pop_back();
	}
}

template <class T> T Stack<T>::peek(){
	if(!stackList.empty())
		return stackList.back();
	return "";
}

////////////////////////////////////////////////////////////////////////////// SIMPLE STACK TEMPLATE DEFINITIONS

int Compiler::getLoopDepth(){ // A complex method to return the number of nested loops.
	int loopsReached = 1;
	int count = 0;
	int beginCount = 0;
	int forCount = 0;
	bool rightParenthError = false;
	bool sameLoop = false;
	bool hasParentheses = false;
	bool isFirstLoop = true;
	std::vector<Stack<std::string>> loops;  // Instantiate a vector of stacks of strings that will hold the loops and their keywords.

	for(std::string word : this->words){  // Loop through all the words in the file.
		if(word == "FOR" && isFirstLoop){ // First FOR creates the first loop stack and pushes FOR to it.
			forCount++;
			loops.push_back(Stack<std::string>());
			loops[count].push(word);
			isFirstLoop = false;
		}
		else if(word == "FOR"){ 				 // If it isn't the first loop and FOR is found,
			forCount++;			 	 			 // increase the count of FOR.
			count++;			 				 // Increase the temporary counter.
			hasParentheses = false;  			 // Reset parentheses checkers.
			rightParenthError = false;
			this->stackChar.clear();			 // Clear the stack that holds parentheses.
			if(loops[count-1].peek() == "FOR"){
				forCount = 0; beginCount = 0; loopsReached = count - 1; // If a FOR is encountered twice in a row approaching the
				break;													// next loop, that will be the breakpoint, and we will
			}															// return the number of loops reached.
			else if(loops[count-1].peek() == "BEGIN"){
				loops.push_back(Stack<std::string>());  // If BEGIN is found after FOR, that is good, we can start a new loop
				loops[count].push(word);				// and push BEGIN to the now-previous stack.
				loopsReached++;
			}
			else if(loops[count-1].peek() == "END"){	// If END is found after FOR, we just broke out of a loop, and
				sameLoop = true;						// don't have to start a new one since we are still in the same loop,
				count--;								// and we can simply pretend the loop that completed before never
				forCount--;								// happened.
				beginCount--;
				loops[count].clear();
				loops[count].push(word);
			}
		}
		if (word[word.size()-1] == ')' || word[0] == '('){  // This checks parentheses by pushing to a character stack
			hasParentheses = true;							// every left parentheses it finds, and popping every right.
			for(char i : word){								// If there is an inequality, either the stack will not be empty
				switch(i){									// at the end of the loop or there will be an error with right
				case '(':									// parentheses, which I catch here with rightParenthError.
					this->stackChar.push(i);
					break;
				case ')':
					if(!this->stackChar.isEmpty())
						this->stackChar.pop();
					else
						rightParenthError = true;
					break;
				}
			}
		}
		if((word == "BEGIN" && loops[count].peek() == "FOR")
			&& this->stackChar.isEmpty() && !rightParenthError && hasParentheses){
			loops[count].push(word);		// If the word found is BEGIN and came after FOR, and there were no parentheses
			beginCount++;					// errors, and there were at least some parentheses, then we push BEGIN to the stack
		}									// and count it.
		if(word == "END"){
			forCount--;								// If the word is END, we decrement everything to signify the end of a loop.
			beginCount--;
			count--;
			if(loops[count+1].peek() == "BEGIN"){   // If a BEGIN came before, we count back up to maintain the same loop.
				count++;
				loops[count].push(word);
			}
			else if(loops[count+1].peek() == "END"){ // If a END came before, we don't count back up, we add it to a lower
				loops[count].push(word);			 // stack previously used.
			}
			else if(sameLoop){		// If the word was END and we are given we are in a loop that has had another loop
				beginCount++;		// in it and deleted, we can increment BEGIN's value up to maintain the equality
				sameLoop = false;	// between BEGIN and FOR for just below this.
			}
		}
	}

	if(forCount > 0)				// If the count of FOR wasn't properly decremented, we are missing END statements.
		loopsReached -= forCount;	// Thus, remove the number of loops that END hadn't completed
	else if(beginCount != forCount)				 // Also, if there were proper END statements but some missing BEGIN statements
		loopsReached -= (forCount - beginCount); // we remove the number of loops that were not balanced.

	this->stackChar.clear(); // Clear the character stack for parentheses so getSyntaxErrors() can properly function.

	return loopsReached;	// Return the loops!
}

void Compiler::getSyntaxErrors(){ // A complex method to find and print any syntax errors found.
	std::cout << "Syntax Error(s): ";
	std::vector<std::string> syntaxErrors;
	std::string possibleError = "";
	int forCount = 0;
	int beginCount = 0;
	int endCount = 0;
	bool hasParentheses = false;
	bool endMissing = true;
	bool beginMissing = true;
	bool hasUpperCase = false;

	for(std::string word : this->words){
		if(!alreadyContains(word, this->keyWords)){
			for(char i : word){						// Loop to check if the word is any bit upperCase and not already a keyword.
				if(isUpperCaseCharacter(i)){		// If it meets both criteria, continue to build the word until the full error is found.
					possibleError += i;
					hasUpperCase = true;
				}
				if(isLowerCaseCharacter(i))
					possibleError += i;
			}

			if(!alreadyContains(possibleError, syntaxErrors) && hasUpperCase){
				if((possibleError[0] == 'B' || possibleError[0] == 'b') ||
				   (possibleError[word.size()-1] == 'N' || possibleError[word.size()-1] == 'n')){  // Checks the first and last letters of the error
					beginMissing = false;	// So, BEGIN is not missing, it is misspelled.		   // to determine if BEGIN is misspelled.
				}
				if((possibleError[0] == 'E' || possibleError[0] == 'e') ||
				   (possibleError[word.size()-1] == 'D' || possibleError[word.size()-1] == 'd')){  // Checks the first and last letters of the error
					endMissing = false;		// So, END is not missing, it is misspelled.		   // to determine if END is misspelled.
				}

				giveError(syntaxErrors, possibleError, possibleError);  // Give the error found.
			}
			possibleError = ""; 	// Reset the value of possibleError.
			hasUpperCase = false; 	// Reset the value of hasUpperCase.
		}
		if(word == "FOR"){											// If the word is FOR, if there are leftover left parentheses not
			if(!this->stackChar.isEmpty()){							// accounted for, pop them all off the stack and give them as errors.
				while(!this->stackChar.isEmpty()){
					giveError(syntaxErrors, possibleError, "(");
					this->stackChar.pop();
				}
			}
			else{												   // If the word is FOR and the stack is empty, if it isn't the first
				if(!hasParentheses && forCount > 0){			   // FOR, if there were no parentheses (therefore balanced), we give
					giveError(syntaxErrors, possibleError, "(");   // the missing pair as an error.
					giveError(syntaxErrors, possibleError, ")");
				}												   // Then clear the stack, reset hasParentheses, and add to forCount.
				this->stackChar.clear();
			}
			hasParentheses = false;
			forCount++;
		}
		else if(word == "END"){   // If the word is END or BEGIN, add to their respective counts;
			endCount++;
		}
		else if(word == "BEGIN"){
			beginCount++;
		}
		else if (word[word.size()-1] == ')' || word[0] == '('){  // This checks parentheses by pushing to a character stack
			hasParentheses = true;								 // every left parentheses it finds, and popping every right.
			for(char i : word){									 // However, it only pops right parentheses if there are left
				switch(i){										 // parentheses to accompany them. Otherwise it gives the
				case '(':										 // those stray right parentheses as errors.
					this->stackChar.push(i);
					break;
				case ')':
					(!this->stackChar.isEmpty()) ? this->stackChar.pop() : giveError(syntaxErrors, possibleError, ")");
					break;
				}
			}
		}
	}

	while(!this->stackChar.isEmpty()){							// At the end of the checking, any overflowing left parentheses
		giveError(syntaxErrors, possibleError, "(");			// are given as errors.
		this->stackChar.pop();
	}

	if(((forCount > beginCount) || (beginCount > forCount)) && beginMissing)  // If the counts are not balanced and BEGIN is missing,
		giveError(syntaxErrors, possibleError, "BEGIN"); 					  // give it as an error.

	if(((forCount > endCount) || (endCount > forCount)) && endMissing)		  // If the counts are not balanced and END is missing,
		giveError(syntaxErrors, possibleError, "END");						  // give it as an error.

	if(syntaxErrors.empty())								// If there have been no pushed errors, print NA.
		std::cout << "NA";

	std::cout << std::endl;
}

void Compiler::getConstants(){ // A simple method to loop through the map of the words in the file, and extract any constants.
	std::cout << "Constants:";
	std::string constant = "";
	std::vector<std::string> tempConstants;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::string word = this->it->first;
		for(char i : word){
			if(isdigit(i)){
				constant += i;
			}
		}
		if(!(alreadyContains(constant, tempConstants))){
			tempConstants.push_back(constant);
			std::cout << constant << " ";
		}
		constant = "";
	}
	std::cout << std::endl;
}

void Compiler::getIdentifiers(){ // A simple method to loop through the map of the words in the file, and extract any identifiers.
	std::cout << "Identifiers: ";
	std::vector<std::string> tempVariables;
	std::string tempVariable;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::string word = this->it->first;
		for(char i : word){
			if(isLowerCaseCharacter(i))
				tempVariable += i;
			else if(tempVariable.size() >= 1 || isUpperCaseCharacter(i))
				break;
		}
		if(!(alreadyContains(tempVariable, tempVariables)) && tempVariable.size() >= 1){
			tempVariables.push_back(tempVariable);
			std::cout << tempVariable << " ";
		}
		tempVariable = "";
	}
	std::cout << std::endl;
}

void Compiler::getKeyWords(){ // A simple method to loop through the map of the words in the file, and extract the keywords.
	std::cout << "Keywords: ";
	std::vector<std::string> tempKeywords;   // Uses an iterator to iterate through the hash map.
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(std::string keyWord : this->keyWords){
			if(((this->it->first).find(keyWord) != std::string::npos) && !(alreadyContains(keyWord, tempKeywords))){
				tempKeywords.push_back(keyWord);
				std::cout << keyWord << " ";
				break;
			}							// Checks for every word, if that word is contained within the set of keywords,
		}								// if it is, it is added only once to the tempKeywords and printed.
	}
	std::cout << std::endl;
}

void Compiler::getOperators(){ // A simple method to loop through the map of the words in the file, and extract the operators.
	std::cout << "Operators: ";
	std::vector<std::string> tempOperators;	 // Uses an iterator to iterate through the hash map.
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(std::string op : this->operators){
			if(((this->it->first).find(op) != std::string::npos) && !(alreadyContains(op, tempOperators))){
				tempOperators.push_back(op);
				std::cout << op << " ";
				break;
			}							// Checks for every word, if that word is contained within the set of operators,
		}								// if it is, it is added only once to the tempOperators and printed.
	}
	std::cout << std::endl;
}

void Compiler::getDelimiters(){ // A simple method to loop through the map of the words in the file, and extract the delimiters.
	std::cout << "Delimiters: ";
	std::vector<std::string> tempDelimiters;  // Uses an iterator to iterate through the hash map.
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(std::string delimiter : this->delimiters){
			if(((this->it->first).find(delimiter) != std::string::npos) && !(alreadyContains(delimiter, tempDelimiters))){
				tempDelimiters.push_back(delimiter);
				std::cout << delimiter << " ";
				break;
			}						 	// Checks for every word, if that word is contained within the set of delimiters,
		}								// if it is, it is added only once to the tempDelimiters and printed.
	}
	std::cout << "\n" << std::endl;
}

void Compiler::read(std::ifstream& file){ // A simple method to read a file.
	std::string word;
	while(file >> word){				// Reads the file and fills a hash map and a vector with the words.
		this->wordFrequency[word]++;	// I used both for different operations at different times.
		this->words.push_back(word);
	}
	file.close();
}

void Compiler::parseData(){ // Asks for a file name, and uses it to find and read that file.
	std::string fileLocation;
	std::cout << "Please enter the name of the input file:" << std::endl;
	std::cin >> fileLocation;
	std::ifstream file(fileLocation.c_str());

	this->read(file);					// Using private methods, performs all operations required on the data.

	std::cout << "The depth of nested loop(s) is " << this->getLoopDepth() << "\n" << std::endl;

	this->getKeyWords();
	this->getIdentifiers();
	this->getConstants();
	this->getOperators();
	this->getDelimiters();
	this->getSyntaxErrors();
}

int main(){
	Compiler * compiler = new Compiler(); // Instantiates a compiler and parses it.

	compiler->parseData();

	return 0;
}

