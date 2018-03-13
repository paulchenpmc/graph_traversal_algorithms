#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits.h>
#include "hobbit_reunion.h"
using namespace std;

// Destination is always Bilbo's home at node 'C'
#define SHIRE 'C'
#define TARGET_NAME "Bilbo"
#define MAX_ROW 26
#define MAX_COL 26

// Global variables
vector<dwarf_struct> dwarves;
vector<edge_struct> edges;
int adjacencyMat[MAX_ROW][MAX_COL];
int distanceMat[MAX_ROW][MAX_COL];
int traveltimeMat[MAX_ROW][MAX_COL];
int magicalcoinsMat[MAX_ROW][MAX_COL];
int trollsMat[MAX_ROW][MAX_COL];

////////////////////////////////////////////////////////////////////////////////

void readHomes(string filename) {
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
    dwarves.push_back(dwarf);
  }
  infile.close();
}

void readMap(string filename) {
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
  infile.close();
}

// Reads data into global adjacency matrix data structures
void createAdjacencyMatrix() {
  for (edge_struct &e : edges) {
    int node1 = e.location1 - 'A';
    int node2 = e.location2 - 'A';

    adjacencyMat[node1][node2] = 1;
    distanceMat[node1][node2] = e.distance;
    traveltimeMat[node1][node2] = e.traveltime;
    magicalcoinsMat[node1][node2] = e.magicalcoins;
    trollsMat[node1][node2] = e.trolls;

    adjacencyMat[node2][node1] = 1;
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

int printSolution(int distances[], int n){
   printf("Vertex   Distance from Source\n");
   for (int i = 0; i < MAX_ROW; i++) {
     if (distances[i] == INT_MAX) continue;
      printf("%c \t\t %d\n", i+'A', distances[i]);
    }
}

int minDist(int distances[], bool sptSet[]){
   int min = INT_MAX, min_index;
   for (int v = 0; v < MAX_ROW; v++)
     if (sptSet[v] == false && distances[v] <= min)
         min = distances[v], min_index = v;
   return min_index;
}

void shortestHopPath() {
  int src = SHIRE - 'A';
  int distances[MAX_ROW];     // The output array.  distances[i] will hold the shortest distance from src to i
  bool sptSet[MAX_ROW]; // sptSet[i] will true if vertex i is included in shortest path tree or shortest distance from src to i is finalized
  // Initialize all distances as INFINITE and stpSet[] as false
  for (int i = 0; i < MAX_ROW; i++)
    distances[i] = INT_MAX, sptSet[i] = false;

  // Distance of source vertex from itself is always 0
  distances[src] = 0;

  // Find shortest path for all vertices
  for (int count = 0; count < MAX_ROW-1; count++)
  {
   // Pick the minimum distance vertex from the set of vertices not yet processed. u is always equal to src in first iteration.
   int u = minDist(distances, sptSet);

   // Mark the picked vertex as processed
   sptSet[u] = true;

   // Update dist value of the adjacent vertices of the picked vertex.
   for (int v = 0; v < MAX_ROW; v++)

     // Update distances[v] only if is not in sptSet, there is an edge from
     // u to v, and total weight of path from src to  v through u is
     // smaller than current value of distances[v]
     if (!sptSet[v] && adjacencyMat[u][v] && distances[u] != INT_MAX && distances[u]+adjacencyMat[u][v] < distances[v])
        distances[v] = distances[u] + adjacencyMat[u][v];
     }

     // print the constructed distance array
     printSolution(distances, MAX_ROW);
}

// Delete and recopy from SHP later
void shortestDistancePath() {
  int src = SHIRE - 'A';
  int distances[MAX_ROW];     // The output array.  distances[i] will hold the shortest distance from src to i
  bool sptSet[MAX_ROW]; // sptSet[i] will true if vertex i is included in shortest path tree or shortest distance from src to i is finalized
  // Initialize all distances as INFINITE and stpSet[] as false
  for (int i = 0; i < MAX_ROW; i++)
    distances[i] = INT_MAX, sptSet[i] = false;

  // Distance of source vertex from itself is always 0
  distances[src] = 0;

  // Find shortest path for all vertices
  for (int count = 0; count < MAX_ROW-1; count++)
  {
   // Pick the minimum distance vertex from the set of vertices not yet processed. u is always equal to src in first iteration.
   int u = minDist(distances, sptSet);

   // Mark the picked vertex as processed
   sptSet[u] = true;

   // Update dist value of the adjacent vertices of the picked vertex.
   for (int v = 0; v < MAX_ROW; v++)

     // Update distances[v] only if is not in sptSet, there is an edge from
     // u to v, and total weight of path from src to  v through u is
     // smaller than current value of distances[v]
     if (!sptSet[v] && distanceMat[u][v] && distances[u] != INT_MAX && distances[u]+distanceMat[u][v] < distances[v])
        distances[v] = distances[u] + distanceMat[u][v];
     }

     // print the constructed distance array
     printSolution(distances, MAX_ROW);
}








void shortestTimePath() {
  // Placeholder
}

void fewestTrollsPath() {
  // Placeholder
}

////////////////////////////////////////////////////////////////////////////////

int main() {
  cout << "Starting program..." << endl;
  
  // Read input files
  readHomes("canadahomes.txt");
  readMap("canadamap.txt");
  cout << "Successfully read input files" << endl;

  // Initialize data structures
  createAdjacencyMatrix();
  // printAdjacencyMatrix();
  cout << "Successfully created adjacency matrix" << endl;

  // Apply routing algorithms
  shortestHopPath();
  cout << "Successfully ran SHP algorithm" << endl;

  // shortestDistancePath();
  // cout << "Successfully ran SDP algorithm" << endl;

  // shortestTimePath();
  // cout << "Successfully ran STP algorithm" << endl;

  // fewestTrollsPath();
  // cout << "Successfully ran FTP algorithm" << endl;
}
