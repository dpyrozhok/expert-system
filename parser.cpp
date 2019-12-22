#include "expert_system.hpp"

class InputManager input_data;

int main(int ac, char **av){
	if (ac == 2) {
		input_data.get_input_file(std::string(av[1]));
		input_data.line_cleaner();
		input_data.print_cleaned_lines();
	}
	return (0);
}

