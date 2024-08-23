#include "header.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;

vector<int> goalState = {0, 1, 2, 3, 4, 5, 6, 7, 8};

bool Node::operator<(const Node& rhs) const{
  return this->f_score > rhs.f_score;
}

void calculateHeuristic(Node& node)
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

//  Reconstrói o caminho até a solução
void makePath(Node& solved, vector<Node> closed_list, vector<Node>& stepsList) {

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

  // insere instância incial
  open_list.push(start);

  while (!open_list.empty())
  {
    auto current = open_list.top();
    open_list.pop();

    // Verifica se chegou no resultado final
    if (current.board == goalState)
    {
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
      cout << "Numero de passos: " << stepsList.size() << endl;

      break;
    }

    // coloca estado na lista de nós visitados
    closed_list.push_back(current);

    // Gera lista de sucessores
    vector<Node> successors = generateSuccessors(current);

    for (int i = 0; i < successors.size(); i++)
    {
      bool isPresent = false;
      /* comparar se um sucessor é um estado que já visitei */
      for (int j = 0; j < closed_list.size(); ++j) {                                                            // se encontrei um sucessor na lista de estados...
        if (successors[i].board == closed_list[j].board) {
          isPresent = true;
          break;                                                                                             // ...então ele já foi visitado, logo, pula ele
        }
      }
      if (!isPresent) {
        successors[i].parent = closed_list.size() - 1;
        open_list.push(successors[i]);                                                                        // caso contrário adiciona na outra lista
      }
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
    newBoard[i] = aux;

    // gera um nó, calcula os valores g, h, f
    Node node(newBoard, parent.g_score + 1, 0, 0, 0);
    calculateHeuristic(node);
    successors.push_back(node);
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
