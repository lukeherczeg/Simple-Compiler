#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include "pa3.h"




void addWord(std::string input, std::vector<std::string> & wordList, std::vector<int> & frequency){
	for(int i = 0; i < wordList.size(); i++){
		if(input == wordList[i]){
			frequency[i]++;
			return;
		}
	}
	wordList.push_back(input);
	frequency.push_back(1);
}



int main(){

	std::string fileLocation;
	std::cin >> fileLocation;
	std::ifstream file(fileLocation.c_str());
	std::string word;


	/* Parallel Vectors
	std::vector <std::string> wordList;
	std::vector <int> frequency;
	while(file >> input){
		addWord(input,wordList,frequency);
	}
	for(int i = 0; i < wordList.size(); i++){
		std::cout << "Word: " << wordList[i] << "   Frequency: " << frequency[i] << std::endl;
	}

	*/
	//Hash Map
	/*Important things for hashing functions
	 * Completely random
	 * Same input - same output
	 * Fast
	 *
	 *
	 * Use modulus to get index 0 - (tableSize-1)
	 * (%tableSize)
	 *
	 * */

	std::map<std::string, int> wordFrequency;
	while(file>>word){
		wordFrequency[word]++;
	}

	for(std::map<std::string, int>::iterator it = wordFrequency.begin(); it != wordFrequency.end(); ++it){
		std::cout << it->first << " frequency: " << it->second << std::endl;
	}

	/*while(getline(file, input)){
		std::cout << input << std::endl;
	}*/
	file.close();

	return 0;
}
