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

void Stack::push(std::string item){
	stackList.push_back(item);
}

void Stack::pop(){
	stackList.pop_back();
}

std::string Stack::peek(){
	return stackList.back();
}

bool Stack::isEmpty(){
	return stackList.empty();
}

int Compiler::findLoopDepth(){
	int count = 0;
	std::string word = this->it->first;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::string word = this->it->first;
		for(char i : word){
			break;
		}
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

/*void Compiler::getSyntaxErrors(){
	std::cout << "Syntax error(s): ";
}
*/

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
	std::cout << std::endl;
}

void Compiler::fillMap(std::ifstream& file){
	std::string word;
	while(file >> word){
		this->wordFrequency[word]++;
	}
	file.close();
}

void Compiler::print(){
	std::cout << std::endl;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::cout << this->it->first << std::endl;
	}
}

void Compiler::parse(){
	std::string fileLocation;
	fileLocation = "code.txt";
	std::ifstream file(fileLocation.c_str());

	this->fillMap(file);
	this->getKeyWords();
	this->getIdentifiers();
	this->getConstants();
	this->getOperators();
	this->getDelimiters();
}

//encounter for, push
//encounter left parenth. if the top of the stack is for, push it to the stack
//encounter right parenth. if the top of the stack is left parenth. pop off the left parenth.


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
