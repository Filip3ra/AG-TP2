#include "header.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;

vector<int> goalState = {0, 1, 2, 3, 4, 5, 6, 7, 8};

void calculateHeuristic(Node node)
{

  int manhattanDistance = 0;
  for (int i = 0; i < node.board.size(); ++i)
  {
    if (node.board[i] != goalState[i])
    {
      node.h_score += 1;
    }
  }
  node.f_score = node.g_score + node.h_score;
}

void a_star(const Node &start)
{

  queue<Node> open_list;    // sequencia de estados até a solução final
  vector<Node> closed_list; // estados já visitados

  // insere instância incial
  open_list.push(start);

  while (!open_list.empty())
  {
    auto current = open_list.front();
    open_list.pop();

    // Verifica se chegou no resultado final
    if (current.board == goalState)
    {
      // implementar
      cout << "Encontrei" << endl;
      break;
    }

    // coloca estado na lista de nós visitados
    closed_list.push_back(current);

    // Gera lista de sucessores
    vector<Node> successors = generateSuccessors(current);

    for (int i = 0; i < successors.size(); i++)
    {
      /* preciso comparar se um sucessor é um estado que já visitei */

      if (true)                      // se encontrei um sucessor na lista de estados...
        continue;                    // ...então ele já foi visitado, logo, pula ele
      open_list.push(successors[i]); // caso contrário adiciona na outra lista
    }
  }
}

vector<Node> generateSuccessors(const Node parent)
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
    newBoard[i] = neighbors[j];

    // gera um nó, calcula os valores g, h, f
    Node node(newBoard, parent.g_score + 1, 0, 0);
    calculateHeuristic(node);
    successors[j] = node;
  }

  return successors;
}

/* mostra os vizinhos válidos dada uma posição
Goal state:
0 1 2
3 4 5
6 7 8          */
vector<int> getNeighbor(int pos)
{
  vector<int> values;
  switch (pos)
  {
  case 0:
    values = {1, 3};
    break;
  case 1:
    values = {0, 2, 4};
    break;
  case 2:
    values = {1, 5};
    break;
  case 3:
    values = {0, 4, 6};
    break;
  case 4:
    values = {1, 3, 5, 7};
    break;
  case 5:
    values = {2, 4, 8};
    break;
  case 6:
    values = {3, 7};
    break;
  case 7:
    values = {6, 4, 8};
    break;
  case 8:
    values = {5, 7};
    break;
  default:
    cerr << "Posição inválida!" << endl;
    break;
  }
  return values;
}
