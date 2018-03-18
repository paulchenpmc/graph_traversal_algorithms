#include <iostream>
#include <string>
#include <fstream> // for file IO
#include <algorithm> // for sort algorithm
#include <vector>
#include "hobbit_reunion.h"
using namespace std;

// Destination is always Bilbo's home at node 'C'
#define TARGET_LOCATION 'C'  // Shire
#define TARGET_NAME "Bilbo"
#define MAX_ROW 26
#define MAX_COL 26
#define INFINITY 1000000000

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
      string s = (traveltimeMat[r][c]) ? "1 " : "0 ";
      cout << s;
    }
    cout << endl;
  }
}

int printRoutingInfo(const char * algorithm, int hops[],
  int dist[], int time[], int gold[], int trolls[], string pathArray[]){
  cout << "\n\n" << algorithm << " Algorithm" << endl;
  cout << "Dwarf\tHome\tHops\tDist\tTime\tGold\tTrolls\tPath" << endl;
  cout << "----------------------------------------------------------------" << endl;
  for (dwarf_struct d : dwarvesvec) {
    int index = d.location - 'A';
    cout << d.name << "\t";
    cout << d.location << "\t";
    cout << hops[index] << "\t";
    cout << dist[index] << "\t";
    cout << time[index] << "\t";
    cout << gold[index] << "\t";
    cout << trolls[index] << "\t";
    cout << d.location << pathArray[index] << endl;
  }
  cout << "----------------------------------------------------------------" << endl;
}

int minDist(int distances[], bool sptSet[]){
   int min = INFINITY;
   int min_index;
   for (int v = 0; v < MAX_ROW; v++)
     if (sptSet[v] == false && distances[v] <= min)
         min = distances[v], min_index = v;
   return min_index;
}

void shortestHopPath() {
  int src = TARGET_LOCATION - 'A';
  bool sptSet[MAX_ROW];
  string pathArray[MAX_ROW];
  int hopsArray[MAX_ROW] = {0};
  int distArray[MAX_ROW] = {0};
  int timeArray[MAX_ROW] = {0};
  int goldArray[MAX_ROW] = {0};
  int trollsArray[MAX_ROW] = {0};
  int predArray[MAX_ROW] = {TARGET_LOCATION - 'A'};

  for (int i = 0; i < MAX_ROW; i++) {
    hopsArray[i] = INFINITY;
    sptSet[i] = false;
  }
  hopsArray[src] = 0;
  for (int c=0; c < MAX_ROW-1; c++) {
    int u = minDist(hopsArray, sptSet);
    sptSet[u] = true;
    for (int v = 0; v < MAX_ROW; v++) {
      if (!sptSet[v] && adjacencyMat[u][v] && (hopsArray[u] != INFINITY)
        && (hopsArray[u]+adjacencyMat[u][v] < hopsArray[v])) {
        hopsArray[v] = hopsArray[u] + adjacencyMat[u][v];
        distArray[v] = distArray[u] + distanceMat[u][v];
        timeArray[v] = timeArray[u] + traveltimeMat[u][v];
        goldArray[v] = goldArray[u] + magicalcoinsMat[u][v];
        trollsArray[v] = trollsArray[u] + trollsMat[u][v];
        predArray[v] = u;
      }
    }
  }
  // Calculate routing paths
  for (dwarf_struct d : dwarvesvec) {
    int index = d.location - 'A';
    int j = index;
    do {
      j = predArray[j];
      pathArray[index] += string(1, (char)(j+'A'));
    } while (j + 'A' != TARGET_LOCATION);
  }
  printRoutingInfo("Shortest Hop Path", hopsArray, distArray, timeArray, goldArray, trollsArray, pathArray);
}

void shortestDistancePath() {
  int src = TARGET_LOCATION - 'A';
  bool sptSet[MAX_ROW];
  string pathArray[MAX_ROW];
  int hopsArray[MAX_ROW] = {0};
  int distArray[MAX_ROW] = {0};
  int timeArray[MAX_ROW] = {0};
  int goldArray[MAX_ROW] = {0};
  int trollsArray[MAX_ROW] = {0};
  int predArray[MAX_ROW] = {TARGET_LOCATION - 'A'};

  for (int i = 0; i < MAX_ROW; i++) {
    distArray[i] = INFINITY;
    sptSet[i] = false;
  }
  distArray[src] = 0;
  for (int c=0; c < MAX_ROW-1; c++) {
    int u = minDist(distArray, sptSet);
    sptSet[u] = true;
    for (int v = 0; v < MAX_ROW; v++) {
      if (!sptSet[v] && distanceMat[u][v] && (distArray[u] != INFINITY)
        && (distArray[u]+distanceMat[u][v] < distArray[v])) {
        hopsArray[v] = hopsArray[u] + adjacencyMat[u][v];
        distArray[v] = distArray[u] + distanceMat[u][v];
        timeArray[v] = timeArray[u] + traveltimeMat[u][v];
        goldArray[v] = goldArray[u] + magicalcoinsMat[u][v];
        trollsArray[v] = trollsArray[u] + trollsMat[u][v];
        predArray[v] = u;
      }
    }
  }
  // Calculate routing paths
  for (dwarf_struct d : dwarvesvec) {
    int index = d.location - 'A';
    int j = index;
    do {
      j = predArray[j];
      pathArray[index] += string(1, (char)(j+'A'));
    } while (j + 'A' != TARGET_LOCATION);
  }
  printRoutingInfo("Shortest Distance Path", hopsArray, distArray, timeArray, goldArray, trollsArray, pathArray);
}

void shortestTimePath() {
  int src = TARGET_LOCATION - 'A';
  bool sptSet[MAX_ROW];
  string pathArray[MAX_ROW];
  int hopsArray[MAX_ROW] = {0};
  int distArray[MAX_ROW] = {0};
  int timeArray[MAX_ROW] = {0};
  int goldArray[MAX_ROW] = {0};
  int trollsArray[MAX_ROW] = {0};
  int predArray[MAX_ROW] = {TARGET_LOCATION - 'A'};

  for (int i = 0; i < MAX_ROW; i++) {
    timeArray[i] = INFINITY;
    sptSet[i] = false;
  }
  timeArray[src] = 0;
  for (int c=0; c < MAX_ROW-1; c++) {
    int u = minDist(timeArray, sptSet);
    sptSet[u] = true;
    for (int v = 0; v < MAX_ROW; v++) {
      if (!sptSet[v] && traveltimeMat[u][v] && (timeArray[u] != INFINITY)
        && (timeArray[u]+traveltimeMat[u][v] < timeArray[v])) {
        hopsArray[v] = hopsArray[u] + adjacencyMat[u][v];
        distArray[v] = distArray[u] + distanceMat[u][v];
        timeArray[v] = timeArray[u] + traveltimeMat[u][v];
        goldArray[v] = goldArray[u] + magicalcoinsMat[u][v];
        trollsArray[v] = trollsArray[u] + trollsMat[u][v];
        predArray[v] = u;
      }
    }
  }
  // Calculate routing paths
  for (dwarf_struct d : dwarvesvec) {
    int index = d.location - 'A';
    int j = index;
    do {
      j = predArray[j];
      pathArray[index] += string(1, (char)(j+'A'));
    } while (j + 'A' != TARGET_LOCATION);
  }
  printRoutingInfo("Shortest Time Path", hopsArray, distArray, timeArray, goldArray, trollsArray, pathArray);
}

void fewestTrollsPath() {
  int src = TARGET_LOCATION - 'A';
  bool sptSet[MAX_ROW];
  string pathArray[MAX_ROW];
  int hopsArray[MAX_ROW] = {0};
  int distArray[MAX_ROW] = {0};
  int timeArray[MAX_ROW] = {0};
  int goldArray[MAX_ROW] = {0};
  int trollsArray[MAX_ROW] = {0};
  int predArray[MAX_ROW] = {TARGET_LOCATION - 'A'};

  for (int i = 0; i < MAX_ROW; i++) {
    trollsArray[i] = INFINITY;
    sptSet[i] = false;
  }
  trollsArray[src] = 0;
  for (int c=0; c < MAX_ROW-1; c++) {
    int u = minDist(trollsArray, sptSet);
    sptSet[u] = true;
    for (int v = 0; v < MAX_ROW; v++) {
      if (!sptSet[v] && trollsMat[u][v] && (trollsArray[u] != INFINITY)
        && (trollsArray[u]+trollsMat[u][v] < trollsArray[v])) {
        hopsArray[v] = hopsArray[u] + adjacencyMat[u][v];
        distArray[v] = distArray[u] + distanceMat[u][v];
        timeArray[v] = timeArray[u] + traveltimeMat[u][v];
        goldArray[v] = goldArray[u] + magicalcoinsMat[u][v];
        trollsArray[v] = trollsArray[u] + trollsMat[u][v];
        predArray[v] = u;
      }
    }
  }
  // Calculate routing paths
  for (dwarf_struct d : dwarvesvec) {
    int index = d.location - 'A';
    int j = index;
    do {
      j = predArray[j];
      pathArray[index] += string(1, (char)(j+'A'));
    } while (j + 'A' != TARGET_LOCATION);
  }
  printRoutingInfo("Fewest Trolls Path", hopsArray, distArray, timeArray, goldArray, trollsArray, pathArray);
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

  shortestDistancePath();
  cout << "Successfully ran SDP algorithm" << endl;

  shortestTimePath();
  cout << "Successfully ran STP algorithm" << endl;

  fewestTrollsPath();
  cout << "Successfully ran FTP algorithm" << endl;

  cout << "Ran all algorithms, exiting..." << endl;
}
