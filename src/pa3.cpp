#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ctype.h>
#include <map>
#include "pa3.h"

bool isUpperCaseCharacter(char character){
	return (character <= 'Z' && character >= 'A') ? true : false;
}

bool isLowerCaseCharacter(char character){
	return (character <= 'z' && character >= 'a') ? true : false;
}

void giveError(std::vector<std::string> & syntaxErrors, std::string possibleError, std::string c){
	possibleError = c; 					   // Make the possibleError the one given as c
	syntaxErrors.push_back(possibleError); // Add the error to the syntaxErrors vector
	std::cout << possibleError << " ";	   // Print the error.
}

template <class T> bool alreadyContains(const T &value, const std::vector<T> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

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
	if(!stackList.empty())
		while(!stackList.empty()){
			stackList.pop_back();
		}
}

template <class T> T Stack<T>::peek(){
	if(!stackList.empty())
		return stackList.back();
	return "";
}

void Compiler::getSyntaxErrors(){
	std::cout << "Syntax Error(s): ";
	std::vector<std::string> syntaxErrors;
	std::string possibleError = "";
	int last = 0;
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
				if(isLowerCaseCharacter(i)){
					possibleError += i;
				}
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
			possibleError = ""; // Reset the value of possibleError.
			hasUpperCase = false; // Reset the value of hasUpperCase.
		}
		if(word == "FOR" && !this->stackChar.isEmpty()){
			hasParentheses = false;
			while(!this->stackChar.isEmpty()){
				giveError(syntaxErrors, possibleError, "(");
				this->stackChar.pop();
			}
			this->stackString.push(word);
			forCount++;
		}
		else if(word == "FOR"){
			if(!hasParentheses && forCount > 0){
				giveError(syntaxErrors, possibleError, "(");
				giveError(syntaxErrors, possibleError, ")");
			}
			this->stackString.push(word);
			this->stackChar.clear();
			hasParentheses = false;
			forCount++;
		}
		else if(word == "END"){
			endCount++;
		}
		else if(word == "BEGIN"){
			beginCount++;
		}
		else if (word[word.size()-1] == ')' || word[0] == '('){
			hasParentheses = true;
			int pos = -1;
			for(char i : word){
				++pos;
				switch(i){
				case '(':
					this->stackChar.push(i);
					break;
				case ')':
					if(!this->stackChar.isEmpty()){
						this->stackChar.pop();
						if(pos == last)
							this->stackString.pop();
					}
					else{
						giveError(syntaxErrors, possibleError, ")");
						this->stackString.pop();
					}
					break;
				}
			}
		}
	}

	while(!this->stackChar.isEmpty()){
		giveError(syntaxErrors, possibleError, "(");
		this->stackChar.pop();
	}

	if(((forCount > beginCount) || (beginCount > forCount)) && beginMissing)
		giveError(syntaxErrors, possibleError, "BEGIN");

	if(((forCount > endCount) || (endCount > forCount)) && endMissing)
		giveError(syntaxErrors, possibleError, "END");

	if(syntaxErrors.empty())
		std::cout << "NA";

	std::cout << std::endl;
}

int Compiler::getLoopDepth(){
	int count = 0;
	int loopsReached = 1;
	int beginCount = 0;
	int forCount = 0;
	bool extraRightParentheses = false;
	bool sameLoop = false;
	bool hasParentheses = false;
	bool isFirstLoop = true;
	std::vector<Stack<std::string>> loops;
	for(std::string word : this->words){
		if(word == "FOR" && isFirstLoop){
			forCount++;
			loops.push_back(Stack<std::string>());
			loops[count].push(word);
			isFirstLoop = false;
		}
		else if(word == "FOR"){
			forCount++;
			count++;
			hasParentheses = false;
			extraRightParentheses = false;
			this->stackChar.clear();
			if(loops[count-1].peek() == "FOR" && count == 1){
				forCount = 0; beginCount = 0; loopsReached = 0;
				break;
			}
			else if(loops[count-1].peek() == "FOR" && count == 2){
				forCount = 0; beginCount = 0; loopsReached = 1;
				break;
			}
			else if(loops[count-1].peek() == "BEGIN" || loops[count-1].peek() == "FOR"){
				loops.push_back(Stack<std::string>());
				loops[count].push(word);
				loopsReached++;
			}
			else if(loops[count-1].peek() == "END"){
				sameLoop = true;
				count--;
				forCount--;
				beginCount--;
				loops[count].clear();
				loops[count].push(word);
			}
		}
		if (word[word.size()-1] == ')' || word[0] == '('){
			hasParentheses = true;
			for(char i : word){
				switch(i){
				case '(':
					this->stackChar.push(i);
					break;
				case ')':
					if(!this->stackChar.isEmpty())
						this->stackChar.pop();
					else
						extraRightParentheses = true;
					break;
				}
			}
		}
		if((word == "BEGIN" && loops[count].peek() == "FOR") && this->stackChar.isEmpty()
				&& !extraRightParentheses && hasParentheses){
			loops[count].push(word);
			beginCount++;
		}
		if(word == "END"){
			forCount--;
			beginCount--;
			count--;
			if(loops[count+1].peek() == "BEGIN"){
				count++;
				loops[count].push(word);
			}
			else if(loops[count+1].peek() == "END"){
				loops[count].push(word);
			}
			else if(sameLoop)
				beginCount++;
		}
	}

	if(forCount > 0)
		loopsReached -= forCount;
	else if(beginCount != forCount)
		loopsReached -= (forCount - beginCount);

	this->stackChar.clear();

	return loopsReached;
}

void Compiler::getConstants(){
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

void Compiler::getIdentifiers(){
	std::cout << "Identifiers: ";
	std::vector<std::string> tempVariables;
	std::string tempVariable;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::string word = this->it->first;
		for(char i : word){
			if(isLowerCaseCharacter(i)){
				tempVariable += i;
			}
			else if(tempVariable.size() >= 1 || isUpperCaseCharacter(i)){
				break;
			}
		}
		if(!(alreadyContains(tempVariable, tempVariables)) && tempVariable.size() >= 1){
			tempVariables.push_back(tempVariable);
			std::cout << tempVariable << " ";
		}
		tempVariable = "";
	}
	std::cout << std::endl;
}

void Compiler::getKeyWords(){
	std::cout << "Keywords: ";
	std::vector<std::string> tempKeywords;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(std::string keyWord : this->keyWords){
			if((this->it->first).find(keyWord) != std::string::npos && !(alreadyContains(keyWord, tempKeywords))){
				tempKeywords.push_back(keyWord);
				std::cout << keyWord << " ";
				break;
			}
		}
	}
	std::cout << std::endl;
}

void Compiler::getOperators(){
	std::cout << "Operators: ";
	std::vector<std::string> tempOperators;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(std::string op : this->operators){
			if((this->it->first).find(op) != std::string::npos && !(alreadyContains(op, tempOperators))){
				tempOperators.push_back(op);
				std::cout << op << " ";
				break;
			}
		}
	}
	std::cout << std::endl;
}

void Compiler::getDelimiters(){
	std::cout << "Delimiters: ";
	std::vector<std::string> tempDelimiters;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(std::string delimiter : this->delimiters){
			if(((this->it->first).find(delimiter) != std::string::npos) && !(alreadyContains(delimiter, tempDelimiters))){
				tempDelimiters.push_back(delimiter);
				std::cout << delimiter << " ";
				break;
			}
		}
	}
	std::cout << "\n" << std::endl;
}

void Compiler::fillMap(std::ifstream& file){
	std::string word;
	while(file >> word){
		this->wordFrequency[word]++;
		this->words.push_back(word);
	}
	file.close();
}

void Compiler::parse(){
	std::string fileLocation;
	std::cout << "Please enter the name of the input file:" << std::endl;
	//std::cin >> fileLocation;
	fileLocation = "code.txt";
	std::ifstream file(fileLocation.c_str());

	this->fillMap(file);

	std::cout << "The depth of nested loop(s) is " << this->getLoopDepth() << "\n" << std::endl;

	this->getKeyWords();
	this->getIdentifiers();
	this->getConstants();
	this->getOperators();
	this->getDelimiters();
	this->getSyntaxErrors();
}

int main(){
	Compiler * compiler = new Compiler();

	compiler->parse();

	return 0;
}

