#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <math.h>
#include <assert.h>
#include <queue>

using std::cout;
using std::endl;
using std::vector;

typedef vector<std::pair<int,int>> seaMonster_t;
typedef vector<vector<char>> bigImage_t;

// Print out the full image
void printBigGrid(bigImage_t &image){
  for (auto it: image){
    for (auto it2: it){
      cout << it2;
    }
    cout << endl;
  }
}

// Rotate the full image clockwise 90
void rotateBigGrid(bigImage_t &image){
  bigImage_t temp_image = image;
  int dim = image.size();

  for (int i = 0; i < dim; i++) {
    vector<char> row = temp_image[i];
    for (int j = 0; j < dim; j++) {
      image[j][dim-1-i] = row[j];
    }
  }
}

// Flip the full image l-r
void flipBigGrid(bigImage_t &image){
  for (auto it = image.begin(); it != image.end(); it++ ){
    std::reverse(it->begin(), it->end());
  }
}

// Count the '#' in full image
int countChoppy(bigImage_t &image){
  int result = 0;
  for (auto it = image.begin(); it != image.end(); it++ ){
    int count_row = std::count(it->begin(), it->end(), '#');
    result += count_row;
  }
  return result;
}

// Parse the sea_monster into a vector of pair<x,y>
seaMonster_t seaMonsterLoc(){
  seaMonster_t result;
  std::ifstream infile("sea_monster.txt");
  std::string line;
  int row_counter = 0;

  while(getline(infile, line)){
    std::stringstream ss(line);
    char val;
    int col_counter = 0;
    while(ss.get(val)){
      if(val == '#') result.push_back({row_counter, col_counter});
      col_counter++;
    }
    row_counter++;
  }
  return result;
}


// The sub-image class
class Grid
{
public:
  int id;

  vector<char> data; // All data
  vector<vector<char>> edges;  // top, right, bottom, left
  vector<int> edge_chsum; // count # on each edge - quick compare
  vector<std::set<int>> edge_match_ids; // Which grids match which edge?
  std::set<Grid*> neighbours;
  int big_x=-1, big_y=-1;
  int max_x, max_y;
  int nx=10, ny=10;
  bool located = false, initialized = false;

  void setupEdges();
  void flip_ud();
  void flip_lr();
  void rot_90();
  void printGrid();
  void printEdge(int id);
  void chopEdges();
  void emplace(vector<vector<char>> &image);
};

// Store the four edges of the 10x10 sub-grid
void Grid::setupEdges(){

  // Resize the edge_match_id vectors

  if(!initialized){
    edge_match_ids.resize(4);
    initialized = true;
  }else{
    edge_chsum.clear();
    edges.clear();
  }

  // Push corners:
  // Top, Right, Bottom, Left
  // l-r, b-t
  vector<char> edge;

  // Top edge l-r
  for(int i=0;i<10;i++){
    edge.push_back(data[i]);
  }
  edges.push_back(edge);

  int count = std::count(edge.begin(), edge.end(), '#');
  edge_chsum.push_back(count);

  // Right edge b-t
  edge.clear();
  for(int i=10;i>0;i--){
    edge.push_back(data[(i*10) - 1]);
  }
  edges.push_back(edge);

  count = std::count(edge.begin(), edge.end(), '#');
  edge_chsum.push_back(count);

  // Bottom edge l-r
  edge.clear();
  for(int i=90;i<100;i++){
    edge.push_back(data[i]);
  }
  edges.push_back(edge);

  count = std::count(edge.begin(), edge.end(), '#');
  edge_chsum.push_back(count);

  // Left edge b-t
  edge.clear();
  for(int i=9;i>=0;i--){
    edge.push_back(data[(i*10)]);
  }
  edges.push_back(edge);

  count = std::count(edge.begin(), edge.end(), '#');
  edge_chsum.push_back(count);
}

// Flip the sub-grid upside down
void Grid::flip_ud(){

  vector<char> temp = data;

  for (int i = 0; i < 10; i++) {
    std::copy(temp.begin()+(i*10),
	      temp.begin()+((i+1) * 10),
	      data.begin()+(9-i)*10);
  }
  this->setupEdges();

  // Flip edge_match_ids too
  std::set<int> temp_ids;
  temp_ids = edge_match_ids[0];
  edge_match_ids.at(0) = edge_match_ids[2];
  edge_match_ids.at(2) = temp_ids;

}

// Flip the sub-grid left-right
void Grid::flip_lr(){

  vector<char> row, temp = data;

  for (int i = 0; i < 10; i++) {
    vector<char> row(temp.begin()+(i*10),
		     temp.begin()+((i+1) * 10));
    std::reverse(row.begin(), row.end());

    std::copy(row.begin(),
	      row.end(),
	      data.begin()+(i*10));
  }
  this->setupEdges();

  // Flip edge_match_ids too
  std::set<int> temp_ids;
  temp_ids = edge_match_ids[1];
  edge_match_ids[1] = edge_match_ids[3];
  edge_match_ids[3] = temp_ids;
}

// Rotate the sub-grid 90 degrees clockwise (swap rows/cols)
void Grid::rot_90(){
  vector<char> temp = data;

  for (int i = 0; i < 10; i++) {
    vector<char> row(temp.begin()+(i*10),
		     temp.begin()+((i+1) * 10));

    for (int j = 0; j < 10; j++) {
      data[(j*10)+(9-i)] = row[j];
    }

  }
  this->setupEdges();

  // Rotate edge_match_ids too
  std::set<int> temp_ids = edge_match_ids[0];
  edge_match_ids.at(0) = edge_match_ids[3];
  edge_match_ids.at(3) = edge_match_ids[2];
  edge_match_ids.at(2) = edge_match_ids[1];
  edge_match_ids.at(1) = temp_ids;

}

void Grid::printGrid(){
  cout << endl;
  int counter = 0;
  for (auto it : data){
    cout << it;
    if(counter % 10 == 9) cout << endl;
    counter++;
  }
  cout << endl;
}

void Grid::printEdge(int id){
  cout << endl;
  for (int i = 0; i < 10; i++) {
    cout << edges[id][i];
  }
  cout << endl;
}

// Take off the edges (10x10 -> 8x8)
void Grid::chopEdges(){
  assert(big_x != -1);
  assert(big_y != -1);

  // Chop left side
  vector<char> temp;
  for(int i=0; i<data.size();i++){
    if(i % nx != 0) temp.push_back(data[i]);
  }
  data = temp;
  nx--;

  // Chop right side
  temp.clear();
  for(int i=0; i<data.size();i++){
    if(i % nx != nx-1) temp.push_back(data[i]);
  }
  data = temp;
  nx--;

  // Chop top
  data.erase(data.begin(), data.begin()+nx);
  ny--;

  // Chop bottom
  data.erase(data.begin()+(ny-1)*nx, data.begin()+(ny*nx));
  ny--;

}

// Place the data from this sub-image in the wider frame
void Grid::emplace(vector<vector<char>> &image){
  int tr_x = big_x * 8;
  int tr_y = big_y * 8;

  //lazy counting
  int counter = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      image[tr_y+i][tr_x+j] = data[counter];
      counter++;
    }
  }


}

int main()
{
  std::ifstream infile("input.txt");
  std::string line;
  vector<Grid> grids;

  // Read the input
  Grid *newGrid;
  while(getline(infile, line)){
    char val;

    if(line.substr(0,4) == "Tile"){
      newGrid = new Grid;
      newGrid->id = stoi(line.substr(5, 11));
    }else if(line == ""){
      grids.push_back(*newGrid);
      delete(newGrid);

    }else{
      std::stringstream ss(line);
      while(ss.get(val)){
	newGrid->data.push_back(val);
      }
    }

  }
  int big_dim = (int) std::pow(grids.size(), 0.5);

  cout << "All grids loaded " << endl;

  // Set up auxilliary vars per grid
  for(Grid& grid : grids){
    cout << grid.id << endl;
    grid.setupEdges();
    grid.max_x = big_dim-1;
    grid.max_y = big_dim-1;
  } // Edge construction

  // Sweep all combos for edge match
  for (Grid& it : grids){
    for (Grid& it2 : grids){
      if(it.id==it2.id) continue; // don't match self

      for(int i=0; i<4; i++){

	// Store reverse it edge for comparison
	vector<char> edge_r = it.edges[i];
	std::reverse(edge_r.begin(), edge_r.end());

	for(int j=0; j<4; j++){
	  if(it.edge_chsum[i] != it2.edge_chsum[j]) continue;
	  if(it.edges[i] == it2.edges[j] ||
	     edge_r == it2.edges[j]){
	    it.edge_match_ids[i].insert(it2.id);
	    it2.edge_match_ids[j].insert(it.id);
	    it.neighbours.insert(&it2);
	    it2.neighbours.insert(&it);
	  }
	}
      }

    }
  }

  // Identify corners and compute part 1 answer
  long long int corner_prod = 1;
  Grid *corner;

  for (Grid& grid : grids){
    int this_match = 0;
    for(int i = 0; i < 4; i++){
      if(grid.edge_match_ids[i].size() > 0) this_match++;

      // Candidates would make this harder!
      assert(grid.edge_match_ids[i].size() <= 1);

    }
    if(this_match == 2) {
      corner_prod *= grid.id;
      corner = &grid;
    }
  }
  cout << "Part 1 answer: " << corner_prod << endl;

  // Make im_slots to hold IDs of images
  vector<vector<int>> im_slots;
  im_slots.resize(big_dim);
  for (int i = 0; i < big_dim; i++){
    im_slots[i].resize(big_dim);
    std::fill(im_slots[i].begin(), im_slots[i].end(), 0);
  }

  // 'Corner' is an arbitrary corner and we consider this
  // to be 'correctly' orientated, so find it's position:
  if(corner->edge_match_ids[0].size() > 0){ // bottom row
    assert(corner->edge_match_ids[2].size() == 0);
    corner->big_y = big_dim-1;
  }else{
    assert(corner->edge_match_ids[2].size() > 0);
    corner->big_y = 0;
  }

  if(corner->edge_match_ids[1].size() > 0){ // left corner
    assert(corner->edge_match_ids[3].size() == 0);
    corner->big_x = 0;
  }else{
    assert(corner->edge_match_ids[3].size() > 0);
    corner->big_x = big_dim-1;
  }

  cout << "Corner: " << corner->big_x << " "
       << corner->big_y << endl;
  im_slots[corner->big_y][corner->big_x] = corner->id;
  corner->located = true;

  std::queue<Grid*> grid_q;
  Grid *curr_grid, *locator;
  for(auto it : corner->neighbours){
    if(!it->located) grid_q.push(it);
  }

  while(!grid_q.empty()){
    curr_grid = grid_q.front();
    grid_q.pop();

    // Skip if found
    if(curr_grid->located) continue;

    // Push unfound neighbours onto queue
    for(auto it : curr_grid->neighbours){
      if(!it->located){
	grid_q.push(it);
      }else{
	locator = it;
      }
    }

    // Locate this grid using 'locator' neighbour
    int curr_id = curr_grid->id;
    int loc_id = locator->id;


    // Find which pair of edges (i,j) match
    int i;
    for (i = 0; i < 4; ++i) {
      if(locator->edge_match_ids[i].count(curr_id)) break;
    }
    assert(i < 4);

    int j;
    for (j = 0; j < 4; ++j) {
      if(curr_grid->edge_match_ids[j].count(loc_id)) break;
    }
    assert(j < 4);

    /* locator is fixed in place, so it defines the target loc
       as the opposite side 0-2, 1-3...
       Start by rotating the new image until the edges correspond
    */
    int tgtLoc = (i+2) % 4;
    int nRot = (((tgtLoc - j) % 4) + 4) % 4;

    for (int k = 0; k < nRot; k++) {
      curr_grid->rot_90();
    }

    // Then if it's reversed, flip accordingly
    if(curr_grid->edges[tgtLoc] != locator->edges[i]){

      // Check it matches in reverse
      vector<char> edge_r = curr_grid->edges[tgtLoc];
      std::reverse(edge_r.begin(), edge_r.end());
      assert(edge_r == locator->edges[i]);

      // Flip it
      if(i == 0 || i == 2){
	curr_grid->flip_lr();
      }else{
	curr_grid->flip_ud();
      }

    }

    // Find its place in the full image (relative to neighbour)
    int loc_x = locator -> big_x;
    int loc_y = locator -> big_y;
    switch(i){
    case 0: // above
      loc_y --;
      break;
    case 1: // right
      loc_x ++;
      break;
    case 2: // below
      loc_y ++;
      break;
    case 3: // left
      loc_x --;
      break;
    }
    curr_grid->big_x = loc_x;
    curr_grid->big_y = loc_y;

    curr_grid->located = true;
  } // Queue empty, all grids placed

  cout << "Big dim " << big_dim << endl;
  int full_img_size = big_dim * 8;
  vector<vector<char>> full_image(full_img_size);

  for (auto it = full_image.begin(); it != full_image.end(); it++) {
    it->resize(full_img_size);
    std::fill(it->begin(), it->end(), '.');
    // cout << it.size() << endl;
  }


  for(auto grid : grids){
    grid.chopEdges();
    grid.emplace(full_image);
  }

  printBigGrid(full_image);

  seaMonster_t monster = seaMonsterLoc();

  // Cycle through image looking for monsters
  int mon_count = 0;
  int counter = 0;
  while(mon_count==0){

    mon_count = 0;
    for (int i = 0; i < full_img_size; i++) {
      for (int j = 0; j < full_img_size; j++) {
	bool found = true;

	for (int k = 0; k < 2; k++) { // 2 pass, seek then delete
	  for(auto mon : monster){
	    int check_x = i + mon.first;
	    int check_y = j + mon.second;
	    if(check_x >= full_img_size ||
	       check_y >= full_img_size){
	      found = false;
	      break;
	    }
	    if(full_image[check_x][check_y] != '#'){
	      found = false;
	      break;
	    }
	    if(k==1) full_image[check_x][check_y] = 'O';
	  }
	  if(!found) break;
	  if(found) mon_count++;
	}
      }
    }
    counter++;
    rotateBigGrid(full_image);
    if(counter % 4 == 0) flipBigGrid(full_image);
    cout << "Monster count: " << mon_count << endl;
  }
  printBigGrid(full_image);
  cout << "Part 2 ans: " << countChoppy(full_image) << endl;
  return 0;
}
