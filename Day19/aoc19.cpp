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
  stringset_t checkMsg(std::string &msg);

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

// Recursively check msg against the rules.
stringset_t Rule::checkMsg(std::string &msg){
  stringset_t result;

  if(verbose)cout << "Rule " << id << " checking: " << msg << endl;

  // Check 'a' or 'b', return snipped string
  if(base){
    if(msg.substr(0,1) == baseStr){
      if(verbose)cout << "base match" << endl;
      result.insert(msg.substr(1));
      return result;
    }else{
      if(verbose) cout << "no base match" << endl;
      return result;
    }
  }

  // Call sub-rules recursively

  // For each set of rules (1 or 2)
  for (auto rule_vec : options){
    stringset_t remainder, next_ops, this_ops;

    this_ops = {msg}; // First feed in just the msg argument

    // For each rule in this rule set
    for (auto rule : rule_vec){

      /* Each rule in the sequence can return several options
	 for remaining snipped message. So we need to check
	 each of these in turn and construct a set 'next_ops'.
	 Then 'next_ops' -> 'this_ops' is the set of possible
	 strings to check against the next rule.
      */
      for(auto msg_op : this_ops){
	remainder = rule->checkMsg(msg_op);
	next_ops.insert(remainder.begin(), remainder.end());
      }
      this_ops = next_ops;
    }

    if(verbose) {
      for (auto it : this_ops){
	cout << "Rule " << id << " candidate: " << it << endl;
      }
    }

    result.insert(this_ops.begin(), this_ops.end());
  }

  return result;
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

  int valid_count = 0;
  while(getline(infile, line)){

    auto result = rule0->checkMsg(line);
    if(result.find("") != result.end()){
      valid_count++;
    }
    cout << line << " set size: " << result.size() << endl;
    // for (auto it : result){
    //   cout << it << endl;
    // }
  }

  cout << "Part 1 answer: " << valid_count << endl;
  return 0;
}
