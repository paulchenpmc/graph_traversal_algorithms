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

int readHomes(string filename);
int readMap(string filename);
void createAdjacencyMatrix();
void printAdjacencyMatrix();
void printRoutingInfo(const char * algorithm, int hops[], int dist[], int time[], int gold[], int trolls[], string pathArray[]);
int minDist(int distances[], bool sptSet[]);
void shortestHopPath();
void shortestDistancePath();
void shortestTimePath();
void fewestTrollsPath();

#endif
