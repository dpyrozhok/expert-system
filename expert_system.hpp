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

class InputManager {
public:
	InputManager(){
		this->input_list = {};
		this->cleaned_lines = {};
	}
	void line_cleaner() {
		std::size_t found;

		for (auto i : this->input_list) {
			found = i.find('#');
			if (found != -1) {
				i.resize(found);
				if (i.size() > 0)
					this->cleaned_lines.push_back(i);
			}
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
	/*void getRules() {
		for (auto i: this->cleaned_lines)  { // chech each line
			for (auto symb: i){ //check each character

			}
		}
	}*/


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
