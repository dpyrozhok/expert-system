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
	std::string get_input_file(char ac, char **av, std::vector<std::string> input_list);
	std::string line_cleaner(std::vector<std::string> input_list);

private:
	std::vector<std::string> input_list;

	std::vector<std::string> cleaned_lines;

};

class DataManager {
	int rules_qty;
	std::vector<std::string> Rules;

	int facts_qty;
	std::vector<std::string> Facts;

	int querries_qty;
	std::vector<std::string> Querries;

};
#endif
