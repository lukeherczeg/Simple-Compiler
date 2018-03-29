#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include "pa3.h"


bool alreadyContains(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}


void Reader::getOperators(){
	std::cout << "Operators : ";
	std::vector<std::string> tempOperators;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(unsigned int i = 0; i < 6; i++){
			if((this->it->first).find(this->operators[i]) != std::string::npos && !(alreadyContains(this->operators[i], tempOperators))){
				tempOperators.push_back(this->operators[i]);
				std::cout << this->operators[i] << " ";
				break;
			}
		}
	}
	std::cout << std::endl;
}

void Reader::getDelimiters(){
	std::cout << "Delimiters : ";
	std::vector<std::string> tempDelimiters;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(unsigned int i = 0; i < 2; i++){
			if((this->it->first).find(this->delimiters[i]) != std::string::npos && !(alreadyContains(this->delimiters[i], tempDelimiters))){
				tempDelimiters.push_back(this->delimiters[i]);
				std::cout << this->delimiters[i] << " ";
				break;
			}
		}
	}
	std::cout << std::endl;
}

void Reader::getKeyWords(){
	std::cout << "Keywords : ";
	std::vector<std::string> tempKeywords;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		for(unsigned int i = 0; i < 3; i++){
			if((this->it->first).find(this->keyWords[i]) != std::string::npos && !(alreadyContains(this->keyWords[i], tempKeywords))){
				tempKeywords.push_back(this->keyWords[i]);
				std::cout << this->keyWords[i] << " ";
				break;
			}
		}
	}
	std::cout << std::endl;
}

void Reader::fillMap(std::ifstream& file){
	std::string word;
	while(file >> word){
		this->wordFrequency[word]++;
	}
	file.close();
}

void Reader::print(){
	std::cout << std::endl;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::cout << this->it->first << " frequency: " << this->it->second << std::endl;
	}
}

int main(){
	Reader * r = new Reader();
	std::string fileLocation;
	//std::cin >> fileLocation;
	fileLocation = "code.txt";
	std::ifstream file(fileLocation.c_str());

	r->fillMap(file);
	r->getOperators();
	r->getKeyWords();
	r->getDelimiters();
	r->print();

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
