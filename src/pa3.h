#ifndef PA3_H_
#define PA3_H_

class Reader{
private:
	std::vector<std::string> keyWords = {"FOR", "BEGIN", "END"};
	std::vector<std::string> operators = {"++", "-", "*", "/", "=", "+"};
	std::vector<std::string> delimiters = {";", ","};
	std::map<std::string, int> wordFrequency;
public:
	std::map<std::string, int>::iterator it;
	void fillMap(std::ifstream& file);
	void getDelimiters();
	void getOperators();
	void getKeyWords();
	void print();
};






#endif /* PA3_H_ */
