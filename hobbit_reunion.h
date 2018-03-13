#ifndef HOBBIT_REUNION_H
#define HOBBIT_REUNION_H

#include <string>
#include <vector>
using namespace std;

struct edge_struct {
  char location1;
  char location2;
  int distance;
  int traveltime;
  int magicalcoins;
  int trolls;
};

struct dwarf_struct {
  string name;
  char location;
};

void readHomes(string filename, vector<dwarf_struct> &nodes);
void readMap(string filename, vector<edge_struct> &edges);
void createAdjacencyMatrix(vector<edge_struct> &edges);

#endif