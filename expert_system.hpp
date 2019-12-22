#ifndef EXPERT_SYSTEM_HPP
#define EXPERT_SYSTEM_HPP

#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <exception>

enum DATA_SWITCH{
	RULES_START = 65,
	RULES_END = 90,
	FACTS = 61,
	QUERRIES = 63
};

enum ALLOWED_CHARS{
	LEX_START = 65,
	LEX_END = 90,
	LSKOBKA = 40,
	PSKOBKA = 41,
	PLUS = 43,
	LUKAZ = 60,
	RAVNO = 62,
	PUKAZ = 61,
	STEPEN = 94,
	VOSKL = 33
};

bool is_char_operator(char c) {
	if (c == '+' || c == '|' || c == '^')
		return true;
	else
		return false;
}

class InputManager {
public:
	InputManager(){
		this->input_list = {};
		this->cleaned_lines = {};
	}
	void line_cleaner() {
		std::size_t found;

		for (auto i : this->input_list) {
			/* Delete comments */
			found = i.find('#');
			if (found != -1) {
				i.resize(found);
			}
			/* Delete spaces */
			found = 0;
			do{
				found = i.find(' ', found);
				if (found != -1) i.erase(found, 1);
			} while (found != -1);
			/* Write to cleaned_rules */
			if (i.size() > 0)
				this->cleaned_lines.push_back(i);
		}
	}

	void get_input_file(std::string filename){
		std::string line;
		std::ifstream input(filename);
		while(std::getline(input, line)) {
			//std::cout << line << std::endl;
			this->input_list.push_back(line);
		}
	}

	void print_cleaned_lines(){
	for(auto i : this->cleaned_lines){
		std::cout << i << std::endl;
	}

	}
//	std::vector<std::string> cleaned_lines;

	std::vector<std::string> getterCL(){
		return this->cleaned_lines;
	}
private:
	std::vector<std::string> input_list;

	std::vector<std::string> cleaned_lines;

};

class DataManager {

public:
	DataManager(){
		this->cleaned_lines = {};
}
	int print_data(void){
		for (auto i: this->cleaned_lines){
			std::cout << i << std::endl;
		}
	}

	void setterCL(std::vector<std::string> in){
		this->cleaned_lines = in;
	}


	std::vector<std::string> getterCL(){
		return this->cleaned_lines;
	}

/*	void getRules(std::string str) {
		int i = 0;
		while (str[i]){*/
			/* Rule one - after each letter should goes operator */
			/*if (str[i] >= 65 && str[i] <= 90 && is_char_operator(str[i + 1])) {
				std::cout<< str[i + 1] << std::endl;
			} else if (str[i] == '(' && (str[i + 1] >= 65 || str[i + 1] <= 90)) {
						i++;
		}
	}
*/

	void getRules(std::string line) {
		for (auto symb: line){
		std::cout << "CAST " << static_cast<ALLOWED_CHARS>(symb) << std::endl;
		if ((symb >= LEX_START && symb <= LEX_END) || (static_cast<ALLOWED_CHARS>(symb) ))
			std::cout << "OK, GO NEXT = (" << symb <<")\n";
		else
			std::cout << "BAD SYMBOL, WARNING = (" << symb <<")\n";
		std::cout << "\n\n";

	}
}

	void getFacts(std::string line){

	}

	void getQuerries(std::string line){

	}

	void dataType(){
		for (auto line: this->cleaned_lines)  { // chech each line

			std::cout << line <<std::endl;
			std::cout << line[0] <<std::endl;
			std::cout << static_cast<ALLOWED_CHARS>(line[0]) <<std::endl;
			int ascii_code = line[0];
			DATA_SWITCH ascii = DATA_SWITCH(ascii_code);

			std::cout << ascii <<std::endl;
			if (FACTS == ascii_code){
				getFacts(line);
			}
			else if (QUERRIES == ascii_code){
				getQuerries(line);
			}

			else if (ascii_code >= RULES_START && ascii_code <= RULES_END){
							getRules(line);
			}
			else
				throw("Error. Bad line\n");
		}
}


private:
	std::vector<std::string> cleaned_lines;

	int rules_qty;
	std::vector<std::string> Rules;

	int facts_qty;
	std::vector<std::string> Facts;

	int querries_qty;
	std::vector<std::string> Querries;

};

#endif
