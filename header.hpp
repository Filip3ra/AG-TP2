#pragma once

#include <vector>
#include <string>

using namespace std;

class FileManager
{
public:
  bool readFile(const string &filename);
  const vector<vector<int>> &getVector() const;

private:
  vector<vector<int>> data;
};

class Node
{
public:
  vector<int> board;
  int g_score; // nível da árvore
  double h_score; // quantidade de blocos fora do lugar
  double f_score; // soma de g + h
  int parent; //  antecessor do Node

  // Construtor para inicializar os valores
  Node(vector<int> board, int g_score, int h_score, int f_score, int parent)
      : board(board), g_score(g_score), h_score(h_score), f_score(f_score), parent(parent) {}

  // Sobrecarga do operator <
  bool operator<(const Node& rhs) const;
};

void a_star(Node &, const bool printSteps);
vector<Node> generateSuccessors(const Node, const unsigned parentIndex);
vector<int> getNeighbor(int);
void ida_star(Node& start, const bool printSteps);