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
std::string ConvertVectorToStr(std::vector<std::string> vector_);
bool check_right_inversion(char z,std::string inversed_rSide);


std::map<char, bool> resolved_letters;
bool bool_result;
bool checkiftrue(char z);
bool checkifInversed(char ch, std::map<char, bool> LocalInversedChars);

void showstack(std::stack<std::string> toSolve) 
{ 
    while (!toSolve.empty()) 
    {
        std::cout << '\t' << toSolve.top();
        toSolve.pop(); 
    } 
    std::cout << '\n';
}
void showstackchar(std::stack<char> toSolve) 
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
    return str;
} 

bool limitateRightSide(std::vector<ParsedRuleList> rule_list){
	std::vector<ParsedRuleList>::iterator it;

	it = rule_list.begin();
	for (it; it != rule_list.end(); it++){
		for (auto i: it->rside){
#if DEBUG_SOLVER
			std::cout << "Additional checking: " << i << std::endl;
#endif
			if (!(i >= RULES_START && i <= RULES_END) && (i != PLUS) && (i != VOSKL)){
				std::cout << "Error. Not allowed operator or symbol in the right side according to mandatory requirements" << '\n';
				exit(1);
				}
			}

		}
#if DEBUG_SOLVER
	std::cout << "Limitation passed - ok" << '\n';
#endif

	return true;
}

bool RPNCalculate(bool a, bool b, char op){
	switch (op) {
		case '|': return(a | b);
		case '^': return(a ^ b);
		case '+': return(a & b);
		default: return (a & b);
	}
}

int getValueFromDict(std::map<char, bool> dict, char letter){
	auto ins = dict.find(letter); 
    if (ins != dict.end()) {
    	return ins->second;
    }	
    return -1;
}

void SolvingStack(std::string toSolve, std::vector<ParsedRuleList> rule_list, std::set<char> init_facts){
#if DEBUG_SOLVER
	std::cout << "Solving rule from stack" << std::endl;
#endif	
	std::string i = toSolve;
	std::string left_token = i.substr(0, i.find("=>")); // token is "left side of the expression"
	std::set<char> inv_Chars = GetInvolvedChars(left_token);
	std::string right_token = i.substr(i.find("=>") + 2, i.size());
	std::string inv_Chars_str = ConvertSetToStr(inv_Chars);

#if DEBUG_SOLVER	
	std::cout << "INV_CHARS_STR: " << inv_Chars_str << std::endl;
#endif

	for (auto z: inv_Chars_str){
		askQuestion(rule_list, init_facts, z);
	}

#if DEBUG_SOLVER
	std::cout << "DONE WITH INVOLVED CHARS, TRY TO RESOLVE EXPRESSION\n";
#endif
	std::string final_status = convertToRPN(left_token);
	std::set<char> inv_Chars_right = GetInvolvedChars(right_token);
	std::string invChRight_str = ConvertSetToStr(inv_Chars_right);
	std::string inversed_rSide = right_token;//convertToRPN(right_token);

#if PRINT_STEPS
	std::cout << "Processing rule: " << i << std::endl;
#endif

	std::stack<char> LeftSideStack; //to calculate left side
	std::map<char, bool> LocalInversedChars; //temporary store of the inversed values in the left side
	bool result;
	
	for (auto x: final_status){
		if (std::isupper(x) != 0){
			//add letter to the stack
			LeftSideStack.emplace(x);
#if DEBUG_RPN_CALCULATING
    		std::cout << "Adding char to the stack " << x << std::endl;
#endif
    		continue;
		}

		else if (std::ispunct(x) != 0){
			//calculate last two letter from stack and save result to the stack
			if (x == '!'){
				//exception with '!' to inverse last symbol in the stack and check next symbol
				char letter = LeftSideStack.top(); //get last letter from stack
				bool inversed_value;

    			LeftSideStack.pop(); //delete last letter from stack
    			
    			auto ins = resolved_letters.find(letter); //try to find letter in the resolved letter
    			if (ins != resolved_letters.end()) {

#if DEBUG_RPN_CALCULATING
    				std::cout << "Found letter in the resolved dic{Letter: " << ins->first << " , " << "Value: " << ins->second << "}" << std::endl;
#endif
    				inversed_value = ins->second;

					auto inv_dict = LocalInversedChars.insert(std::make_pair(letter, !inversed_value )); //try to add to the special dict
					inv_dict.second = !inversed_value; //inverse value and put on special dictionary

#if DEBUG_RPN_CALCULATING
    				std::cout << "Inverse letter added to the special dic{Letter: " << inv_dict.first->first << " , " << "Value: " << inv_dict.first->second << " }" << std::endl;
#endif

    				LeftSideStack.emplace(letter);

#if DEBUG_RPN_CALCULATING
    				std::cout << "Also adding back symbol after inversion checking to the stack: " << letter << std::endl;
#endif

				}

#if DEBUG_RPN_CALCULATING
				else{
					std::cout << "UNEXPECTED BEHAVIOR. Not found letter in the resolved dic" << std::endl;
				}
#endif
    			continue; //switch to the next character from expression
			}
			//others operators
			//check how many letter already on stack, maybe if only 1 => missmatch somewhere
			char operand1;
			char operand2;
			bool value_operand1;
			bool value_operand2;

#if DEBUG_RPN_CALCULATING
    				std::cout << "Trying to calculate two values from stack" << std::endl;
#endif
			if (LeftSideStack.size() == 1){
#if DEBUG_RPN_CALCULATING
				std::cout << "UNEXPECTED BEHAVIOR. Only one letter on stack and requested calculations\n";
				showstackchar(LeftSideStack);
				std::cout << "PRINTED STACK";
#endif
			}
			if (LeftSideStack.size() >= 2){

				operand1 = LeftSideStack.top(); // get last letter from stack
				LeftSideStack.pop(); //delete it 

				operand2 = LeftSideStack.top(); // get last-1 letter from stack
				LeftSideStack.pop(); //delete it
				
				if (getValueFromDict(LocalInversedChars, operand1) != -1){
				//try to check first operand in the inv dict	
					value_operand1 = getValueFromDict(LocalInversedChars, operand1);
				}
				else{
				//else process as usual from resolved dict
					value_operand1 = getValueFromDict(resolved_letters, operand1);
				}

				if (getValueFromDict(LocalInversedChars, operand2) != -1){
				//try to check second operand in the inv dict	
					value_operand2 = getValueFromDict(LocalInversedChars, operand2);
				}
				else{
				//else process as usual from resolved dict
					value_operand2 = getValueFromDict(resolved_letters, operand2);
				}
#if DEBUG_RPN_CALCULATING
				std::cout << "Perform calculation: " << operand1 << " with " << "value " << value_operand1 << " action: " << x << " and " \
				<< operand2 << " with " << "value: " << value_operand2 << " " << std::endl;
#endif
				result = RPNCalculate(value_operand1, value_operand2, x); // x is current char from line (operand)
#if DEBUG_RPN_CALCULATING
				std::cout << "Result after two operand calculation: " << result << std::endl;
				std::cout << "Here should be saved result\n";
#endif
				LeftSideStack.emplace('r');
				auto rewrite = resolved_letters.insert(std::make_pair('r', result));
				rewrite.first->second = result;
    			}
			}
		}
#if DEBUG_RPN_CALCULATING
    std::cout << "Done with calculating and try to update all chars from right side" << std::endl;
#endif
	char ch = LeftSideStack.top(); // get last letter from stack
	LeftSideStack.pop(); //delete it 
#if DEBUG_RPN_CALCULATING
    std::cout << "Get char from stack: " << ch << std::endl;
#endif
	auto search45 = resolved_letters.find(ch);
	result = search45->second;
#if DEBUG_RPN_CALCULATING
    std::cout << "Get value according to last char from stack: " << ch << " " << "Value: " << result << std::endl;
#endif
   	if (ch != 'r' && checkifInversed(ch, LocalInversedChars)){
#if DEBUG_RPN_CALCULATING
   		std::cout << "Alredy got letter: " << ch << " with value: " << result  << " => requested inversion so change value to " << !result << " |" << std::endl;
#endif
   		result = !result;
   	}

	for (auto involved: invChRight_str){
//updating all letters from right side
		if (checkiftrue(involved)){
		//skip character if already status TRUE present in the resolved list
			continue;
		}
		auto rewrite1 = resolved_letters.insert(std::make_pair(involved, result));
		rewrite1.first->second = result;
#if DEBUG_RPN_CALCULATING
  	  std::cout << "Updated needed value in the resolved list: " << involved << " " << "value: " << result << std::endl;
#endif
	}

}

bool checkifInversed(char ch, std::map<char, bool> LocalInversedChars){
#if DEBUG_RPN_CALCULATING
    std::cout << "Only one operand and no calculations, check whether inversion requested" << std::endl;
#endif
	auto check_dict = LocalInversedChars.find(ch);
 	if (check_dict != LocalInversedChars.end()) {
#if DEBUG_RPN_CALCULATING
		std::cout << "Requested inversion, please reverse value from resolved dicitionary " << std::endl;
#endif
		return true;
	}
	return false;
}

bool checkiftrue(char z){
	auto check_dict = resolved_letters.find(z);
  
 	if (check_dict != resolved_letters.end()) {
#if DEBUG_RPN_CALCULATING
 	 std::cout << "==============> Found in dict " << check_dict->first << " " << check_dict->second << '\n';
#endif
    if (check_dict->second == 1){
#if DEBUG_RPN_CALCULATING
	       std::cout << "Found in dict TRUE and ignore possible FALSE: " << check_dict->first << " " << check_dict->second << '\n';
#endif	       
	       return true;
    	}
	}
    return false;
}

bool check_right_inversion(char z, std::string inversed_rSide){
	int pos = inversed_rSide.find(z);
	if (pos != std::string::npos){
		if (inversed_rSide[pos + 1] == '!')
			return true;
	}
	return false;
}


bool askQuestion(std::vector<ParsedRuleList> rule_list, std::set<char> init_facts, char quer){
#if PRINT_STEPS
	std::cout << "\nAsk question" << std::endl;
	std::cout << quer << std::endl;
#endif
	//check if Question present in the input facts or not
	std::stack<std::string> toSolve;

	auto search = init_facts.find(quer);
    if (search != init_facts.end()) {
		resolved_letters.insert(std::make_pair(quer, true)); 
#if PRINT_STEPS
        std::cout << "Found in the init facts " << (*search) << "\n\n";
#endif
        return true;
    }
    

    ///HEERE TO BE CHECKED AT THE EVENING
    auto search1 = resolved_letters.find(quer);
    //########################################################################
    if (search1 != resolved_letters.end()) {
		//resolved_letters.insert(std::make_pair(quer, true)); 
#if PRINT_STEPS
        std::cout << "Found in the resolved letters "<<search1->first<<" = true\n"; //<< (*search1) << "\n\n";
#endif
        return true;
    }
#if PRINT_STEPS
    std::cout << "Not found in dictionary." << '\n';
#endif
    //########################################################################
    std::vector<ParsedRuleList>::iterator it;
	it = rule_list.begin();
    for (it; it != rule_list.end(); it++){
    	// std::cout << "================" << '\n';    
#if DEBUG_SOLVER	
    	std::cout << it->rside << '\n';
#endif
    	// std::cout << it->rside.find(quer) << '\n';

		if (it->rside.find(quer) != std::string::npos){
			if (it->is_used == true){
                throw("Error. Find internal loop in the rule list");
				exit(777);
			}
			it->is_used = true;
#if PRINT_STEPS
			std::cout << "Found letter in the right side" << std::endl;
			std::cout << it->rule << std::endl;
#endif
			toSolve.push(it->rule);
		}
#if DEBUG_SOLVER
		else{
			std::cout << "Not found in the right side" << std::endl;			
		}
#endif
	}
	if (toSolve.empty()){
#if PRINT_STEPS
		std::cout << "Not found value nor in init facts neither in right side => take default FALSE value" << std::endl;
#endif
		resolved_letters.insert(std::make_pair(quer, false)); 
		return true;
	}
	std::string str;
    while (!toSolve.empty()) 
    {
    	str = toSolve.top();
    	toSolve.pop(); 
		SolvingStack(str, rule_list, init_facts);
    }
#if PRINT_STEPS
	std::cout << std::endl;
#endif
	return true;
}

void giveAnswer(std::string querries){//?ABG
	for (auto i: querries){
		auto search = resolved_letters.find(i);
    	if (search != resolved_letters.end()) {
    		std::cout << "\e[40mEXPERT ANSWER: " << "\e[37mLETTER: \e[44m" << i << "\e[40m" \
    		<< "  \e[40mSTATUS:" << (((bool)(search->second)) ? " \e[42mTRUE\e[49m" : " \e[41mFALSE\e[49m");
    		std::cout << std::endl;
    		//save if in macos not support that colors
		   	// std::cout << "EXPERT ANSWER:\n" << "LETTER: " << i << " " \
    		// << "  STATUS:" << (((bool)(search->second)) ? " TRUE" : " FALSE") << std::endl;
		}
	}
}

int resolver(std::vector<ParsedRuleList> rule_list, std::set<char> init_facts, std::set<char> querries){
#if DEBUG_SOLVER
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "NewEpoch" << std::endl;
#endif
	limitateRightSide(rule_list);

	for (auto i: querries){
#if DEBUG_SOLVER
		std::cout << "Main resolver function=====\n";
#endif

		askQuestion(rule_list, init_facts, i);
	}
	giveAnswer(ConvertVectorToStr(data_parser.getterQuerry()));
}

int Priority(char c)
{
    if(c == '!') {
        return 4;
    } else if(c == '+') {
        return 3;
    } else if(c== '|' ) {
        return 2;
    } else if(c== '^' ) {
        return 1;
    } else {
        return 0;
    }
}

bool isOperator(char c)
{
    return (c == '+' || c == '!' || c == '|' || c == '^');
}
std::string convertToRPN(std::string tokens)
{
    //std::string tokens = "A|B+C";//our infix expression
    std::vector<char> outputList;//output vector
    std::stack<char> s;//main stack


    for(unsigned int i = 0; i < tokens.size(); i++)
    {
        if(std::isupper(tokens[i]))
        {
            outputList.push_back(tokens[i]);
        }
        if(tokens[i] == '(')
        {
            s.push(tokens[i]);
        }
        if(tokens[i] == ')')
        {
            while(!s.empty() && s.top() != '(')
            {
                outputList.push_back(s.top());
                s.pop();
            }
            s.pop();
        }
        if(isOperator(tokens[i]) == true)
        {
            while(!s.empty() && Priority(s.top()) >= Priority(tokens[i]))
            {
                outputList.push_back(s.top());
                s.pop();
            }
            s.push(tokens[i]);
        }
    }
    //pop any remaining operators from the stack and insert to outputlist
    while(!s.empty())
    {
        outputList.push_back(s.top());
        s.pop();
    }

    tokens = "";
    for(unsigned int i = 0; i < outputList.size(); i++)
    {
        tokens +=outputList[i];
    }
#if DEBUG_RPN
    std::cout<<"\nRPN ==============="<<tokens<<std::endl;
#endif
    return tokens;
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
#if DEBUG_SOLVER
	std::cout << "Ivolved chars in the string" << std::endl;
  	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  std::cout << *it;
	std::cout << std::endl;
#endif
	return used_chars;
}

void PrintSet(std::set<char> used_chars){
#if  DEBUG_SOLVER
	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  std::cout << *it;
	std::cout << std::endl;
#endif
}

std::string ConvertSetToStr(std::set<char> used_chars){
	#if  DEBUG_SOLVER
	std::cout << "ConvertSetToStr" << std::endl;
	#endif
	std::string str = "";
	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  str += *it;
#if  DEBUG_SOLVER
	std::cout << std::endl;
#endif
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
			std::cout << "Error. Bidirectional not supported. That's bonus part" << std::endl;
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
#if DEBUG_SOLVER

		PrintSet(temp.invL);
		PrintSet(temp.invR);
#endif
		temp.lside = left_token;
		temp.rside = right_token;//convertToRPN(right_token);

#if DEBUG_SOLVER

		std::cout << temp.lside << std::endl;
		std::cout << temp.rside << std::endl;
		std::cout << "===================================" << std::endl;
#endif
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

if (!data_parser.getterFacts().empty()) {

	info_storage.setterInitF(data_parser.getterFacts());

	info_storage.ApplyInitFacts();
#if DEBUG_PARSING
	std::cout << "======================================\n";
	std::cout << "Applied init facts to the dictionary\n";
	std::cout << "======================================\n";
	info_storage.print_alpha();
#endif
}
#if DEBUG_PARSING
else {
std::cout << "No facts\n";
}
#endif

	//convertToRPN("C|!G+Z+(A+B)+D");
if (data_parser.getterQuerry().empty())
	std::cout << "Error. No querries\n";
else
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
		std::cout <<"\e[41m"<<line<<"\e[49m"<<std::endl;
	}

	return (0);
}

 
