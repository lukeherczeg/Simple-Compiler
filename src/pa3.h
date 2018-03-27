#ifndef PA3_H_
#define PA3_H_

class Reader{
private:
	std::string * keywords = new std::string[3] {"BEGIN","END","FOR"};
	std::string * operators = new std::string[6] {"++","+","-","*","/","="};
	std::map<std::string, int> wordFrequency;
public:
	std::map<std::string, int>::iterator it;
	void parse(std::ifstream& file);
	void getKeyWords();
	void print();
};






#endif /* PA3_H_ */
