#include "expert_system.hpp"

class InputManager input_data;
class SyntaxChecker data_parser;
class DecisionManager info_storage;
class RuleManager rule_manager;

int parsing(void){
	data_parser.setterCL(input_data.getterCL());
	data_parser.dataType();
	rule_manager.setterRulesList(data_parser.getterRules());
#if DEBUG_RULES_WORK
	rule_manager.printRules();
#endif
#if DEBUG_PARSING
	std::cout << "======================================\n";
	std::cout << "Inititialize final dict\n";
	std::cout << "======================================\n";
	info_storage.print_alpha();
#endif
	info_storage.setterInitF(data_parser.getterFacts());

	info_storage.ApplyInitFacts();
#if DEBUG_PARSING
	std::cout << "======================================\n";
	std::cout << "Applied init facts to the dictionary\n";
	std::cout << "======================================\n";
	info_storage.print_alpha();
#endif


}

int main(int ac, char **av){
	try{
		if (ac == 2) {
			input_data.get_input_file(std::string(av[1]));
			input_data.line_cleaner();
#if DEBUG_READING
		input_data.print_cleaned_lines();
#endif
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

 
