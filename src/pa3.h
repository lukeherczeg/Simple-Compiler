#ifndef PA3_H_
#define PA3_H_

class Stack{
private:
	std::vector<std::string> stackList;
public:
	std::string peek();
	void push(std::string item);
	void pop();
	bool isEmpty();
};


class Compiler{
private:
	std::map<std::string, int>::iterator it;
	std::vector<std::string> keyWords = {"FOR", "BEGIN", "END"}; const int numKeyWords = 3;
	std::vector<std::string> operators = {"++", "-", "*", "/", "=", "+"}; const int numOperators = 6;
	std::vector<std::string> delimiters = {";", ","}; const int numDelimiters = 2;
	std::map<std::string, int> wordFrequency;
	void fillMap(std::ifstream& file);
	void getSyntaxErrors();
	void getDelimiters();
	void getConstants();
	void getIdentifiers();
	void getOperators();
	void getKeyWords();
public:
	void parse();
	void print();
};






#endif /* PA3_H_ */
