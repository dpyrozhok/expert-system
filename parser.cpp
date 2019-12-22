#include "expert_system.hpp"


std::string remove_comments_from_string(std::string str) {
	std::size_t found=str.find('#');

	if (found != -1) {
		str.resize(found);
		//std::cout<<found<<" "<<str<<std::endl;
	}
	return str;
}

int main(int ac, char **av){
	std::string line;
	std::ifstream input( "input.txt" );
	while(std::getline(input, line)) {
		//std::cout << line << std::endl;
		line = remove_comments_from_string(line);
		if (line.size() > 0){
		std::cout << line << std::endl;}
	}

}
