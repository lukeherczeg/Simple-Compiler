#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include "pa3.h"


void Reader::read(std::ifstream file){
	std::string word;
	while(file >> word){
		this->wordFrequency[word]++;
	}
	std::cout << std::endl;
	for(this->it = this->wordFrequency.begin(); this->it != this->wordFrequency.end(); (this->it)++){
		std::cout << this->it->first << " frequency: " << this->it->second << std::endl;
	}
	file.close();
}


int main(){
	Reader * r = new Reader();
	std::string fileLocation;
	std::ifstream file(fileLocation.c_str());
	std::cin >> fileLocation;

	r->read(file);

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
