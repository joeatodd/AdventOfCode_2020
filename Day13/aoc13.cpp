#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>

int main()
{
	std::ifstream infile("input.txt");
	std::string line;
	int t_time;
	std::vector<int> bus_times, bus_offsets, bus_remainders;
	bool verbose = false;

        // First line: the time I get to the bus station
	getline(infile, line);
	t_time = stoi(line);

	// Next line is comma separated bus times
	// Need to keep track of their position in the list for Pt2
	int counter = -1, bus_time, remainder;
	while(getline(infile, line, ',')){
		counter++;
		if(line.at(0) == 'x') continue;
		bus_time = stoi(line);
		bus_times.push_back(bus_time);
		bus_offsets.push_back(counter);

                // Convoluted imitation of python's mod operator
		remainder = (((bus_time - counter) % bus_time) + bus_time) % bus_time;
		bus_remainders.push_back(remainder);

		if(verbose) std::cout << " Bus: " << bus_time << " " << counter << " " << remainder << std::endl;
	}

	int prev_bus, next_bus, min_wait = 1000, best_bus;

	// Part 1 - next bus after t_time
	for(auto it = bus_times.begin(); it != bus_times.end(); it++){
		prev_bus = t_time % *it;
		next_bus = -prev_bus+*it;
		if(next_bus < min_wait){
			min_wait = next_bus;
			best_bus = *it;
		}
	}

	std::cout << "Part 1 " << best_bus * min_wait << std::endl << std::endl;

	// Part 2 - Chinese remainder problem
	// Each bus comes every n minutes (bus_times)
	// Each bus has a target remainder (bus_remainder)

	// First sort the two arrays (speeds sieving up dramatically!)
	std::vector<std::size_t> index(bus_times.size());
	std::iota(index.begin(), index.end(), 0);
	std::sort(index.begin(), index.end(),
		  [&](size_t a, size_t b){
			  return bus_times[a] > bus_times[b];
		  });

	// Check buses sorted descending
	if(verbose){
		for(auto it = index.begin(); it != index.end(); it++){
			std::cout << bus_times[*it] << " ";
		}
		std::cout << std::endl;
	}

	// Sieving algorithm https://www.wikiwand.com/en/Chinese_remainder_theorem#Search_by_sieving
	long long int curr_mult = 1, cum_time, candidate;
	cum_time = bus_remainders.at(index[0]);
	for(size_t i = 0; i < index.size()-1; i++){

		curr_mult *= bus_times.at(index[i]);
		candidate = cum_time;
		while(true){
			candidate += curr_mult;
			if(candidate % bus_times.at(index[i+1]) == 
			   bus_remainders.at(index[i+1])){
				if(verbose) std::cout << candidate << std::endl;
				break;
			}
		}
		cum_time = candidate;
		if(verbose) std::cout << "Cum time: " << cum_time << std::endl;
	}

	std::cout << "Part 2 ans: " << cum_time << std::endl;
	return 0;
}
