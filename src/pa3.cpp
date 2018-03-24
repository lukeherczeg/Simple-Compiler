#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include "pa3.h"


void Reader::parse(std::ifstream& file){
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
	std::cin >> fileLocation;
	std::ifstream file(fileLocation.c_str());

	r->parse(file);
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
