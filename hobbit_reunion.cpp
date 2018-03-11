#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "hobbit_reunion.h"
using namespace std;

void readHomes(string filename, vector<node_struct> &nodes) {
  ifstream infile(filename);
  if (!infile.is_open()) return;
  string token;
  node_struct ns;
  while (infile >> token) {
    ns.name = token;
    char city;
    infile >> city;
    ns.location = city;
    // cout << ns.name << ns.location << endl;
    nodes.push_back(ns);
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

int main() {
  vector<node_struct> nodes;
  vector<edge_struct> edges;

  readHomes("canadahomes.txt", nodes);
  readMap("canadamap.txt", edges);
  // cout << output << endl;
}
