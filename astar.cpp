#include "header.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <list>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <set>

using namespace std;
using namespace chrono;

vector<int> goalState;

bool Node::operator<(const Node& rhs) const{
  return this->f_score > rhs.f_score;
}

void calculateHeuristicLCMD(Node& node) {
  vector<int> board = node.board;
  int sideSize = sqrt(board.size());

  int linearConflict = 0;
  int manhattanDistance = 0;

  for (int row = 0; row < sideSize; ++row) {
    for (int col = 0; col < sideSize; ++col) {
      int leftTile = row * sideSize + col;

      if (!board[leftTile]) {
        continue;
      }

      for (int colAux = col + 1; colAux < sideSize; ++colAux) {
        int rightTile = row * sideSize + colAux;

        if (!board[rightTile]) {
          continue;
        }

        int goalRowLeft = board[leftTile]/ sideSize;
        int goalRowRight = board[rightTile] / sideSize;

        if (goalRowLeft != goalRowRight || goalRowLeft != row) {
          continue;
        }
        
        if (board[leftTile] > board[rightTile]) {
          linearConflict++;
        }
      }
    }
  }

  for (int row = 0; row < sideSize; ++row) {
    for (int col = 0; col < sideSize; ++col) {
      int leftTile = row * sideSize + col;

      if (!board[leftTile]) {
        continue;
      }

      for (int rowAux = row + 1; rowAux < sideSize; ++rowAux) {
        int belowTile = rowAux * sideSize + col;

        if (!board[belowTile]) {
          continue;
        }

        int goalColAbove = board[leftTile] % sideSize;
        int goalColBelow = board[belowTile] % sideSize;

        if (goalColAbove != goalColBelow || goalColAbove != col) {
          continue;
        }

        if (board[leftTile] > board[belowTile]) {
          linearConflict++;
        }
      }
    }
  }

  for (int i = 0; i < sideSize; ++i) {
    for (int j = 0; j < sideSize; ++j) {
      manhattanDistance += abs(i - (node.board[i*sideSize+j] / sideSize)) + abs(j - (node.board[i * sideSize + j] % sideSize));
    }
  }

  node.h_score = linearConflict + manhattanDistance;
  node.f_score = node.g_score + node.h_score;
}

void calculateHeuristicManhattan(Node& node)
{
  int manhattanDistance = 0;

  int sideSize = sqrt(node.board.size());

  for (int i = 0; i < sideSize; ++i) {
    for (int j = 0; j < sideSize; ++j) {
      manhattanDistance += abs(i - (node.board[i * sideSize + j] / sideSize)) + abs(j - (node.board[i * sideSize + j] % sideSize));
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

void a_star(Node& start, const bool printSteps)
{
  priority_queue<Node, vector<Node>> open_list;    // sequencia de estados até a solução final
  vector<Node> closed_list; // estados já visitados

  goalState.clear();
  for (unsigned i = 0; i < start.board.size(); ++i) {
    goalState.push_back(i);
  }

  high_resolution_clock::time_point tpStart = high_resolution_clock::now();

  // insere instância incial
  calculateHeuristicLCMD(start);
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
      if(printSteps)
      {
        for (auto nodeAux : stepsList) {
          for (unsigned i = 0; i < nodeAux.board.size(); ++i)
          {
            int num = nodeAux.board[i];
            cout << num << " ";
            if ((i + 1) % (int)(sqrt(start.board.size())) == 0)
              cout << endl;
          }
          cout << "g: " << nodeAux.g_score << " h: " << nodeAux.h_score << endl << endl;
        }
      }
      cout << "Passos: " << stepsList.size() - 1 << " Tempo Gasto: " << timeSpent <<  " Nós avaliados: " << closed_list.size()  <<endl;

      break;
    }

    // coloca estado na lista de nós visitados
    closed_list.push_back(current);

    // Gera lista de sucessores
    vector<Node> successors = generateSuccessors(current, closed_list.size() - 1);

    for (unsigned i = 0; i < successors.size(); i++)
    {
      bool isPresent = false;
      /* comparar se um sucessor é um estado que já visitei */
      for (unsigned j = 0; j < closed_list.size(); ++j)
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

int countIters = 0;

int search(vector<Node>& path,double lowerBound) {
  unsigned lastNode = path.size() - 1;
  if (path[lastNode].f_score > lowerBound) return path[lastNode].f_score;
  if (path[lastNode].board == goalState) return -1;
  countIters++;
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
      int result = search(path,lowerBound);
      if (result == -1) return -1;
      if (result < min) min = result;
      path.pop_back();
    }
  }
  return min;
}


void ida_star(Node& start, const bool printSteps) {

  goalState.clear();

  for (unsigned i = 0; i < start.board.size(); ++i) {
    goalState.push_back(i);
  }

  high_resolution_clock::time_point tpStart = high_resolution_clock::now();

  calculateHeuristicLCMD(start);
  double lowerBound = start.h_score;
  vector<Node> path;
  set<Node> auxPath;
  path.push_back(start);
  while (true) {
    int result = search(path,lowerBound);
    if (result == -1) {
      unsigned timeSpent = duration_cast<milliseconds>(high_resolution_clock::now() - tpStart).count();
      // printa o passo a passo da solução
      if(printSteps)
      {
        for (auto nodeAux : path) {
          for (unsigned i = 0; i < nodeAux.board.size(); ++i)
          {
            int num = nodeAux.board[i];
            printf("%02d ", num);
            if ((i + 1) % (int)(sqrt(start.board.size())) == 0)
              cout << endl;
          }
          cout << "g: " << nodeAux.g_score << " h:" << nodeAux.h_score << endl << endl;
        }
      }
      cout << "Passos: " << path.size() - 1 << " Tempo Gasto: " << timeSpent << " Iteracoes recursivas: " << countIters << endl << endl;
      return;
    }
    if (result == INT32_MAX) return;
    lowerBound = result;
  }
}

vector<Node> generateSuccessors(const Node parent, const unsigned parentIndex)
{
  vector<int> neighbors;
  unsigned i;

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

  for (unsigned j = 0; j < neighbors.size(); j++)
  {
    vector<int> newBoard = parent.board;

    // faz uma troca e gera um novo tabuleiro
    int aux = newBoard[neighbors[j]];
    newBoard[neighbors[j]] = 0;
    newBoard[i] = aux;

    // gera um nó, calcula os valores g, h, f
    Node node(newBoard, parent.g_score + 1, 0, 0, parentIndex);
    calculateHeuristicLCMD(node);
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