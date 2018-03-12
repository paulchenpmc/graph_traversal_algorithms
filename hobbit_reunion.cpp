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

void shortestHopPath() {

}

int main() {
  // Initialize data structures
  vector<dwarf_struct> dwarves;
  vector<edge_struct> edges;

  // Read input files
  readHomes("canadahomes.txt", dwarves);
  readMap("canadamap.txt", edges);

  // Apply routing algorithms

}
