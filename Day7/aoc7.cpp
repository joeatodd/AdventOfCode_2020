#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <set>
#include <queue>

using namespace std;

// Two regex for containing bag & contained bag
regex bigbag_regex("^(.*) bags contain");
regex smallbag_regex("([0-9]+) ([a-z ]+) bag");
smatch match;
bool verbose = false;

// Multimap for representing bag graph
typedef multimap<string, pair<string, int>> BagContainer;

int main()
{
	// ifstream infile("eg2.txt");
	ifstream infile("input.txt");
	string line, remainder, bigBag, smallBag;
	BagContainer bagContains, bagContained;
	int smallBagCount;

	while(getline(infile, line)){
		if(regex_search(line, match, bigbag_regex)){
			bigBag = match[1];
			if(verbose) cout << " Big bag  " << bigBag << endl;
		}else{
			throw runtime_error("No match for big bag");
		}

		// Iterate through the line, identifying contained bags
		remainder = line;
		while (regex_search(remainder, match, smallbag_regex)){
			smallBagCount = stoi(match[1]);
			smallBag = match[2];

			// Store graph of contained & contains (suspect will need for Pt2)
			bagContains.insert({bigBag, {smallBag, smallBagCount}});
			bagContained.insert({smallBag, {bigBag, smallBagCount}});

			if(verbose) cout << "Small bag: " << smallBag << " count : " 
					 << smallBagCount << endl;
			remainder = match.suffix(); // The rest of the line
		}

	}

	string myBag = "shiny gold";
	set<string> bagCandidates = {myBag};
	int setSize = 1, oldSetSize = 1;
	// Generate a set of bags (bagCandidates) which can contain (directly or indirectly)
	// my shiny gold bag. It's pretty inefficient because I recheck *every* member of
	// the set each time round.
	do{
		oldSetSize = setSize;
		for (auto itr = bagContained.begin(); itr != bagContained.end(); itr++){
			if(bagCandidates.find(itr -> first) != bagCandidates.end()){

				if(verbose) cout << itr->first << " " <<
						    itr->second.first << endl;
				bagCandidates.insert(itr->second.first);

			}
		}
		setSize = bagCandidates.size();

		if(verbose) { // print info
			cout << "Bag set: ";
			for(auto it = bagCandidates.begin(); it != bagCandidates.end(); it++){
				cout <<  "|" << *it << "|  ";
			}
			cout << endl;
		}
	}while(setSize > oldSetSize); // Halt when no more bags added


	cout << "Part 1 answer: " << setSize - 1 << endl;


	// Now need a priority queue to work down from 'shiny gold' to count total bags
	typedef pair<string, int> bagEntry;
	queue<bagEntry> bagQ;
	bagEntry currEntry;
	int bagCount = 0, currMult;
	string currBag;
	typedef BagContainer::iterator bagMapIterator;
	pair<bagMapIterator, bagMapIterator> currBagRange;

	bagQ.push(make_pair(myBag, 1));

	while(!bagQ.empty()){
		// For each bag inside current bag, add count to bagCount, and add to queue
		currEntry = bagQ.front();
		currBag = currEntry.first;
		currMult = currEntry.second;
		bagQ.pop();

		// Get all entries from the bagContains multimap
		currBagRange = bagContains.equal_range(currBag);
		for(bagMapIterator it = currBagRange.first; it != currBagRange.second; it++){
			smallBag = it -> second.first;
			smallBagCount = it -> second.second;
			cout << currBag << " contains " << smallBagCount << " " << smallBag << endl;

			cout << "Multi: " << smallBagCount * currMult << endl;
			bagCount += smallBagCount * currMult;
			bagQ.push(make_pair(it -> second.first, smallBagCount * currMult));
		}
		
	}

	cout << "Part 2 answer: " << bagCount << " bags in my shiny gold bag!" << endl;
	return 0;
}
