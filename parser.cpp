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
30.01.20 - trying to start resolve
31.01.20 - continue with resolving

What's already done with Solver:
1. when question present in the initial facts => return status = TRUE
2. when question not present in the rules => return status = FALSE
3. todo: when one/two rule present
	3.1 think about recursion
	Notes:
	- Each question should be processed separately and should be added to init facts for next questions)
	- Each affected rule should be porcessed separately too
*/
std::string ConvertSetToStr(std::set<char> used_chars);
std::set<char> GetInvolvedChars(std::string str);
bool askQuestion(std::vector<ParsedRuleList> rule_list, std::set<char> init_facts, char quer);
std::string convertToRPN(std::string expression);

std::map<char, bool> resolved_letters;
bool bool_result;

void showstack(std::stack<std::string> toSolve) 
{ 
    while (!toSolve.empty()) 
    {
        std::cout << '\t' << toSolve.top();
        toSolve.pop(); 
    } 
    std::cout << '\n';
}

std::string getlinefromstack(std::stack<std::string> toSolve) 
{ 
	std::string str;
    while (!toSolve.empty()) 
    {
        str = toSolve.top();
        toSolve.pop(); 
    } 
    // std::cout << '\n';
    return str;
} 

bool limitateRightSide(std::vector<ParsedRuleList> rule_list){
	std::vector<ParsedRuleList>::iterator it;

	it = rule_list.begin();
	for (it; it != rule_list.end(); it++){
		for (auto i: it->rside){
			std::cout << "Additional checking: " << i << std::endl;
			if (!(i >= RULES_START && i <= RULES_END) && (i != PLUS) && (i != VOSKL)){
				std::cout << "Error. Not allowed operator or symbol" << '\n';
				std::cout << "Please, use only Operator (+) in the right side according to mandatory requirements" << '\n';
				exit(1);
				}
			}

		}
	std::cout << "Limitation passed - ok" << '\n';
	return true;
}

bool RPNCalculate(bool a, bool b, char op){
	switch (op) {
		case '|': return(a | b);
		case '^': return(a ^ b);
		case '+': return(a + b);
		default: return (a + b);
	}
}

bool SolvingStack(std::stack<std::string> toSolve, std::vector<ParsedRuleList> rule_list, std::set<char> init_facts){
	std::cout << "SolvingStack" << std::endl;
	std::cout << "STACK SIZE: " << toSolve.size() << std::endl;
	//showstack(toSolve);
	std::string i = getlinefromstack(toSolve);
	std::string left_token = i.substr(0, i.find("=>")); // token is "left side of the expression"
	std::set<char> inv_Chars = GetInvolvedChars(left_token);
	std::string right_token = i.substr(i.find("=>") + 2, i.size());
	std::string inv_Chars_str = ConvertSetToStr(inv_Chars);
	std::cout << "INV_CHARS_STR: " << inv_Chars_str << std::endl;
	for (auto z: inv_Chars_str){
		askQuestion(rule_list, init_facts, z);
	}
	std::cout << "DONE WITH INVOLVED CHARS, TRY TO RESOLVE EXPRESSION\n";
	std::string final_status = convertToRPN(left_token);
	std::set<char> inv_Chars_right = GetInvolvedChars(right_token);
	std::string invChRight_str = ConvertSetToStr(inv_Chars_right);

	int first = -1;
	int second = -1;
	int iterat = 1;
	bool result;
	for (auto x: final_status){
		auto search = resolved_letters.find(x);
    	if (search != resolved_letters.end()) {
  			//if std::cout << "FINAL_STATUS " << search->first << " " << search->second << std::endl;
    		if (iterat == 1){
    			first = search->second;
    		}
    		if (iterat == 2){
    			second = search->second;
    		}
    		std::cout << "Iteration : " << iterat << std::endl;
    		std::cout << "FIRST : " << first << std::endl;
    		std::cout << "SECOND : " << second << std::endl;
    		std::cout << "LETTER:VALUE FROM DICT: " << search->first << " " << search->second << std::endl;
    		iterat++;
    	}
    	else{
    		std::cout << "Didn't find char in the resolved letter : " << x << std::endl;
    		if (x == '!'){
    			if (iterat == 2){
    				first = !first;

    			}
    			if (iterat > 2){
    				second = !second;

    			}
	    		std::cout << "Iteration : " << iterat << std::endl;
    			std::cout << "FIRST : " << first << std::endl;
    			std::cout << "SECOND : " << second << std::endl;
    		}
    		else if (x == '+' || x == '^' || x == '|'){
				std::cout << "Calculating RPN" << std::endl;
	    		std::cout << "Iteration : " << iterat << std::endl;
    			std::cout << "FIRST : " << first << std::endl;
    			std::cout << "SECOND : " << second << std::endl;
    			result = RPNCalculate((bool)first, (bool)second, x);
    			std::cout << "RESULT is: " << result << std::endl;
    			iterat=1;
    			x = '\0';
    		}
    	}
	}

	for (auto z: invChRight_str){
		if (inv_Chars.size() == 1){
		//here when only one letter in the left
		//C=>E for example
			std::cout << "When in the left side only one letter\n";
			std::cout << "Adding letter: " << z << " Status: " << (bool)first << std::endl;
			resolved_letters.insert(std::make_pair(z, first)); 
		}
		std::cout << "Adding letter: " << z << " Status: " << (bool)result << std::endl;
		resolved_letters.insert(std::make_pair(z, result)); 
	}

}

bool askQuestion(std::vector<ParsedRuleList> rule_list, std::set<char> init_facts, char quer){
	//std::cout << "Initial facts: " << info_storage.GetterInitFacts() << std::endl;
	std::cout << "\nAsk question" << std::endl;
	std::cout << quer << std::endl;

	//check if Question present in the input facts or not
	std::stack<std::string> toSolve;

	auto search = init_facts.find(quer);
    if (search != init_facts.end()) {
		resolved_letters.insert(std::make_pair(quer, true)); 
        std::cout << "Found in the init facts " << (*search) << "\n\n";
        return true;
    }
    std::cout << "Not found, go find " << '\n';
    

    ///HEERE TO BE CHECKED AT THE EVENING
    auto search1 = resolved_letters.find(quer);
    //########################################################################
    if (search1 != resolved_letters.end()) {
		//resolved_letters.insert(std::make_pair(quer, true)); 
        std::cout << "Found in the resolved letters\n"; //<< (*search1) << "\n\n";
        return true;
    }
    //########################################################################
    std::vector<ParsedRuleList>::iterator it;
	it = rule_list.begin();
    for (it; it != rule_list.end(); it++){
    	// std::cout << "================" << '\n';    	
    	std::cout << it->rside << '\n';
    	// std::cout << it->rside.find(quer) << '\n';

		if (it->rside.find(quer) != std::string::npos){
			std::cout << "Found letter in the right side" << std::endl;
			std::cout << it->rule << std::endl;
			toSolve.push(it->rule);
		}
		else{
			std::cout << "Not found in the right side" << std::endl;			
		}
	}
	if (toSolve.empty()){
		std::cout << "Not found value nor in init facts neither in right side => take default FALSE value" << std::endl;
		resolved_letters.insert(std::make_pair(quer, false)); 
		return true;
	}
	SolvingStack(toSolve, rule_list, init_facts);
	std::cout << std::endl;
	return true;
}

int resolver(std::vector<ParsedRuleList> rule_list, std::set<char> init_facts, std::set<char> querries){
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "NewEpoch" << std::endl;

	limitateRightSide(rule_list);

	for (auto i: querries){
		std::cout << "Main resolver function=====\n";
		askQuestion(rule_list, init_facts, i);
	}
}

int precedence(char x)
{
    if (x == '(') {
        return 0;
    } else if (x != '!') {
        return 1;
    }
    return 2;
}

std::string convertToRPN(std::string expression)// to add &
{
	//expression = "C|!G+Z+(A+B)+D";
    std::stack<char> s;
    std::string postfix;
    for (auto tok : expression) {
        if (std::isupper(tok)) {
            postfix += tok;
        } else if (tok == '(') {
            s.emplace(tok);
        } else if (tok == ')') {
            while (!s.empty()) {
                tok = s.top();
                s.pop();
                if (tok == '(') {
                    break;
                }
                postfix += tok;
            }
        } else {
            while (!s.empty() && precedence(tok) <= precedence(s.top())) {
                postfix += s.top();
                s.pop();
            }
            s.emplace(tok);
        }
    }
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }
    expression = postfix;
    std::cout << "Converted to RPN: " << expression << std::endl;
    return expression;
}

std::set<char> GetInvolvedChars(std::string str){

	std::set<char> used_chars;
	std::set<char> inverted_ch;
//GET LIST WITH INVOLVED CHARS
	for (auto i: str){
		if (i >= LEX_START && i <= LEX_END){
			used_chars.insert(i);
		}
		else
			continue;
	}

/*
// i think we can don't handle char:(!), because it will automatically handled by final expression with true/false.
// 	for (auto i: str){
// 		if (i == VOSKL){
// 			std::cout << "FIND !VOSKL: " << i << std::endl;
// 		}
// 		else
// 			continue;
// 	}
*/

	std::cout << "Ivolved chars in the string" << std::endl;
  	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  std::cout << *it;
	std::cout << std::endl;

	return used_chars;
}

void PrintSet(std::set<char> used_chars){
	std::cout << "PRINT SET, HELLO" << std::endl;
	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  std::cout << *it;
	std::cout << std::endl;
}

std::string ConvertSetToStr(std::set<char> used_chars){
	std::cout << "ConvertSetToStr" << std::endl;
	std::string str = "";
	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  str += *it;
	std::cout << std::endl;
	return str;
}

std::set<char> ConvertToSet(std::string str){
	
	std::set<char> convertedKeys;
	for (auto i: str){
		convertedKeys.insert(i);
	}
	return convertedKeys;
}

std::string ConvertVectorToStr(std::vector<std::string> vector_){
	std::string convertedStr;
	for (auto i: vector_)
		convertedStr += i;
	return convertedStr;		
}

int process_rules(void){
	ParsedRuleList temp; //class to process each line with that you will manage solver
	std::vector<ParsedRuleList> rule_list; //here will be saved all metadata
	std::vector<std::string> facts = rule_manager.getRu(); //here full list of rules

	std::set<char> init_facts = ConvertToSet(info_storage.GetterInitFacts());
	std::set<char> querries =  ConvertToSet(ConvertVectorToStr(data_parser.getterQuerry()));
	
#if DEBUG_SOLVER
	PrintSet(init_facts);	
	int size = facts.size(); //size
	std::cout << "Size: " << facts.size() << std::endl; // size of rule_list, how many cycles will be
#endif

/*	example
	std::cout << temp.isEnd << std::endl;
	rule_list.push_back(temp);
	std::cout << rule_list[0].isEnd << std::endl;
	test.isEnd = true;

*/

	std::string delimiter = "<=>";

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
		//Step 2. split with delimeters and process each line
		temp.rule = i;
		std::string left_token = i.substr(0, i.find(delimiter)); // token is "left side of the expression"
		std::string right_token = i.substr(i.find(delimiter) + 2, i.size());
#if DEBUG_SOLVER
		std::cout << "Str: " << i << std::endl;
		std::cout << "LEFT: " << left_token << std::endl;
		std::cout << "RIGHT: " << right_token << std::endl;
		std::cout << std::endl;
#endif
		std::set<char> left_inv_ch = GetInvolvedChars(left_token);
		std::set<char> right_inv_ch = GetInvolvedChars(right_token);

		temp.invL = left_inv_ch;
		temp.invR = right_inv_ch;

		PrintSet(temp.invL);
		PrintSet(temp.invR);

		temp.lside = convertToRPN(left_token);
		temp.rside = convertToRPN(right_token);

		std::cout << temp.lside << std::endl;
		std::cout << temp.rside << std::endl;
		std::cout << "===================================" << std::endl;

		//PUT THIS TO THE GLOBAL CONTAINER
		rule_list.push_back(temp);
	}
	// std::vector<ParsedRuleList>::iterator it;
	// it = rule_list.begin();
	//std::cout << "YAAAAAAAAAAAAAZ" << it->rside << std::endl;

	resolver(rule_list, init_facts, querries);
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

 
