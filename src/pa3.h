#ifndef PA3_H_
#define PA3_H_

template<class T>
class Stack{
private:
	std::vector<T> stackList;
public:
	T peek();
	void push(T item);
	void pop();
	bool isEmpty();
};

class Compiler{
private:
	Stack<char> stackChar;
	Stack<std::string> stackString;
	std::map<std::string, int>::iterator it;
	std::vector<std::string> keyWords = {"FOR", "BEGIN", "END"};
	std::vector<std::string> operators = {"++", "-", "*", "/", "=", "+"};
	std::vector<std::string> delimiters = {";", ","};
	std::map<std::string, int> wordFrequency;
	std::vector<std::string> words;
	std::string rawData;
	bool parenthesesValid(std::string word);
	int findLoopDepth();
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
