#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <tuple>
#include <algorithm>

using std::endl;
using std::cout;
using std::vector;

typedef vector<vector<vector<char>>> map_t;

void print_map(map_t map_in){
  int dim = map_in.size(); // assume cube

  for (int i = 0; i < dim; ++i) {
    for (int j=0; j < dim; ++j) {
      for (int k=0; k < dim; ++k) {
	cout << map_in[k][j][i];
      }
      cout << endl;
    }
    cout << endl << endl;
  }

}

void blank_map(map_t &map_in){
  int dim = map_in.size(); // assume cube

  for (int i = 0; i < dim; ++i) {
    for (int j=0; j < dim; ++j) {
      std::fill(map_in[i][j].begin(),map_in[i][j].end(), '.');
    }
  }
}

int count_fullmap(map_t &map_in){
  int dim = map_in.size();
  int counter = 0;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      counter += std::count(map_in[i][j].begin(),
			    map_in[i][j].end(),
			    '#');
    }
  }
  return counter;
}

int count_neighbours(map_t &map_in, int x, int y, int z){
  int dim = map_in.size();
  int cx, cy, cz, counter=0;
  bool verbose = false;
  if(verbose) cout << "For loc: " << x << " "
		   << y << " " << z << " " << endl;

  for(int dx = -1; dx <= 1; dx++){
    cx = x + dx;
    if(cx < 0 || cx >= dim) continue;
    for(int dy = -1; dy <= 1; dy++){
      cy = y + dy;
      if(cy < 0 || cy >= dim) continue;
      for(int dz = -1; dz <= 1; dz++){
	cz = z + dz;
	if(cz < 0 || cz >= dim) continue;
	if(dx == 0 && dy == 0 && dz == 0) continue;
	if(map_in[cx][cy][cz] == '#') counter++;
      }
    }
  }
  if(verbose) cout << "Count: " << counter << endl;
  return counter;
}

int main()
{
  //Map order x, y, z
  map_t map_init, map, map_n;
  std::ifstream infile("input.txt");
  bool verbose = true;

  int nIter = 6;

  // Read first line to get problem size
  std::string line;
  getline(infile, line);
  int initDim = line.size();
  int dim = initDim + (2*nIter) + 2; // Buffer of 2 for first iter

  // Initialize the vector of chars empty:
  map_init.resize(dim);
  for(int i=0; i<dim; i++){
    map_init[i].resize(dim);
    for(int j=0; j<dim; j++){
      map_init[i][j].resize(dim);
    }
  }

  blank_map(map_init);

  // Put puzzle input in from this loc:
  std::tuple<int, int, int> curr_loc, ll_corner;
  ll_corner = {nIter+1, nIter+1, nIter+1};
  curr_loc = ll_corner;

  // Rewind file & read input
  infile.clear();
  infile.seekg(0);
  int *x,*y,*z;
  char val;

  x = &std::get<0>(curr_loc);
  y = &std::get<1>(curr_loc);
  z = &std::get<2>(curr_loc);

  while(getline(infile, line)){
      std::stringstream linestream(line);

      while(linestream.get(val)){

	if(verbose) cout << val;
	// std::get<0>(curr_loc) ++;
	map_init[*x][*y][*z] = val;
	(*x)++;
      }
      *x = std::get<0>(ll_corner);
      if(verbose) cout << endl;
      std::get<1>(curr_loc) ++;
  }

  if(verbose){
    cout << "Time 0 :" << endl;
    cout << "---------------" << endl;
    print_map(map_init);
  }
  map = map_init;
  map_n = map_init; // just for size
  for (int t = 0; t < nIter; t++) {

    blank_map(map_n);
    for (int i = 0; i < dim; i++) {
      for (int j = 0; j < dim; j++) {
	for (int k = 0; k < dim; k++) {

	  int n = count_neighbours(map, i, j, k);
	  if(map[i][j][k] == '#'){ //active
	    map_n[i][j][k] = (n==2 || n==3) ? '#' : '.';
	    // if(n==2 || n==3) cout << "Keeping " << endl;
	  }else{                   // inactive
	    map_n[i][j][k] = (n==3) ? '#' : '.';
	    // if(n==3) cout << "Setting " << endl;
	  }

	}
      }
    }

    map = map_n;
    cout << "Time " << t << " :" << endl;
    cout << "---------------" << endl;
    print_map(map);
  }

  cout << "Part 1 ans " << count_fullmap(map) << endl;


  return 0;
}
