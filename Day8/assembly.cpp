#include <fstream>
#include <string.h>
#include <stdio.h>
#include "assembly.h"

AssemblyMachine::AssemblyMachine(std::string filename){
	std::ifstream infile(filename);
	std::string line; // could oc be char[3]?

	std::string oc;
	int val;

	while(getline(infile, line)){
		oc = line.substr(0,3);
		val = stoi(line.substr(4));
		opcodes.push_back(std::make_pair(oc, val));
		visit_count.push_back(0);
	}
	opcodes_init = opcodes;
}

// Reset the machine to initial state
void AssemblyMachine::reset(){

	curr_idx = 0;
	acc = 0;
	fill(visit_count.begin(), visit_count.end(), 0);
	opcodes = opcodes_init;

}

// Run the machine (return 0 if finished successfully, 1 otherwise)
int AssemblyMachine::run(bool verbose){

	opcode_t oc_pair;
	std::string oc;
	int val;


	while(true){
		oc_pair = opcodes.at(curr_idx);
		oc = oc_pair.first;
		val = oc_pair.second;

		visit_count.at(curr_idx) += 1;
		if(visit_count.at(curr_idx) > 1){
			return 1;
		}

		if(oc == "acc"){

			acc += val;
			curr_idx += 1;

		}else if(oc == "jmp"){
			curr_idx += val;

		}else if(oc == "nop"){
			curr_idx += 1;

		}else{
			throw std::logic_error("Bad code");
		}

		// Return if at end of code
		if(curr_idx >= opcodes.size()){ return 0;}
	}

}

int AssemblyMachine::get_val(int idx){
	return opcodes.at(idx).second;
}

std::string AssemblyMachine::get_code(int idx){
	return opcodes.at(idx).first;
}

void AssemblyMachine::set_code(int idx, std::string code){
	opcodes.at(idx).first = code;
}

void AssemblyMachine::set_val(int idx, int val){
	opcodes.at(idx).second = val;
}

std::vector<opcode_t> AssemblyMachine::get_codes(bool initial){
	return (initial) ? opcodes_init : opcodes;
}

int AssemblyMachine::get_acc(){return acc;}
