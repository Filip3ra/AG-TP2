#pragma once

#include <vector>
#include <string>

using namespace std;

class FileManager
{
public:
  void readFile(const string &filename);
  const vector<vector<int>> &getVector() const;

private:
  vector<vector<int>> data;
  vector<int> values;
};

class Node
{
public:
  vector<int> board;
  int g_score; // nível da árvore
  int h_score; // quantidade de blocos fora do lugar
  int f_score; // soma de g + h
  int parent; //  antecessor do Node

  // Construtor para inicializar os valores
  Node(vector<int> board, int g_score, int h_score, int f_score, int parent)
      : board(board), g_score(g_score), h_score(h_score), f_score(f_score), parent(parent) {}

  // Sobrecarga do operator <
  bool operator<(const Node& rhs) const;
};

void a_star(const Node &);
vector<Node> generateSuccessors(const Node);
vector<int> getNeighbor(int);
void calculateHeuristic(Node&);