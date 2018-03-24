#ifndef PA3_H_
#define PA3_H_

class Reader{
private:
	//std::string * keywords = {"BEGIN","END","FOR"};
	//char * operators = {"+-*/="};
	std::map<std::string, int> wordFrequency;
public:
	std::map<std::string, int>::iterator it;
	void parse(std::ifstream& file);
	void print();
};






#endif /* PA3_H_ */
