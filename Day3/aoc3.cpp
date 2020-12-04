#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;
int main()
{
	fstream infile("input.txt");
	int i, j, nrows, ncols;
	string line;
	bool verbose = true;
	map<char, int> charmap = {{'#',1},{'.',0}};
	vector<pair<int, int>> directions;

	// count lines & length of line
	nrows = count(istreambuf_iterator<char>(infile),
	      istreambuf_iterator<char>(),
	      ('\n'));
	infile.clear();
	infile.seekg(0); // back to start of file
	getline(infile, line, '\n');
	ncols = line.size();
	infile.clear();
	infile.seekg(0); // back to start of file

	if(verbose) cout << "ncols " << ncols << " nrows " << nrows;

	// Allocate arrays & fill from file
	int** treemap = new int*[nrows];
	for(i=0; i<nrows; i++){
		treemap[i] = new int[ncols];
		getline(infile, line);
		for(j=0; j<ncols; j++){
			treemap[i][j] = charmap[line[j]];
		}
	}

	// Start at top left corner and slide downhill!
	int x=0, y=0, hitcount=0;
	for(i=0; i<nrows; i++){
		if(treemap[x][y] == 1) hitcount++;
		x+=1;
		y+=3;
		y %= ncols;
		if(verbose) cout << "X: " << x << " Y: " << y << endl;
	}

	cout << "Part 1, number of trees hit: " << hitcount << endl;

	// Create vector of directions to try
	directions.push_back(make_pair(1,1));
	directions.push_back(make_pair(3,1));
	directions.push_back(make_pair(5,1));
	directions.push_back(make_pair(7,1));
	directions.push_back(make_pair(1,2));

	long long int pt2_ans = 1;
	for(pair<int,int> dir : directions){
		int x=0, y=0, hitcount=0;
		cout << "Trying new dir: " << dir.first << "  " << dir.second << endl;
		
		// for(i=0; i<nrows; i++){
		do{
			if(treemap[x][y] == 1) hitcount++;
			x+=dir.second;
			y+=dir.first;
			y %= ncols;
			if(verbose) cout << "X: " << x << " Y: " << y << endl;
		}while(x < nrows);
		pt2_ans *= hitcount;
	}

	cout << "Part 2 ans: " << pt2_ans;
	return 0;
}
