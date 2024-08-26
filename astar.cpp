#include "header.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;
using namespace chrono;

vector<int> goalState;
vector<vector<int>> positionToCoord;

bool Node::operator<(const Node& rhs) const{
  return this->f_score > rhs.f_score;
}

void calculateHeuristicNWrongs(Node& node)
{
  for (int i = 0; i < node.board.size(); ++i)
  {
    if (node.board[i] != goalState[i])
    {
      node.h_score += 1;
    }
  }
  node.f_score = node.g_score + node.h_score;
}

void calculateHeuristicManhattan(Node& node)
{
  
  int manhattanDistance = 0;
  for (unsigned i=0; i < node.board.size(); ++i) {
    for (unsigned j=0; j < node.board.size(); ++j) {
      if (node.board[j] == i) {
        manhattanDistance += abs(positionToCoord[j][0] - positionToCoord[i][0]) + abs(positionToCoord[j][1] - positionToCoord[i][1]);
        break;
      }
    }
  }
  node.h_score = manhattanDistance;
  node.f_score = node.g_score + node.h_score;
}

double euclidianDistance(vector<int>& p1, vector<int>& p2) {
  return sqrt((p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]));
}

void calculateHeuristicEuclidian(Node& node)
{
  double manhattanDistance = 0;
  for (unsigned i = 0; i < node.board.size(); ++i) {
    for (unsigned j = 0; j < node.board.size(); ++j) {
      if (node.board[j] == i) {
        manhattanDistance += euclidianDistance(positionToCoord[i], positionToCoord[j]);
        break;
      }
    }
  }
  node.h_score = manhattanDistance;
  node.f_score = node.g_score + node.h_score;
}

//  Reconstrói o caminho até a solução
void makePath(Node& solved, vector<Node> closed_list, vector<Node>& stepsList)
{
  Node auxNode = solved;
  while (auxNode.parent != 0) {
    stepsList.push_back(auxNode);
    auxNode = closed_list[auxNode.parent];
  }
  stepsList.push_back(auxNode);
  stepsList.push_back(closed_list[0]);
  reverse(stepsList.begin(), stepsList.end());
}

void a_star(const Node& start)
{
  priority_queue<Node, vector<Node>> open_list;    // sequencia de estados até a solução final
  vector<Node> closed_list; // estados já visitados

  goalState.clear();
  positionToCoord.clear();
  for (unsigned i = 0; i < start.board.size(); ++i) {
    goalState.push_back(i);
  }

  for (int i = sqrt(goalState.size())-1; i >= 0; --i) {
    for (int j = 0; j < sqrt(goalState.size()); ++j) {
      positionToCoord.push_back({ j, i });
    }
  }

  high_resolution_clock::time_point tpStart = high_resolution_clock::now();

  // insere instância incial
  open_list.push(start);
  unsigned iterCount = 0;
  while (!open_list.empty())
  {
    auto current = open_list.top();
    open_list.pop();

    // Verifica se chegou no resultado final
    if (current.board == goalState)
    {
      unsigned timeSpent = duration_cast<milliseconds>(high_resolution_clock::now() - tpStart).count();
      vector<Node> stepsList;
      makePath(current, closed_list, stepsList);
      
      // Printa os passos para chegar na solução
      /*for (auto nodeAux : stepsList) {
        for (int i = 0; i < nodeAux.board.size(); ++i)
        {
          int num = nodeAux.board[i];
          cout << num << " ";
          if ((i + 1) % 3 == 0)
            cout << endl;
        }
        cout << nodeAux.g_score << " " << nodeAux.h_score << " ";
        cout << endl << endl;
      }*/
      cout << stepsList.size()-1 << " " << timeSpent << endl;

      break;
    }

    // coloca estado na lista de nós visitados
    closed_list.push_back(current);

    // Gera lista de sucessores
    vector<Node> successors = generateSuccessors(current, closed_list.size() - 1);

    for (int i = 0; i < successors.size(); i++)
    {
      bool isPresent = false;
      /* comparar se um sucessor é um estado que já visitei */
      for (int j = 0; j < closed_list.size(); ++j)
      {                                                            // se encontrei um sucessor na lista de estados...
        if (successors[i].board == closed_list[j].board)
        {
          if (successors[i].g_score < closed_list[j].g_score) {
            closed_list[j].g_score = successors[i].g_score;
            closed_list[j].parent = successors[i].parent;
            closed_list[i].f_score = successors[i].f_score;
          }
          isPresent = true;
          break;                                                                                             // ...então ele já foi visitado, logo, pula ele
        }
      }
      if (!isPresent) 
      {
        open_list.push(successors[i]);                                                                        // caso contrário adiciona na outra lista
      }
    }
    ++iterCount;
  }
}

unsigned countIterIda = 0;

int search(vector<Node>& path, double lowerBound) {
  ++countIterIda;
  unsigned lastNode = path.size() - 1;
  if (path[lastNode].f_score > lowerBound) return path[lastNode].f_score;
  if (path[lastNode].board == goalState) return -1;
  int min = INT32_MAX;
  vector<Node> successorsVec = generateSuccessors(path[lastNode], path.size()-1);
  priority_queue<Node> successors(successorsVec.begin(), successorsVec.end());
  unsigned nSuccessors = successors.size();
  for (unsigned i = 0; i < nSuccessors; ++i) {
    Node successor = successors.top();
    successors.pop();
    bool isInPath = false;
    for (auto nodeAux : path) {
      if (nodeAux.board == successor.board) {
        isInPath = true;
        break;
      }
    }
    if (!isInPath) {
      path.push_back(successor);
      int result = search(path, lowerBound);
      if (result == -1) return -1;
      if (result < min) min = result;
      path.pop_back();
    }
  }
  return min;
}

void ida_star(Node& start) {

  goalState.clear();
  positionToCoord.clear();

  for (unsigned i = 0; i < start.board.size(); ++i) {
    goalState.push_back(i);
  }

  for (int i = sqrt(goalState.size()) - 1; i >= 0; --i) {
    for (int j = 0; j < sqrt(goalState.size()); ++j) {
      positionToCoord.push_back({ j, i });
    }
  }

  high_resolution_clock::time_point tpStart = high_resolution_clock::now();

  calculateHeuristicManhattan(start);
  double lowerBound = start.h_score;
  vector<Node> path;
  path.push_back(start);
  while (true) {
    int result = search(path, lowerBound);
    if (result == -1) {
      unsigned timeSpent = duration_cast<milliseconds>(high_resolution_clock::now() - tpStart).count();
      // printa o passo a passo da solução
      /*for (auto nodeAux : path) {
        for (int i = 0; i < nodeAux.board.size(); ++i)
        {
          int num = nodeAux.board[i];
          printf("%02d ", num);
          if ((i + 1) % (int)(sqrt(start.board.size())) == 0)
            cout << endl;
        }
        cout << nodeAux.g_score << " " << nodeAux.h_score << " ";
        cout << endl << endl;
      }*/
      cout << path.size() - 1 << " " << timeSpent << " " << countIterIda << endl;
      return;
    }
    if (result == UINT32_MAX) return;
    lowerBound = result;
  }
}

vector<Node> generateSuccessors(const Node parent, const unsigned parentIndex)
{
  vector<int> neighbors;
  int i;

  // encontra a posição vazia pra poder saber quais vizinhos posso trocar
  for (i = 0; i < parent.board.size(); i++)
  {
    if (parent.board[i] == 0)
    {
      neighbors = getNeighbor(i); // lista de vizinhos que o bloco vazio pode trocar
      break;
    }
  }

  vector<Node> successors;

  for (int j = 0; j < neighbors.size(); j++)
  {
    vector<int> newBoard = parent.board;

    // faz uma troca e gera um novo tabuleiro
    int aux = newBoard[neighbors[j]];
    newBoard[neighbors[j]] = 0;
    newBoard[i] = aux;

    // gera um nó, calcula os valores g, h, f
    Node node(newBoard, parent.g_score + 1, 0, 0, parentIndex);
    calculateHeuristicManhattan(node);
    successors.push_back(node);
  }

  return successors;
}

/* mostra os vizinhos válidos dada uma posição*/
vector<int> getNeighbor(int pos)
{
  vector<int> values;
  int size = goalState.size();
  int side = sqrt(size);

  if (pos < (size-side)) {
    values.push_back(pos + side);
  }
  if (pos > (side-1)) {
    values.push_back(pos - side);
  }
  if ((pos % side)) {
    values.push_back(pos - 1);
  }
  if (abs(pos - (side - 1)) % side) {
    values.push_back(pos + 1);
  }

  return values;
}