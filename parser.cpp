#include "expert_system.hpp"



int main(int ac, char **av){
	std::string line;
	std::ifstream input( "input.txt" );
	while(std::getline(input, line)) {
		std::cout << line << std::endl;
	}
}
