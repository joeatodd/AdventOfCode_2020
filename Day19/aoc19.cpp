#include <regex>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <list>
#include <queue>
#include <set>

using std::cout;
using std::endl;

std::regex base_rule_re("([0-9]+): \"([a-z])\"");
std::regex simple_rule_re("([0-9]+):((?: [0-9]+)+)$");
std::regex or_rule_re("([0-9]+):(( [0-9]+)+) \\|((?: [0-9]+)+)$");
std::smatch match, *match_p;

bool verbose = false;

typedef std::set<std::string> stringset_t;

/* Rule object which represents a node in the tree, and is
linked to 1 or 2 sub-rules. Matching either of these sub-rules
implies matching this rule.

In the case of the base rules, these match only "a" and "b" and
represent the base of the tree.

Rule 0 is the top of the tree.
*/
class Rule
{
public:

  Rule(int id_in){
    id=id_in;
    if(id==0) root = true;
  };

  void parseRules(const std::string instr,
		  std::map<int, std::shared_ptr<Rule>> &ruleMap);
  void printRules(const int idx);
  std::pair<stringset_t, bool> checkMsg(std::string &msg);

  int id;
  bool base = false; // 'a' or 'b'
  bool root = false; // rule 0

  int nOptions = 0;
  std::vector<std::vector<std::shared_ptr<Rule>>> options;
  std::string ruleStr1, ruleStr2, baseStr;
};

// Convert string like '1 2 31' to a vector of pointers to rules.
void Rule::parseRules(const std::string instr,
		      std::map<int,std::shared_ptr<Rule>> &ruleMap){
      int val;
      std::vector<std::shared_ptr<Rule>> rule_seq;
      std::stringstream ss(instr);
      bool verbose = false;
      while(ss >> val){
      	rule_seq.push_back((ruleMap[val]));
      }

      // Add a sequence of pointers to sub-rules for this rule
      options.push_back(rule_seq);
}

// Print out this rule's subrules
void Rule::printRules(const int idx){
  cout << "Rule " << id << " child " << idx << " ";
  for(auto it : options.at(idx)){
    cout << it->id << " ";
  }
  cout << endl;
}

/* Recursively check msg against the rules.

   Returns a set of strings which are the possible remainders,
   and boolean success (whether it matches the rule)
*/
std::pair<stringset_t, bool> Rule::checkMsg(std::string &msg){
  stringset_t result;

  // Trying to match rule with no string (fail)
  if(msg.empty()){return {result, false};}

  if(verbose)cout << "Rule " << id << " checking: " << msg << endl;

  // Check 'a' or 'b', return snipped string
  if(base){
    if(msg.substr(0,1) == baseStr){
      if(verbose)cout << "base match" << endl;
      result.insert(msg.substr(1));
      return {result, true};
    }else{
      // Or return empty stringset & fail
      // (or should we return the whole string?)
      if(verbose) cout << "no base match" << endl;
      return {result, false};
    }
  }

  // Call sub-rules recursively

  bool matched, some_success = false;
  // For each set of rules (1 or 2)

  for (auto rule_vec : options){
    stringset_t remainder, next_ops, this_ops;
    bool success;


    this_ops = {msg}; // First feed in just the msg argument

    // For each rule in this rule set
    for (auto rule : rule_vec){
      matched = false;
      next_ops.clear();

      // If only candidate is empty string, failed
      if(this_ops.size() == 1 &&
      	 (this_ops.find("") != this_ops.end())){
      	break;
      }

      /* Each rule in the sequence can return several options
	 for remaining snipped message. So we need to check
	 each of these in turn and construct a set 'next_ops'.
	 Then 'next_ops' -> 'this_ops' is the set of possible
	 strings to check against the next rule.
      */
      for(auto msg_op : this_ops){

	std::tie(remainder, success) = rule->checkMsg(msg_op);

	if(success){
	  next_ops.insert(remainder.begin(), remainder.end());
	  matched = true;
	}
      }

      if(!matched) break;
      this_ops = next_ops;
    }

    // If we didn't match all of the sub-rules of this
    // option, skip and move onto next option
    if(!matched){
      continue;
    }else{
      // If we did match this rule, copy 'this_ops'
      // whch is the possible remainders from matching
      // this set of rules onto 'result'.
      some_success = true;
      result.insert(this_ops.begin(), this_ops.end());
    }

    if(verbose) {
      for (auto it : this_ops){
	cout << "Rule " << id << " candidate: " << it << endl;
      }
    }

  }

  bool success;
  if(id==0){
    success = result.find("") != result.end();
  }else{
    success = some_success;
  }

  return {result, success};
}

void print_message(std::string header, std::vector<int> inlist){
  cout << header;
  for(auto i:inlist) cout << i;
  cout << endl;
}

int main()
{

  std::ifstream infile("input.txt");
  std::string line;
  std::map<int, std::shared_ptr<Rule>> ruleMap;

  // Parse rules from file
  while(getline(infile, line)){

    bool verbose = false;

    // 'a' or 'b' -> base rules
    if(std::regex_search(line, match, base_rule_re)){

      if(verbose) cout << "Rule " << match[1] << " matches char "
		       << match[2] << endl;

      // basemap[match[2]] = stoi(match[1]);
      int id = stoi(match[1]);

      auto newRule = std::make_shared<Rule>(Rule(id));
      newRule->base = true;
      newRule->baseStr = match[2];
      ruleMap.insert({id, newRule});
    }

    // Simple sequence
    else if(std::regex_search(line, match, simple_rule_re)){

      if(verbose){
	cout << "Match size: " << match.size() << endl;
	cout << "Rule " << match[1] << " matches seq " <<
	  match[2] << endl;
      }
      int id = stoi(match[1]);
      auto newRule = std::make_shared<Rule>(Rule(id));
      newRule->ruleStr1 = match.str(2);
      newRule->nOptions = 1;
      ruleMap.insert({id, newRule});

    }

    // 'or' sequence
    else if(std::regex_search(line, match, or_rule_re)){

      if(verbose){
	cout << "Match size: " << match.size() << endl;
	cout << "Rule " << match[1] << " matches seq " <<
	  match[2] << " or seq " << match[4] << endl;
      }

      int id = stoi(match[1]);

      auto newRule = std::make_shared<Rule>(Rule(id));

      newRule->nOptions = 2;
      newRule->ruleStr1 = match.str(2);
      newRule->ruleStr2 = match.str(4);
      ruleMap.insert({id, newRule});

    }else if(line.empty()){
      cout << "Done with rules" << endl;
      break;
    }
  } // End of rule matching

  cout << "Finished construction of members..." << endl;

  /* Resolve rule strings into vectors of pointers
     to other rules (tree structure) */
  for(auto it : ruleMap){
    auto currRule = it.second;
    currRule->parseRules(currRule->ruleStr1, ruleMap);
    if(currRule->nOptions == 2)
      currRule->parseRules(currRule->ruleStr2, ruleMap);
  }

  auto rule0 = ruleMap.at(0);
  auto rule42 = ruleMap.at(42);
  auto rule31 = ruleMap.at(31);

  int valid_count1 = 0;
  int valid_count2 = 0;
  while(getline(infile, line)){

    cout << endl << line << endl;

    // Part 1 - does rule 0 match?
    {
      stringset_t result;
      bool success;
      std::tie(result, success) = rule0->checkMsg(line);
      if(success) valid_count1++;
    }

    bool do_part2 = true;
    if(do_part2){
      // Part 2 - does recursive rule 0 match?
      // Actually need N * 42 followed by M * 31
      // Where N > M >= 1

      // Strip off one 42 at least
      typedef std::map<std::string, int> mapstr_t;
      mapstr_t vec42;

      stringset_t result;
      bool success;

      std::tie(result, success) = rule42->checkMsg(line);
      result.erase(line);
      if(!success) continue;

      for (auto it : result){
	vec42.insert({it, 0});
      }
      if(vec42.size() == 0) continue;

      bool proceed = true;
      while(proceed){
	// cout << "proceeding " << endl;
	mapstr_t temp = vec42;
	for (auto it : temp){

	  std::string checky = it.first;
	  stringset_t result;
	  bool success;
	  std::tie(result, success) = rule42->checkMsg(checky);
	  result.erase(it.first); // sign of failure

	  if(!success) continue;
	  for (auto it2 : result){
	    vec42[it2] =  it.second+1;
	  }

	}
	if(temp == vec42) proceed = false;
      }

      // Delete the ones which have max level 0, because
      // they only matched once (no good), or those which
      // are blank
      for (auto it = vec42.begin(), last = vec42.end(); it != last;)
	{
	  if(it->second == 0 || it->first == ""){
	    it = vec42.erase(it);
	  }else{
	    ++it;
	  }
	}

      // Now we want to find 31 at least once, and no more
      // times than we found 42, minus 1
      mapstr_t vec31 = vec42;
      proceed = true;
      int counter = 0;

      while(proceed){
	// cout << "proceeding " << endl;

	mapstr_t temp = vec31;
	if(counter == 0){
	  counter++;
	  vec31.clear();
	}
	for (auto it : temp){

	  std::string checky = it.first;
	  stringset_t result;
	  bool success;
	  std::tie(result, success) = rule31->checkMsg(checky);
	  result.erase(checky); // sign of failure
	  if(!success) continue;

	  // cout << "31 seeking in : " << checky << endl;

	  for (auto it2 : result){
	    // only insert if doesn't exist at higher level
	    vec31.insert({it2, it.second-1});
	    // cout << it2 << endl;
	  }

	}
	if(temp == vec31 && counter > 0) proceed = false;
      }

      // Seek in vec31 for int > 1

      for (auto it : vec31){
	if(it.second >= 0 && it.first == "") {valid_count2++; break;}
      }
    } // do part 2
  }

  cout << "Part 1 answer: " << valid_count1 << endl;
  cout << "Part 2 answer: " << valid_count2 << endl; // 462 too high
  return 0;
}
