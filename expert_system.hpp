#ifndef EXPERT_SYSTEM_HPP
#define EXPERT_SYSTEM_HPP

#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

class InputManager {
public:
	string get_input_file(char ac, char **av, std::vector<string> input_list);
	string line_cleaner(std::vector<string> input_list);

private:
	int line_quantity;
	std::vector<string> input_list;

	int cleaned_quantity;
	std::vector<string> cleaned_lines

};

class DataManager {
	int rules qty;
	std::vector<string> Rules;

	int facts qty;
	std::vector<string> Facts;

	int querries qty;
	std::vector<string> Querries;

};
#endif
