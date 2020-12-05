#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

map<char, char> binmap = {{'F','0'}, {'B','1'}, {'L','0'}, {'R','1'}};

// Applies binmap to convert string of e.g. FFBBFFFBLRL -> 00110001010
string letters_to_binary(string * instr){
	string outstr = "";
	for (char const c : *instr){
		outstr += binmap[c];
	}
	return outstr;
}

/* Boarding passes are already in binary form, just switched
   from 0,1 to F,B,L,R. So convert the seat number to binary,
   split it into an 8 digit (row) & 3 digit (seat number in row)
*/
int main()
{
	ifstream infile("input.txt");
	string str, line, eg;
	int row_no, col_no, seat_id, max_id=0;
	vector<int> all_seats;

	while(getline(infile, line)){

		str = letters_to_binary(&line);

		// Binary to int
		row_no = stoi(str.substr(0, 7), 0, 2);
		col_no = stoi(str.substr(7), 0, 2);

		seat_id = row_no * 8 + col_no;

		// Add to vector for Part 2
		all_seats.push_back(seat_id);

		// Part 1 answer - the max seat ID
		max_id = (seat_id > max_id) ? seat_id : max_id;
		
	}

	cout << "Part 1, Max ID: " << max_id << endl;

	// My seat ID is between two occupied seat IDs
	// So, sort the vector & look for a gap of 2.
	sort(all_seats.begin(), all_seats.end());
	for(int i = 0; i < all_seats.size() - 1; i++){
		if(all_seats.at(i+1) - all_seats.at(i) == 2)
		{
			cout << "Part 2, My seat is: " << 
				all_seats.at(i) + 1 << endl;
			break;
		}
	}
	
	return 0;
}
