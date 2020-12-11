#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

typedef vector<vector<char>> seats_t;

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

	for(int dy : {-1, 0, 1}){
		for(int dx : {-1, 0, 1}){
			if(dx == 0 && dy == 0) continue; // My seat

			ty = y + dy;
			tx = x + dx;
			if(ty < 0 || tx < 0 || ty >= ny || tx >= nx) continue;

			if(seats[ty][tx] == '#') neigh_occ ++;

		}
	}

	// cout << "X " << x << "Y " << y << "empty: " << is_empty << " occ: " << neigh_occ << endl;
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

	// cout << "X " << x << "Y " << y << "empty: " << is_empty << " occ: " << neigh_occ << endl;
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

	while(getline(infile, line)){
		seat_row.clear();
		for(auto l : line){
			seat_row.push_back(l);
		}
		seat_arr_init.push_back(seat_row);
		// cout << line << endl;
	}

	int nx = seat_arr_init.at(0).size();
	int ny = seat_arr_init.size();
	
	seat_arr = seat_arr_init;
	seat_arr_n = seat_arr;

	while(true){
		for(int i = 0; i < ny; i++){
			for(int j = 0; j < nx; j++){
				seat_arr_n.at(i).at(j) = get_seat_next(seat_arr, j, i);
				// cout << "Change: " << j << " " << i << " " << seat_arr.at(i).at(j) << seat_arr_n.at(i).at(j) << endl;
			}
		}

		// print_seats(seat_arr_n);
		if(unchanged(seat_arr, seat_arr_n)) break;
		seat_arr = seat_arr_n;

		// break;
	}

	cout << "Part 1 answer: " << count_occ(seat_arr) << endl;

	seat_arr = seat_arr_init;
	seat_arr_n = seat_arr_init;

	while(true){
		for(int i = 0; i < ny; i++){
			for(int j = 0; j < nx; j++){
				seat_arr_n.at(i).at(j) = get_seat_next_pt2(seat_arr, j, i);
				// cout << "Change: " << j << " " << i << " " << seat_arr.at(i).at(j) << seat_arr_n.at(i).at(j) << endl;
			}
		}

		// print_seats(seat_arr_n);
		if(unchanged(seat_arr, seat_arr_n)) break;
		seat_arr = seat_arr_n;

		// break;
	}

	cout << "Part 2 answer: " << count_occ(seat_arr) << endl;
	return 0;
}
