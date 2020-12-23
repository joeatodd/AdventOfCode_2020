#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using std::cout;
using std::endl;

typedef std::vector<long int> cups_t;

bool verbose = false;

void printList(cups_t inL, std::string header = "List: ", int first=1){
  if(!verbose) return;
  cout << header;
  cout <<  " " << first;
  int next = inL.at(first-1);
  while(next != first){
    cout << " " << next;
    next = inL.at(next-1);
  }
  cout << endl;
}

// Perform the crab shuffle
int shuffleCups(cups_t &cups, int first){

  long long int vals[3];
  long long int maxVal = cups.size() + 1;

  if(verbose) cout << "First " << first << " next " << cups.at(first-1) << endl;
  // Grab the next 3 numbers after 'first'
  vals[0] = cups.at(first-1);
  vals[1] = cups.at(vals[0]-1);
  vals[2] = cups.at(vals[1]-1);

  long long int join = cups.at(vals[2]-1);

  if(verbose) cout << "Next 3: " << vals[0] << " "
       << vals[1] << " "
       << vals[2] << endl;

  // Find the target loc (1 less than first, unless it's in the set)
  long long int tgt = first-1;
  while(std::find(vals, vals+3, tgt) != vals+3 || tgt==0
        ){
    tgt = (((tgt - 1) % maxVal) + maxVal) % maxVal;
    if(tgt == 0 ) cout << "Hit zero now" << endl;
  }
  long long int next = cups.at(tgt-1);

  if(verbose) cout << "Target " << tgt << " next " << next << endl;

  // link the first of our 3 numbers as clockwise neighbour of tgt
  cups.at(tgt-1) = vals[0];

  // Link the number after tgt as clockwise neighbour of 3rd num
  cups.at(vals[2]-1) = next;

  // Join the first & fourth numbers together as neighbours
  if(verbose) cout << "Joining first: " << first << " to " << join << endl;
  cups.at(first-1) = join;

  return cups.at(first-1);
}

int main()
{
  std::ifstream infile("input.txt");
  std::string line;
  cups_t cups, cups_init;
  char valc;

  getline(infile, line);
  std::stringstream line_ss(line);

  // Read cup numbers into list "clockwise"
  long long int maxVal=0, val;
  while(line_ss.get(valc)){
    cout << valc << endl;
    val = valc - '0';
    maxVal = (val > maxVal) ? val : maxVal;
    cups_init.push_back(val);
  }


  // Part 1
  cups_t cups_v(9);
  long long int first_init = cups_init.at(0);

  // For each value, cups_v stores the clockwise neighbour
  for (int i = 0; i < 8; i++) {
    cups_v.at(cups_init.at(i)-1) = cups_init.at(i+1);
  }
  cups_v.at(cups_init.at(8)-1) = cups_init.at(0);


  long long int nShuff = 100;
  long long int first = first_init;
  printList(cups_v, "Init ", first);
  for (int i = 0; i < nShuff; i++) {
    first = shuffleCups(cups_v, first);
    printList(cups_v, "Next ", first);
  }


  // Part 2

  cups_t milCups(1e6);
  first = first_init;

  for (int i = 0; i < 8; i++) {
    milCups.at(cups_init.at(i)-1) = cups_init.at(i+1);
  }
  milCups.at(cups_init.at(8)-1) = 10;

  for (long unsigned int i = 10; i < 1e6; i++) {
    milCups.at(i-1) = i+1;
  }

  milCups.at((1e6)-1) = first;

  for (long long int i = 0; i < 1e7; i++) {
    first = shuffleCups(milCups, first);
    if(i % 10000 == 0) cout << "it: " << i << endl;
  }

  long long int next = milCups.at(1-1);
  long long int subs = milCups.at(next-1);

  cout << "Part 2 " << next << " " << subs << " product " << next * subs << endl;

  return 0;
}
