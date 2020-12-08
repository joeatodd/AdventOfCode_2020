#include <iostream>
#include <fstream>
#include <vector>
#include "assembly.h"

using namespace std;

int main()
{
	typedef pair<string, int> opcode_t;
	ifstream infile("input.txt");
	string line, oc;
	vector<opcode_t> opcodes;
	int num_codes;

	// Create an assembly machine
	AssemblyMachine ass_mach("input.txt");

	// Run till failure, answer is value of 'acc'
	int status = ass_mach.run();
	cout << "Part 1 answer : " << ass_mach.get_acc() << std::endl;
	cout << "Machine status: " << status << std::endl;

	opcodes = ass_mach.get_codes(true);
	num_codes = opcodes.size();

	for(int i = 0; i < num_codes; i++){

		ass_mach.reset();

		// Swap (if possible) or cycle
		if(opcodes.at(i).first == "nop"){
			ass_mach.set_code(i, "jmp");
		}else if(opcodes.at(i).first == "jmp"){
			ass_mach.set_code(i, "nop");
		}else{
			continue;
		}

		status = ass_mach.run();
		if(status == 0){
			std::cout << "Part 2 answer: " << ass_mach.get_acc() << std::endl;
			break;
		}
	}

	return 0;
}
