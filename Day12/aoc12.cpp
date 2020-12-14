#include <vector>
#include <iostream>
#include <fstream>
#include <string>

typedef std::pair<char, int> dir_t;
typedef std::pair<int, int> coord_t;

using std::cout;
using std::endl;

// Make a single 90 degree turn (right or left)
void turn(coord_t &orient, bool right){
	coord_t orient_temp;
	if(right){
		orient_temp.first = orient.second;
		orient_temp.second = - orient.first;
	}else{
			orient_temp.first = -orient.second;
			orient_temp.second =  orient.first;
	}
	orient = orient_temp;
}

// Move the ship (Part 1) or the waymarker (Part 2)
void move(coord_t &pos, coord_t &orient, dir_t &instr, bool part1){
	switch(instr.first){
		case 'F': // Move a multiple of current orientation
			pos.first += orient.first * instr.second;
			pos.second += orient.second * instr.second;
			break;

		case 'R': // Transform orientation 90 degrees
			for(int i = 0; i < instr.second / 90; i++){
				turn(orient, true);
			}
			break;

		case 'L':
			for(int i = 0; i < instr.second / 90; i++){
				turn(orient, false);
			}
			break;

		case 'N': // Move fixed direction, leave orient
			if(part1){
				pos.second += instr.second;
			}else{
				orient.second += instr.second;
			}
			break;

		case 'S':
			if(part1){
				pos.second -= instr.second;
			}else{
				orient.second -= instr.second;	
			}
			break;

		case 'E':
			if(part1){
				pos.first += instr.second;
			}else{
				orient.first += instr.second;
			}
			break;

		case 'W':
			if(part1){
				pos.first -= instr.second;
			}else{
				orient.first -= instr.second;
			}
			break;
	}
}

int main()
{
	std::ifstream infile("input.txt");
	std::vector<dir_t> directions;
	dir_t curr_dir;
	coord_t curr_coord, orient;
	std::string line;

	while(getline(infile, line)){
		curr_dir.first = line.at(0);
		curr_dir.second = stoi(line.substr(1));
		directions.push_back(curr_dir);
	}

	// Begin at origin, facing east
	orient = {1, 0};
	curr_coord = {0, 0};

	// Move ship per instruction
	for(auto instr : directions){
		cout << "Direction " << instr.first;
		cout << " count : " << instr.second << endl;
		move(curr_coord, orient, instr, true);
	}

	// Compute manhattan distance
	int manhat = abs(curr_coord.first) + 
		abs(curr_coord.second);
	cout << "Part 1 answer : dist = " << manhat << endl;


	// Part 2: start at origin, waymarker 10,1
	orient = {10, 1};
	curr_coord = {0, 0};

	for(auto instr : directions){
		cout << "Direction " << instr.first;
		cout << " count : " << instr.second << endl;
		move(curr_coord, orient, instr, false);
	}

	manhat = abs(curr_coord.first) + 
		abs(curr_coord.second);
	cout << "Part 2 answer : dist = " << manhat << endl;

	return 0;
}
