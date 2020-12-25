#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

int main()
{
  std::string line;
  std::ifstream infile("input.txt");

  getline(infile, line);
  long int card_pubkey = stoi(line);
  getline(infile, line);
  long int door_pubkey = stoi(line);

  cout << "Keys: " << card_pubkey<< "  " << door_pubkey << endl;

  int modulo = 20201227;

  long int i = 1;
  int card_loop = 0;
  while (i != card_pubkey) {
    card_loop++;
    i = i * 7 % modulo;
  }

  cout << "Card loop size: " << card_loop << endl;

  i = 1;
  for (int j = 0; j < card_loop; j++) {
    i = i * door_pubkey % modulo;
  }

  cout << "Part 1: " << i << endl;

  return 0;
}
