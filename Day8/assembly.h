#include <string>
#include <vector>

typedef std::pair<std::string, int> opcode_t;

class AssemblyMachine
{
public:
	AssemblyMachine(std::string filename);

	int run(bool verbose = false);
	int get_acc();

	void set_code(int idx, std::string code);
	void set_val(int idx, int val);

	std::string get_code(int idx);
	int get_val(int idx);

	std::vector<opcode_t> get_codes(bool initial = false);

	void reset();

private:
	int acc = 0, curr_idx = 0;
	std::vector<int> visit_count;
	std::vector<opcode_t> opcodes, opcodes_init;

};
