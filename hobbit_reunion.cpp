#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "hobbit_reunion.h"
using namespace std;

// Destination is always Bilbo's home at node 'C'
#define SHIRE 'C'
#define TARGET_NAME "Bilbo"
#define MAX_ROW 26
#define MAX_COL 26

// Global variables
bool adjacencyMat[MAX_ROW][MAX_COL];
int distanceMat[MAX_ROW][MAX_COL];
int traveltimeMat[MAX_ROW][MAX_COL];
int magicalcoinsMat[MAX_ROW][MAX_COL];
int trollsMat[MAX_ROW][MAX_COL];

////////////////////////////////////////////////////////////////////////////////

void readHomes(string filename, vector<dwarf_struct> &dwarves) {
  ifstream infile(filename);
  if (!infile.is_open()) return;
  string name;
  vector<string> namesvec;
  while (infile >> name) {
    char city;
    if (name == string(TARGET_NAME)) {
      infile >> city;
      continue;
    }
    infile >> city;
    name += city;
    namesvec.push_back(name);
  }
  // Sort alphabetically
  sort(namesvec.begin(), namesvec.end());
  dwarf_struct dwarf;
  for (string &s : namesvec) {
    dwarf.name = s.substr(0, s.length()-1);
    dwarf.location = s.substr(s.length()-1, 1).c_str()[0];
    // cout << dwarf.name << " " << dwarf.location << endl;
  }
}

void readMap(string filename, vector<edge_struct> &edges) {
  ifstream infile(filename);
  if (!infile.is_open()) return;
  char token;
  edge_struct es;
  while (infile >> token) {
    es.location1 = token;
    infile >> token;
    es.location2 = token;
    int numtoken;
    infile >> numtoken;
    es.distance = numtoken;
    infile >> numtoken;
    es.traveltime = numtoken;
    infile >> numtoken;
    es.magicalcoins = numtoken;
    infile >> numtoken;
    es.trolls = numtoken;
    // cout << es.location1 << es.location2 << es.distance << es.traveltime << es.magicalcoins << es.trolls << endl;
    edges.push_back(es);
  }
}

// Reads data into global adjacency matrix data structures
void createAdjacencyMatrix(vector<edge_struct> &edges) {
  for (edge_struct &e : edges) {
    int node1 = e.location1 - 'A';
    int node2 = e.location2 - 'A';

    adjacencyMat[node1][node2] = true;
    distanceMat[node1][node2] = e.distance;
    traveltimeMat[node1][node2] = e.traveltime;
    magicalcoinsMat[node1][node2] = e.magicalcoins;
    trollsMat[node1][node2] = e.trolls;

    adjacencyMat[node2][node1] = true;
    distanceMat[node2][node1] = e.distance;
    traveltimeMat[node2][node1] = e.traveltime;
    magicalcoinsMat[node2][node1] = e.magicalcoins;
    trollsMat[node2][node1] = e.trolls;
  }
}

void printAdjacencyMatrix() {
  cout << "  ";
  for (int f = 0; f < MAX_ROW; f++)
    cout << (char) (f + 'A') << " ";
  cout << endl;

  for (int r = 0; r < MAX_ROW; r++) {
    cout << (char) (r + 'A') << " ";
    for (int c = 0; c < MAX_COL; c++) {
      string s = (adjacencyMat[r][c]) ? "1 " : "0 ";
      cout << s;
    }
    cout << endl;
  }
}

void shortestHopPath() {
  // Placeholder
}

void shortestDistancePath() {
  // Placeholder
}

void shortestTimePath() {
  // Placeholder
}

void fewestTrollsPath() {
  // Placeholder
}

////////////////////////////////////////////////////////////////////////////////

int main() {
  // Initialize data structures
  vector<dwarf_struct> dwarves;
  vector<edge_struct> edges;

  // Read input files
  readHomes("canadahomes.txt", dwarves);
  readMap("canadamap.txt", edges);

  // Initialize data structures
  createAdjacencyMatrix(edges);
  // printAdjacencyMatrix();

  // Apply routing algorithms
  shortestHopPath();
  // shortestDistancePath();
  // shortestTimePath();
  // fewestTrollsPath();
}
