#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
	ifstream infile("input.txt");
	string line, password;
	int hyph, colon, min_c, max_c, occ, valid1=0, valid2=0;
	char lett;
	bool char1, char2, verbose = false;

	while(getline(infile, line)){
		if(verbose) cout << line << endl;

		// Use locations of - and : to parse ints & letter
		hyph = line.find('-');
		colon = line.find(':');

		min_c = stoi(line.substr(0, hyph));
		max_c = stoi(line.substr(hyph+1, colon-3));
		lett = line.at(colon-1);

		// Password is colon+2 : end (implicit)
		password = line.substr(colon+2);
		occ = count(password.begin(), password.end(), lett);

		if(verbose){
		cout << min_c << "  "  << max_c << " " << lett << " " << password << endl;
		cout << lett << "  occurs  " << occ << " times in  " << password  << endl;
		}


		if(verbose) cout << char1 << " " << char2 << endl;
		// increment part 1 count if valid
		if(occ <= max_c && occ >= min_c) valid1++;

		// Part 2: interpret min_c and max_c as 1-indexed positions in password
		// Valid passwords have lett in exactly 1 of these positions
		char1 = password.at(min_c-1) == lett;
		char2 = password.at(max_c-1) == lett;
		if(char1 != char2) valid2++;
	}

	cout << "Part 1 answer is: " << valid1 << " valid passwords." << endl;
	cout << "Part 2 answer is: " << valid2 << " valid passwords." << endl;
	return 0;
}
