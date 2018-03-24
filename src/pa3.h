#ifndef PA3_H_
#define PA3_H_

class Reader{
private:
	std::map<std::string, int> wordFrequency;
public:
	std::map<std::string, int>::iterator it;
	void read(std::ifstream file);
};






#endif /* PA3_H_ */
