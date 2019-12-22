#include "expert_system.hpp"

class InputManager input_data;
class DataManager data_parser;


int parsing(void){
	data_parser.setterCL(input_data.getterCL());
	data_parser.print_data();
	/*for (auto str: data_parser.getterCL()) {
		data_parser.getRules(str);
	}*/
	data_parser.dataType();
}

int main(int ac, char **av){
	try{
		if (ac == 2) {
			input_data.get_input_file(std::string(av[1]));
			input_data.line_cleaner();
			//input_data.print_cleaned_lines();
			parsing();
		}
		else{
			throw std::invalid_argument( "received invalid numbers of arguements" );
		}
	}
	catch( const std::invalid_argument& e ) {
		    std::cout << "ERROR. Invalid numbers of arguement\n";
		 }
	catch (char const * line){
		std::cout << line << std::endl;
	}
	return (0);
}

