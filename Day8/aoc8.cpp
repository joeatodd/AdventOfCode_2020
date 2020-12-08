#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	typedef pair<string, int> opcode_t;
	ifstream infile("input.txt");
	string line, oc;
	vector<opcode_t> opcodes, opcodes_init;
	vector<int> visit_count;
	int acc = 0, val;

	while(getline(infile, line)){
		oc = line.substr(0,3);
		val = stoi(line.substr(4));
		opcodes.push_back(make_pair(oc, val));
		visit_count.push_back(0);
	}
	opcodes_init = opcodes;


	int curr_idx = 0;
	opcode_t oc_pair;
	while(true){
		oc_pair = opcodes.at(curr_idx);
		oc = oc_pair.first;
		val = oc_pair.second;
		// cout << oc << "  " << val << endl;
		visit_count.at(curr_idx) += 1;
		if(visit_count.at(curr_idx) > 1){
			cout << "Part 1 ans: " << acc << endl;
			break;
		}

		if(oc == "acc"){

			acc += val;
			curr_idx += 1;

		}else if(oc == "jmp"){
			curr_idx += val;

		}else if(oc == "nop"){
			curr_idx += 1;

		}else{
			throw;
		}
	}

	int num_codes = opcodes.size();
	bool done = false;
	for(int i = 0; i < num_codes; i++){
		
		opcodes = opcodes_init;
		acc = 0;
		curr_idx = 0;
		fill(visit_count.begin(), visit_count.end(), 0);

		if(opcodes.at(i).first == "nop"){
			opcodes.at(i).first = "jmp";
		}else if(opcodes.at(i).first == "jmp"){
			opcodes.at(i).first = "nop";
		}else{continue;}


		int curr_idx = 0;
		opcode_t oc_pair;
		// cout << "Trying " << i << "  " << opcodes.at(i).first << endl;
		while(true){
			if(curr_idx >= num_codes){
				cout << "Part 2 ans: " << acc << endl;
				done = true;
				break;
			}

			oc_pair = opcodes.at(curr_idx);
			oc = oc_pair.first;
			val = oc_pair.second;
			// cout << oc << "  " << val << endl;

			visit_count.at(curr_idx) += 1;

			if(visit_count.at(curr_idx) > 1){
				break;
			}

			if(oc == "acc"){

				acc += val;
				curr_idx += 1;

			}else if(oc == "jmp"){
				curr_idx += val;

			}else if(oc == "nop"){
				curr_idx += 1;

			}else{
				throw;
			}
		}
		if(done) break;

	}

	return 0;
}
