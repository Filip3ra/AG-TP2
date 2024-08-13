#pragma once

#include <vector>
#include <string>

using namespace std;

class FileManager
{
public:
  void readFile(const string &filename);
  const vector<int> &getVector() const;

private:
  vector<int> data;
  vector<int> values;
};
/*
class Node
{
public:
  vector<int> getNeighbor(int);
  Node *a_star(const Node);
  void calculateHeuristic(Node);
  Node(vector<int> board, int g_score, int h_score, int f_score);
};*/

class Node
{
public:
  vector<int> board;
  int g_score; // nível da árvore
  int h_score; // quantidade de blocks fora do lugar
  int f_score; // soma de g + h

  // Construtor para inicializar os valores
  Node(vector<int> board, int g_score, int h_score, int f_score)
      : board(board), g_score(g_score), h_score(h_score), f_score(f_score) {}

  vector<int> getNeighbor(int);
  Node *a_star(const Node);
  void calculateHeuristic(Node);
  Node(vector<int> board, int g_score, int h_score, int f_score);
};