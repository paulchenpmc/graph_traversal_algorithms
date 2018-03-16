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
#define TARGET_LOCATION 'C'  // Shire
#define TARGET_NAME "Bilbo"
#define MAX_ROW 26
#define MAX_COL 26

// Global variables
vector<dwarf_struct> dwarvesvec;
vector<edge_struct> edgesvec;
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
    dwarvesvec.push_back(dwarf);
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
    edgesvec.push_back(es);
  }
  infile.close();
}

// Reads data into global adjacency matrix data structures
void createAdjacencyMatrix() {
  for (edge_struct &e : edgesvec) {
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
   int min = INT_MAX;
   int min_index;
   for (int v = 0; v < MAX_ROW; v++)
     if (sptSet[v] == false && distances[v] <= min)
         min = distances[v], min_index = v;
   return min_index;
}

void shortestHopPath() {
  // Find min dist of all nodes from source
  int src = TARGET_LOCATION - 'A';
  int distances[MAX_ROW];
  bool sptSet[MAX_ROW];
  string pathArray[MAX_ROW];

  for (int i = 0; i < MAX_ROW; i++) {
    distances[i] = INT_MAX, sptSet[i] = false;
  }
  distances[src] = 0;
  for (int c=0; c < MAX_ROW-1; c++) {
    int u = minDist(distances, sptSet);
    sptSet[u] = true;
    for (int v = 0; v < MAX_ROW; v++) {
      if (!sptSet[v] && adjacencyMat[u][v] && distances[u] != INT_MAX
        && distances[u]+adjacencyMat[u][v] < distances[v]) {
        distances[v] = distances[u] + adjacencyMat[u][v];
        pathArray[v] += string(1, (char)(u+'A'));
        pathArray[v] += pathArray[u];
      }
    }
  }
  // printSolution(distances, MAX_ROW);
  for (int i = 0; i < MAX_ROW; i++) {
    if (pathArray[i] == string("")) continue;
    cout << "Origin: " << (char)(i+'A') << " path: " << pathArray[i] << endl;
  }
}






// void SHP() {
//   for (dwarf_struct dwarf : dwarvesvec) {
//     int start = dwarf.location - 'A';
//     int currentnode = start;
//     cout << "start node: " << (char)(start+'A') << endl;
//     bool visited[MAX_ROW];
//     for (int adjacent = 0; adjacent < MAX_ROW; adjacent++) {
//       if (!adjacencyMat[currentnode][adjacent]) continue;
//       cout << (char)(adjacent+'A') << " ";
//
//     }
//     break;
//   }
// }

void shortestDistancePath() {

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
