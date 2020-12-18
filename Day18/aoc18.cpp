#include <list>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <boost/range/irange.hpp>

using std::cout;
using std::endl;
typedef std::list<std::string> expr_t;

bool verbose = false;
bool part2 = false;

// Print the current mathematical list
void print_expr(std::string header, expr_t expr){
  cout << header;
      for (auto it = expr.begin(); it != expr.end(); it++) {
	cout << *it;
      }
      cout << endl;
}

// Recursive function to do the maths (recursing into brackets)
std::string resolve_expr(expr_t expr){

    // Iterate until we have only a single number left
    while(expr.size() > 1){

      assert(expr.size() >= 3); // Should never have 2 members

      if(verbose) print_expr("Expression: ", expr);

      // Scan for "(value)" & replace if found
      bool broke_brack = false;
      for (auto it = expr.begin(); std::distance(it, expr.end()) > 2; it++) {
	if(*it == "(" && *std::next(it,2) == ")"){
	  long long int val = stoll(*std::next(it,1));
	  int dist = std::distance(expr.begin(), it);

	  expr.erase(it, std::next(it,3));
	  expr.insert(std::next(expr.begin(),dist), std::to_string(val));
	  broke_brack = true;
	  break;
	}
      }
      // If we broke brackets this time, go back to start
      if(broke_brack) continue;

      // Now scan for bracketed expressions and call self recursively
      expr_t::iterator it1, it2;
      bool got_brack = false;
      int brac_depth = 0;
      for (auto it = expr.begin(); it != expr.end(); it++) {
	if(*it == "("){
	  if(brac_depth == 0) it1 = it; // Mark outermost ( bracket
	  brac_depth++;
	  got_brack = true;

	}else if(*it == ")"){
	  brac_depth--;
	  if(brac_depth == 0){ // Mark outermost ) bracket
	    it2 = it;
	    break;
	  }
	}
      }

      // Extract bracketed expression into subexpr
      if(got_brack){
	int insert_pos = std::distance(expr.begin(),
				       std::next(it1,1));
	expr_t subexpr;
	subexpr.splice(subexpr.begin(),
		       expr,
		       std::next(it1,1),
		       it2);

	if(verbose) print_expr("Subexpr: ", subexpr);

	expr.insert(std::next(expr.begin(),insert_pos),
		       resolve_expr(subexpr)); // <- recursion
	continue;
      }

      /* Now scan for + operations, then * operations
	 Only get to this stage with an expression which
	 has no brackets.
      */
      int nIter = (part2) ? 2 : 1;
      for (int i=0; i<nIter; i++){

	bool operated = false;

	for (auto it = expr.begin(); it != expr.end(); it++) {

	  if(std::distance(it, expr.end()) < 3) break;

	  if(verbose) cout << "Trying: " << *it <<
			" and " << *std::next(it,2) << endl;

	  // Read the two values & oper
	  long long int val1 = stoll(*it);
	  long long int val2 = stoll(*std::next(it,2));
	  std::string oper = *std::next(it,1);

	  // Do the maths (if we get this far)

	  // In part 2, we do + before *, so:
	  if(part2){
	    // Not doing this operation this time round
	    std::string tgt_oper = (i==0) ? "+" : "*";
	    if(oper != tgt_oper){
	      std::advance(it, 1); // If cast fails, move forward 2
	      if(it == expr.end()) break;
	      continue;
	    }
	  }

	  if(verbose) cout << "Made it to operation with " << *it
			   << " and " << *std::next(it,2) << endl;

	  long long int result;
	  if(oper == "*"){
	    result = val1 * val2;
	  }else if(oper == "+"){
	    result = val1 + val2;
	  }else{
	    throw std::invalid_argument("Bad oper ");
	  }

	  // Replace "val1 + val2" with "result"
	  int dist = std::distance(expr.begin(), it);
	  expr.erase(it, std::next(it,3));
	  expr.insert(std::next(expr.begin(), dist),
		      std::to_string(result));
	  operated = true;
	  break;

	}
	if(operated) break; // Only 1 operation per loop for safety
      }
    }
    if(verbose) print_expr("Oper result: ", expr);
    return *expr.begin();
}

// Loop over all lines & keep running total
int main()
{
  std::ifstream infile("input.txt");
  std::string line;

  long long int total = 0;

  while(getline(infile, line)){

    if(verbose) cout << "New line: " << line << endl;
    expr_t expr;
    char val;
    std::string s;
    std::stringstream sstr(line);

    // Read the expression into a list of chars '1','+','2',...
    while(sstr.get(val)){
      if(val == ' ') continue;
      s = val;
      expr.push_back(s);
    }

    if(verbose) print_expr("Initial: ",expr);

    // Do the maths
    auto result = resolve_expr(expr);

    total += std::stoll(result);
    if(verbose) cout << "Result for line: " << result
		     << " Running total: " << total << endl;
  }

  cout << "Part " << ((part2) ? 2 : 1) <<
    " answer: " << total << endl;

  return 0;
}
