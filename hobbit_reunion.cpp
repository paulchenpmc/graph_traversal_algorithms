/*
  This program is a fun attempt to simulate network routing algorithms,
  specifically Shortest Hop Path, Shortest Distance Path,
  Shortest Time Path, and Fewest Trolls Path.
  It employs dijkstra's algorithm to find optimal routing.
  As seen in the beginning of JRR Tolkien's 'The Hobbit'the dwarves
  must all gather to meet at the request of Gandalf, who has a detailed
  map of the Middle-Earth universe and must route them to their meeting
  location at Bilbo's home in the Shire.
*/


#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream> // for file IO
#include <algorithm> // for sort algorithm
#include "hobbit_reunion.h"
using namespace std;

// Destination is always Bilbo's home, determined when reading the input file
#define TARGET_NAME "Bilbo"
#define MAX_ROW 26 // These two numbers should always be the same for adjacency matrix
#define MAX_COL 26 // These two numbers should always be the same for adjacency matrix
#define INFINITY 1000000000

// Global variables
vector<dwarf_struct> dwarvesvec;
vector<edge_struct> edgesvec;
int adjacencyMat[MAX_ROW][MAX_COL];
int distanceMat[MAX_ROW][MAX_COL];
int traveltimeMat[MAX_ROW][MAX_COL];
int magicalcoinsMat[MAX_ROW][MAX_COL];
int trollsMat[MAX_ROW][MAX_COL];
char TARGET_LOCATION; // Associated with TARGET_NAME

////////////////////////////////////////////////////////////////////////////////

int readHomes(string filename) {
  ifstream infile(filename);
  if (!infile.is_open()) return 0;
  string name;
  vector<string> namesvec;
  while (infile >> name) {
    char city;
    if (name == string(TARGET_NAME)) {
      infile >> city;
      TARGET_LOCATION = city;
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
  return 1;
}

int readMap(string filename) {
  ifstream infile(filename);
  if (!infile.is_open()) return 0;
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
  return 1;
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

void printRoutingInfo(const char * algorithm, int hops[],
  int dist[], int time[], int gold[], int trolls[], string pathArray[]){
  cout << "\n\n" << algorithm << " Algorithm" << endl;
  cout << "Dwarf\tHome\tHops\tDist\tTime\tGold\tTrolls\tPath" << endl;
  cout << "----------------------------------------------------------------" << endl;
  int avHops=0, avDist=0, avTime=0, avGold=0, avTrolls=0;
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
    avHops += hops[index];
    avDist += dist[index];
    avTime += time[index];
    avGold += gold[index];
    avTrolls += trolls[index];
  }
  cout << "----------------------------------------------------------------" << endl;
  cout << "Average:\t";
  cout << setprecision(2) << fixed << (double)avHops/dwarvesvec.size() << "\t";
  cout << setprecision(2) << fixed << (double)avDist/dwarvesvec.size() << "\t";
  cout << setprecision(2) << fixed << (double)avTime/dwarvesvec.size() << "\t";
  cout << setprecision(2) << fixed << (double)avGold/dwarvesvec.size() << "\t";
  cout << setprecision(2) << fixed << (double)avTrolls/dwarvesvec.size() << endl;
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
  // ASSUMPTIONS:
  //   Graph is always connected
  //   Undirected graph
  cout << "Starting Global Dwarf Repositioning System (GDPS)..." << endl;


  // Read input files
  while (1) {
    cout << "Enter the filename containing the hobbit/dwarf names and homes..." << endl;
    cout << "Format: name location" << endl;
    string h;
    cin >> h;
    int rv = readHomes(h);
    if (rv) break;
    cout << "Error opening the file, please try again..." << endl;
  }
  cout << "Successfully read home locations file\n" << endl;

  // Read input files
  while (1) {
    cout << "Enter the filename containing the world map data..." << endl;
    cout << "Format: location1 location2 distance traveltime magicalcoins trolls" << endl;
    string h;
    cin >> h;
    int rv = readMap(h);
    if (rv) break;
    cout << "Error opening the file, please try again..." << endl;
  }
  cout << "Successfully read map locations file\n" << endl;


  // Initialize data structures
  createAdjacencyMatrix();
  // printAdjacencyMatrix();
  cout << "Successfully created adjacency matrix from input files";


  // Apply routing algorithms
  while (1) {
    cout << "\n\n\nEnter the algorithm you would like to route with: (SHP, SDP, STP, FTP, exit)" << endl;
    string uin;
    cin >> uin;

    if (uin == "SHP") {
      shortestHopPath();
      cout << "Finished running SHP algorithm" << endl;
    } else if (uin == "SDP") {
      shortestDistancePath();
      cout << "Finished running SDP algorithm" << endl;
    } else if (uin == "STP") {
      shortestTimePath();
      cout << "Finished running STP algorithm" << endl;
    } else if (uin == "FTP") {
      fewestTrollsPath();
      cout << "Finished running FTP algorithm" << endl;
    } else if (uin == "exit") {
      break;
    } else {
      cout << "Invalid input..." << endl;
    }
  }

  cout << "\nProgram exiting..." << endl;
}
