#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ctype.h>
#include <map>
#include "pa3.h"

template <class T> bool alreadyContains(const T &value, const std::vector<T> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

bool isUpperCaseCharacter(char character){
	return (character <= 'Z' && character >= 'A') ? true : false;
}

bool isLowerCaseCharacter(char character){
	return (character <= 'z' && character >= 'a') ? true : false;
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

template <class T> bool Stack<T>::isEmpty(){
	return stackList.empty();
}

char isMatching(char paren){
	return (paren == '(') ? ')' : 'n';
}

void giveError(std::vector<std::string> & syntaxErrors, std::string possibleError, std::string c){
	possibleError = c;
	syntaxErrors.push_back(possibleError);
	std::cout << possibleError << " ";
}

/*
int main(){

	std::string exp;
	std::cin >> exp;
    Stack * parenStack = new Stack();

    for(unsigned int i = 0; i < exp.size(); i++){
    	if(exp[i] == '(' || exp[i] == '[' || exp[i] == '{'){
    		parenStack->push(exp[i]);
    	}
    	else{
    		if(parenStack->isEmpty() || isMatching(parenStack->peek()) != exp[i]){
    			std::cout << "Not valid expression" << std::endl;
    			return 0;
    		}
    		parenStack->pop();
    	}
    }
    if(!parenStack->isEmpty()){
    	std::cout << "Not valid expression" << std::endl;
    }
    else{
    	std::cout << "Valid expression" << std::endl;
    }

	return 0;
}
*/

int Compiler::findLoopDepth(){
	int count = 0;
	std::string word = this->it->first;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::string word = this->it->first;
		//for(char i : word){
		//	break;
		//}
	}
	return count;
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
	std::cout << "Identifiers:";
	std::vector<std::string> tempVariables;
	std::string tempVariable;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::string word = this->it->first;
		for(char i : word){
			if(isLowerCaseCharacter(i)){
				tempVariable += i;
			}
			else break;
		}
		if(!(alreadyContains(tempVariable, tempVariables))){
			tempVariables.push_back(tempVariable);
			std::cout << tempVariable << " ";
		}
		tempVariable = "";
	}
	std::cout << std::endl;
}

void Compiler::getSyntaxErrors(){
	std::cout << "Syntax error(s): ";
	std::vector<std::string> syntaxErrors;
	std::string possibleError = "";
	int last = 0;
	bool hasUpperCase = false;

	for(std::string word : this->words){
		if(word == "BEGIN" && !this->stackChar.isEmpty()){
			giveError(syntaxErrors, possibleError, "(");
			this->stackString.push(word);
			this->stackChar.clear();
		}
		else if(word == "FOR" && !this->stackChar.isEmpty()){
			giveError(syntaxErrors, possibleError, "(");
			this->stackString.push(word);
			this->stackChar.clear();
		}
		else if(word == "BEGIN"){
			this->stackString.push(word);
		}
		else if(word == "FOR"){
			this->stackString.push(word);
			this->stackChar.clear();
		}
		else if((word == "FOR" && this->stackString.peek() == word) || (word == "BEGIN" && this->stackString.peek() == word)){
			giveError(syntaxErrors, possibleError, word);
		}
		else if(word == "END" && (this->stackString.peek() == "BEGIN" || this->stackString.peek() == "FOR")){
			this->stackString.pop();
		}
		else if (word[last] == ')' || word[0] == '('){
			int pos = -1;
			last = word.size() - 1;
			for(char i : word){
				++pos;
				if(i == '('){
					this->stackChar.push(i);
				}
				else if(i == ')'){
					if(!this->stackChar.isEmpty()){
						this->stackChar.pop();
						if(pos == last){
							this->stackString.pop();
						}
					}
					else{
						giveError(syntaxErrors, possibleError, ")");
						this->stackString.pop();
						break;
					}
					/*else if(this->stackChar.isEmpty()){
						giveError(syntaxErrors, possibleError, "(");
						break;
					}*/
				}
			}
		}
	}
	//if(!this->stackChar.isEmpty()){
	//	giveError(syntaxErrors, possibleError, "(");
	//}
	if(!this->stackString.isEmpty()){
		giveError(syntaxErrors, possibleError, "END");
	}

	//encounter for, push
	//encounter left parenth. if the top of the stack is for, push it to the stack
	//encounter right parenth. if the top of the stack is left parenth. pop off the left parenth.



	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::string word = this->it->first;
		if(!alreadyContains(word, this->keyWords)){
			for(char i : word){
				if(isUpperCaseCharacter(i)){
					possibleError += i;
					hasUpperCase = true;
				}
				if(isLowerCaseCharacter(i)){
					possibleError += i;
				}
			}

			if(!alreadyContains(possibleError, syntaxErrors) && hasUpperCase){
				syntaxErrors.push_back(possibleError);
				std::cout << possibleError << " ";
				possibleError = "";
			}
			possibleError = "";
			hasUpperCase = false;
		}
	}
	if(syntaxErrors.empty())
		std::cout << "NA";
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
		this->rawData += word;
		this->words.push_back(word);
	}
	file.close();
}

void Compiler::print(){
	std::cout << std::endl;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::cout << this->it->first << std::endl;
	}
	std::cout << "\n";
	for(std::string word : this->words){
		std::cout << word << " ";
	}
}

void Compiler::parse(){
	std::string fileLocation;
	std::cout << "Please enter the name of the input file:" << std::endl;
	//std::cin >> fileLocation;
	fileLocation = "code.txt";
	std::ifstream file(fileLocation.c_str());

	this->fillMap(file);
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
	compiler->print();

	return 0;
}





/* Parallel Vectors
std::vector <std::string> wordList;
std::vector <int> frequency;
while(file >> input){
	addWord(input,wordList,frequency);
}
for(int i = 0; i < wordList.size(); i++){
	std::cout << "Word: " << wordList[i] << "   Frequency: " << frequency[i] << std::endl;
}*/


/*void addWord(std::string input, std::vector<std::string> & wordList, std::vector<int> & frequency){
	for(unsigned int i = 0; i < wordList.size(); i++){
		if(input == wordList[i]){
			frequency[i]++;
			return;
		}
	}
	wordList.push_back(input);
	frequency.push_back(1);
}*/
