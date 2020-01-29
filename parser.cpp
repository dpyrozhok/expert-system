#include "expert_system.hpp"

class InputManager input_data;
class SyntaxChecker data_parser;
class DecisionManager info_storage;
class RuleManager rule_manager;
class ParsedRuleList depend_man;


/*

WHAT YOU SHOULD DO?

PARSE RULES AND PROCESS THEM

FEW NOTES:
1. At right side you will have something like 
	=> LETTER + LETTER and maybe INVERSION(!) 
	=> LETTER_A + !LETTER_B and result will affect both letters

2. INIT facts will be same to the end of the program run

3. When you will see contradictions 
	blabla => A == FALSE
	blabla => A == TRUE
	IF YOU LETTER == TRUE, ANYWAY YOU FINAL RESULT WILL BE TRUE, don't take into account FALSE 

29.01.20 -> stopped at process rules

	// example
	// test1.convertToRPN("C|!G+Z+(A+B)+D");
	// rule_list.push_back(test1);
	// std::cout << rule_list[0].rule;

*/
int process_rules(void){
	std::string init_facts = info_storage.GetterInitFacts();
	std::cout << "Init facts that should be true: " << init_facts << std::endl; //truth to start
	std::vector<ParsedRuleList> rule_list; //conatiner with class objects(ParsedRulesList)
	ParsedRuleList test; //class to process each line with that you will manage solver

	std::vector<std::string> facts = rule_manager.getRu(); //here full list of rules
	std::string delimiter = "<=>";
#if DEBUG_SOLVER

	for (auto i: facts){
		//Step 1.
		//Check if bidirectional -> if true -> decline and except error
#if DEBUG_SOLVER
		std::cout << i << std::endl;
#endif
		// std::cout << i.find(delimiter) << std::endl;  // position in the line
		if (i.find(delimiter) != std::string::npos){
			std::cout << "Bidirectional not supported. That's bonus part" << std::endl;
			exit(2);
		}
#if DEBUG_SOLVER
		else{
		//	std::cout << "Rule is onedirectional - passed" << std::endl;
		}
#endif
	}

	delimiter = "=>";

	for (auto i: facts){
		//Step 2. split with => lines
		std::string token = i.substr(0, i.find(delimiter)); // token is "scott"
#if DEBUG_SOLVER
		std::cout << token << std::endl;
#endif
		/*
		//std::cout << test.convertToRPN(left side to solve) << std::endl;
		//std::cout << test.convertToRPN(right side to solve// will be affected all variables included here) << std::endl;
		*/

	}
}	

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
	//convertToRPN("C|!G+Z+(A+B)+D");
	process_rules();
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

 
