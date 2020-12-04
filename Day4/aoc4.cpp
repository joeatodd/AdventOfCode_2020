#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

typedef map<string, string> passport_t;
vector<string> passport_keys = {"byr","iyr","eyr","hgt","hcl","ecl","pid","cid"};


bool validate_passport_pt1(passport_t passport){
	for(int j=0; j<7; j++){ // ignore 8th entry (cid)
		if(passport.find(passport_keys[j]) == passport.end()){
			return false;
		}}
	return true;
}

bool validate_passport_pt2(passport_t passport, bool verbose=false){
	vector<string> haircols = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
	int len, height;
	bool centi, inches, invalid;
	string val;
		

	invalid = false;
	for(int j=0; j<7; j++){
		if(passport.find(passport_keys[j]) == passport.end()){
			invalid = true;
		}else{
			val = passport[passport_keys[j]];
			switch(j){
			case 0: // byr
				if(stoi(val) < 1920 || stoi(val) > 2002) invalid = true;
				if(verbose) cout << " byr " << val << " valid " << !invalid << endl;
				break;
			case 1: // iyr
				if(stoi(val) < 2010 || stoi(val) > 2020) invalid=true;
				if(verbose) cout << " iyr " << val << " valid " << !invalid << endl;
				break;
			case 2: // eyr
				if(stoi(val) < 2020 || stoi(val) > 2030) invalid=true;
				if(verbose) cout << " eyr " << val << " valid " << !invalid << endl;

				break;
			case 3: // hgt
				len = val.size();
				centi = val.substr(len-2) == "cm";
				inches = val.substr(len-2) == "in";

				if(centi){
					height = stoi(val.substr(0,len-2));
					if(height < 150 || height > 193){invalid=true;}
				}else if(inches){
					height = stoi(val.substr(0,len-2));
					if(height < 59 || height > 76){invalid=true;}
				}else{
					invalid = true;
				}

				if(verbose) cout << " hgt " << val << " valid " << !invalid << endl;
				break;
			case 4: // hcl
				if(val.size() != 7){invalid=true; break;}
				for(int k=1; k<7; k++){
					if(!isxdigit(val[k])){invalid=true;}
				}
				if(verbose) cout << " hcl " << val << " valid " << !invalid << endl;
				break;
			case 5: // ecl
				if(find(haircols.begin(), haircols.end(), val) == haircols.end()) invalid = true;
				if(verbose) cout << " ecl " << val << " valid " << !invalid << endl;
				break;
			case 6: // pid
				if(val.size()!=9){
					invalid=true;
				}else{
					for(int k=0; k<9; k++){
						if(!isdigit(val[k])){invalid=true; break;}
					}
				}
				if(verbose) cout << " pid " << val << " valid " << !invalid << endl;
				break;
			}
			if(invalid){
				break;
			}
		}
	}
	return !invalid;
}

int main()
{
	fstream infile("input.txt");
	string line, keypair, val, key;
	vector<passport_t> passports;
	passport_t current;
	int valid_count_1 = 0, valid_count_2 = 0;
	bool verbose=true, new_passport;

	/* Input is an untidy mix of formatting, where only a 
	   blank line consistently indicates a new passport.
	   Need to continuously parse file, pushing passport
	   to 'passports' vector when a blank line is encountered
	   (or after finished looping)
	*/
	while(getline(infile, line)){

		// Reset things if we're on a new passport (blank line)
		new_passport = (line.empty()) ? true : false;
		if(new_passport){
			passports.push_back(current);
			current.clear();
		}

		stringstream keypairstr(line);
		while(getline(keypairstr, key, ':') >> val){
			trim(key); // boost
			trim(val);
			current[key] = val;
		}

	}
	passports.push_back(current);

	for( passport_t passport : passports){
		if(validate_passport_pt1(passport)) valid_count_1++;
		if(validate_passport_pt2(passport)) valid_count_2++;
	}

	cout << "Part 1 valid passports: " << valid_count_1 << endl;
	cout << "Part 2 valid passports: " << valid_count_2 << endl;
	return 0;
}
