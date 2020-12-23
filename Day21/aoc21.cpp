#include <set>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <map>

using std::endl;
using std::cout;

std::regex word_re("([a-z]+)");
std::regex allergens_re("\\(contains (.*)\\)");
std::regex ingredients_re("^(.*)\\(");
std::smatch match;

typedef std::set<std::string> strset_t;
typedef std::map<std::string, strset_t> ingrMap_t;

ingrMap_t ingrMap;
std::sregex_iterator end_it;
std::vector<std::string> all_ingreds;

int main()
{
  std::ifstream infile("input.txt");
  std::string line;

  // Read input
  while(getline(infile, line)){

    // Match ingredient list
    strset_t ingreds;

    // Grab the ingredients string
    if(std::regex_search(line, match, ingredients_re)){
      cout << match[1] << endl;
      std::string match_str = match[1].str();

      // Add each ingredient to the set ingreds
      std::sregex_iterator iter(match_str.begin(),
				match_str.end(),
				word_re);
      while(iter != end_it){
	ingreds.insert((*iter)[0]);
	all_ingreds.push_back((*iter)[0]);
	++iter;
      }
      cout << ingreds.size() << endl;
    }

    // Match allergen list
    if(std::regex_search(line, match, allergens_re)){
      cout << match[1] << endl;
      std::string match_str = match[1].str();

      // Consider each allergen in turn
      std::sregex_iterator iter(match_str.begin(),
				match_str.end(),
				word_re);
      while(iter != end_it){
	std::string allergen = (*iter)[0];
	if(ingrMap.find(allergen) == ingrMap.end()){// new entry
	  ingrMap.insert({allergen, ingreds});

	}else{// set intersection
	  strset_t prev_ingreds = ingrMap[allergen];
	  strset_t intersect;
	  std::set_intersection(prev_ingreds.begin(),
				prev_ingreds.end(),
				ingreds.begin(),
				ingreds.end(),
				std::inserter(intersect,
					      intersect.begin()));
	  ingrMap[allergen] = intersect;
	}
	++iter;
      }
      // cout << ingreds.size() << endl;


    }

  }

  std::vector<std::string> unallergens = all_ingreds;

  cout << "-------------" << endl;
  // Write out the possibilities for each allergen
  // and remove them from unallergens
  for(auto it : ingrMap){
    cout << it.first << endl;
    for (auto it2 : it.second){
      cout << it2 << " " << endl;
      unallergens.erase(std::remove(unallergens.begin(),
				    unallergens.end(), it2),
			unallergens.end());
    }
    cout << endl;
  }

  // Part 1 - identify ingredients in no list and count
  // their occurrences.
  cout << "Part 1 ans: " << unallergens.size() << endl;

  while(true){
    bool hitOne = false;
    for(auto it : ingrMap){
      auto allergen = it.first;
      auto possIngr = it.second;

      if(possIngr.size() > 1) continue; // Seeking a single answer
      std::string ingr = *possIngr.begin();
      for(auto it2 = ingrMap.begin(); it2 != ingrMap.end(); it2++){
	if(it == *it2) continue; // Don't remove from self
	auto possSet = &it2->second;
	if(possSet->find(ingr)!=possSet->end()){
	  hitOne = true;
	  possSet->erase(ingr);
	}

      }
    }
    if(!hitOne) break;
  }

  std::stringstream pt2ans;
  for (auto it : ingrMap){
    cout << it.first << endl;
    pt2ans << *it.second.begin();
    pt2ans << ",";
  }
  cout << "Pt2 ans follows:" << endl;
  cout << pt2ans.str() << endl;
  return 0;
}
