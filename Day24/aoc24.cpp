#include <map>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

typedef std::pair<int,int> tile_t;
typedef std::map<std::string, std::pair<int, int>> dirmap_t;
typedef std::map<std::pair<int,int>, bool> tilemap_t;
typedef vector<vector<bool>> tilegrid_t;

std::regex dir_re("([ns]*[ew])");
std::smatch match;

bool verbose = false;

void printGrid(tilegrid_t grid){

  int yRange = grid.size();
  int xRange = grid[0].size();

  for (int y = 0; y < yRange; y++) {
    for (int x = 0; x < xRange; x++) {
      cout << ((grid[y][x]) ? "x" : "-");
      if(y==12 && x==11){cout << "*";}else{cout << " ";}
    }
    cout << endl;
  }
}

// Given euclidian coordinates (& origin), return tile grid pos
tile_t coord_to_grid(tile_t coord, tile_t origin){

  int tile_y = coord.second + origin.second;
  int tile_x = ((coord.first - coord.second)/2) + origin.first;

  return {tile_x, tile_y};
}

// Produce next grid according to rules
tilegrid_t next_flip(tilegrid_t &grid){
  tilegrid_t grid_n = grid; // to copy dims...

  int yRange = grid.size();
  int xRange = grid[0].size();


  for (int y = 0; y < yRange; y++) {
    for (int x = 0; x < xRange; x++) {

      bool amBlack = grid[y][x];
      int nBlack = 0;

      // Neighbour check w/ bounds check
      if(x > 0 && grid[y][x-1]) {nBlack++;} // west
      if((x < (xRange-1)) && grid[y][x+1]){ nBlack++;} // east

      if((y > 0) && grid[y-1][x]){nBlack++;} // south-west
      if((y < (yRange-1)) && grid[y+1][x]) {nBlack++;} // north-east

      if((y < (yRange-1)) && (x > 0) && grid[y+1][x-1]) {nBlack++;} // north-west
      if((y > 0) && (x < (xRange-1)) && grid[y-1][x+1]) {nBlack++;} // south-east

      if(amBlack){
	if(nBlack == 0 || nBlack > 2) grid_n[y][x] = false;
      }else{
	if(nBlack == 2) grid_n[y][x] = true;
      }
    }
  }

  return grid_n;
}

int countBlack(tilegrid_t grid){
  int result = 0;
  for(auto it : grid){
    result += std::count(it.begin(), it.end(), true);
  }
  return result;
}

int main()
{

  dirmap_t dirmap = {{"sw",{-1,-1}},
		     {"se",{1,-1}},
		     {"e",{2,0}},
		     {"w",{-2,0}},
		     {"ne",{1,1}},
		     {"nw",{-1,1}}
  };

  tile_t ref_tile = {0,0};
  tilemap_t tilemap;

  std::string line;
  std::ifstream infile("input.txt");
  if(!infile) return 2;

  int minX = 1e6, minY = 1e6, maxX = -1e6, maxY = -1e6;
  // Parse directions & flip tiles in map
  while(getline(infile, line)){
    tile_t curr_tile = ref_tile;
    while(std::regex_search(line, match, dir_re)){

      tile_t new_dir = dirmap[match[1]];

      curr_tile.first += new_dir.first;
      curr_tile.second += new_dir.second;

      line = match.suffix();
    }

    if(verbose) cout << "Final tile loc " << curr_tile.first << "  " << curr_tile.second << endl;

    // Store bounds
    if(curr_tile.first < minX) minX = curr_tile.first;
    if(curr_tile.first > maxX) maxX = curr_tile.first;
    if(curr_tile.second < minY) minY = curr_tile.second;
    if(curr_tile.second > maxY) maxY = curr_tile.second;

    auto it = tilemap.find(curr_tile);
    if(it == tilemap.end()){ // Unfound, flip to black
      tilemap[curr_tile] = true;
      if(verbose) cout << "First flip " << curr_tile.first << " " << curr_tile.second << endl;
    }else{ // Found, flip
      tilemap[curr_tile] = !tilemap[curr_tile];
      if(verbose) cout << "Subs flip " << curr_tile.first << " " << curr_tile.second << endl;
    }
  }

  int blackcount = 0;
  for(auto it : tilemap){
    if(it.second == true) blackcount++;
  }
  cout << "Part 1 ans: " << blackcount << endl;


  // Part 2 - game of life w/ hex grid

  /* Determine max size of grid based on initial coord range & nSteps

     Hex tiles are arranged so that...
   */
  int nSteps = 100;
  int buff = 4; // Fudge factor...

  int xRange = 2*nSteps + (maxX - minX) + buff;
  int yRange = 2*nSteps + (maxY - minY) + buff;

  tile_t origin = {xRange/2, yRange/2};  // origin in grid coords, not tile coords

  cout << "xRange: " << xRange << " yRange: " << yRange << endl;
  cout << "Origin: " << origin.first << " " << origin.second << endl;

  // Define grid w/ non-orthogonal axes [rowNum][colNum]
  tilegrid_t tilegrid;
  tilegrid.resize(yRange);
  for (int i = 0; i < yRange; i++) {
    tilegrid[i].resize(xRange);
    std::fill(tilegrid[i].begin(), tilegrid[i].end(), false);
  };

  // From euclidian coords to tile
  for(auto it : tilemap){
    tile_t coord = coord_to_grid(it.first, origin);
    tilegrid[coord.second][coord.first] = it.second;

    cout << "Coordinate " << it.first.first << " "
	 << it.first.second << " grid: " << coord.first << " " << coord.second << endl;
  }

  cout << "Init black count: " << countBlack(tilegrid) << endl;

  for (int i = 0; i < nSteps; i++) {
    // printGrid(tilegrid);
    tilegrid_t tilegrid_n = next_flip(tilegrid);
    tilegrid = tilegrid_n;
    // cout << "Step " << i+1 << " black count: " << countBlack(tilegrid) << endl;
  }

  return 0;
}
