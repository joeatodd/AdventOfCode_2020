#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

typedef vector<vector<char>> seats_t;

/* Rules for Pt 1:
   - Unoccupied seats with no occupied neighbours become occupied
   - Occupied seats with 4 or more occupied neighbours become empty
*/
char get_seat_next(seats_t &seats, int x, int y){

	int nx = seats.at(0).size();
	int ny = seats.size();
	int tx, ty, neigh_occ;

	neigh_occ = 0;
	
	// If it's floor space, we're done
	bool is_seat = seats[y][x] != '.';
	if(!is_seat) return '.';

	// Check if empty or occupied
	bool is_empty = seats[y][x] == 'L';

	// Inspect all neighbours
	for(int dy : {-1, 0, 1}){
		for(int dx : {-1, 0, 1}){
			if(dx == 0 && dy == 0) continue; // My seat

			ty = y + dy;
			tx = x + dx;
			if(ty < 0 || tx < 0 || ty >= ny || tx >= nx) continue;

			if(seats[ty][tx] == '#') neigh_occ ++;

		}
	}

	// Return new seat state based on rules
	if(is_empty){
		if(neigh_occ == 0){
			return '#';
		}else{
			return 'L';
		}
	}else{
		if(neigh_occ >= 4){
			return 'L';
		}else{
			return '#';
		}
	}
}

/* Rules for Pt 2:
   - Unoccupied seats with no occupied neighbours become occupied
   - Occupied seats with 5 or more occupied neighbours become empty
   BUT!
   - We consider the nearest visible seat in each direction (i.e.
     look across empty floorspace.
*/
char get_seat_next_pt2(seats_t &seats, int x, int y){

	int nx = seats.at(0).size();
	int ny = seats.size();
	int tx, ty, neigh_occ;

	neigh_occ = 0;
	
	// If it's floor space, we're done
	bool is_seat = seats[y][x] != '.';
	if(!is_seat) return '.';

	// Check if empty or occupied
	bool is_empty = seats[y][x] == 'L';

	for(int dy : {-1, 0, 1}){
		for(int dx : {-1, 0, 1}){
			if(dx == 0 && dy == 0) continue; // My seat

			ty = y;
			tx = x;

			// Keep looking outward until we reach a seat
			while(true){
				ty += dy;
				tx += dx;
				if(ty < 0 || tx < 0 || ty >= ny || tx >= nx) break;

				if(seats[ty][tx] == '#'){
					neigh_occ ++;
					break;
				}else if(seats[ty][tx] == 'L'){
					break;
				}
			}
		}
	}

	// Return new seat state
	if(is_empty){
		if(neigh_occ == 0){
			return '#';
		}else{
			return 'L';
		}
	}else{
		if(neigh_occ >= 5){
			return 'L';
		}else{
			return '#';
		}
	}


}


// Debugging output - show the current seating plan
void print_seats(seats_t seat_arr){
	for( auto row : seat_arr ){
		for( auto s : row ){
			cout << s;
		}
		cout << endl;
	}

	cout << endl << endl;
}

bool unchanged(seats_t seats, seats_t seats_n){
	return seats == seats_n;
}

// Return numbers of occupied seats
int count_occ(seats_t seats){
	int count = 0;
	for (auto row : seats){
		for ( auto seat : row ){
			if(seat == '#') count ++;
		}
	}

	return count;
}

int main()
{
	std::ifstream infile("input.txt");
	std::string line;
	seats_t seat_arr, seat_arr_n, seat_arr_init;
	vector<char> seat_row;
	bool verbose = false;

	// Read input
	while(getline(infile, line)){
		seat_row.clear();
		for(auto l : line){
			seat_row.push_back(l);
		}
		seat_arr_init.push_back(seat_row);
	}

	int nx = seat_arr_init.at(0).size();
	int ny = seat_arr_init.size();
	
	seat_arr = seat_arr_init;
	seat_arr_n = seat_arr;

	// Iterate seat evolution until equilibrium
	while(true){
		#pragma omp parallel for collapse(2)
		for(int i = 0; i < ny; i++){
			for(int j = 0; j < nx; j++){
				seat_arr_n.at(i).at(j) = get_seat_next(seat_arr, j, i);
			}
		}

		if(verbose) print_seats(seat_arr_n);
		if(unchanged(seat_arr, seat_arr_n)) break;
		seat_arr = seat_arr_n;
	}

	cout << "Part 1 answer: " << count_occ(seat_arr) << endl;

	seat_arr = seat_arr_init;
	seat_arr_n = seat_arr_init;

	// Iterate to equilibrium
	while(true){
		#pragma omp parallel for collapse(2)
		for(int i = 0; i < ny; i++){
			for(int j = 0; j < nx; j++){
				seat_arr_n.at(i).at(j) = get_seat_next_pt2(seat_arr, j, i);
			}
		}

		if(verbose) print_seats(seat_arr_n);
		if(unchanged(seat_arr, seat_arr_n)) break;
		seat_arr = seat_arr_n;
	}

	cout << "Part 2 answer: " << count_occ(seat_arr) << endl;
	return 0;
}
