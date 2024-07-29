#include "header.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void FileManager::readFile(const string &filename)
{
  ifstream file(filename);

  if (!file.is_open())
  {
    cerr << "Erro ao abrir o arquivo!" << endl;
    return;
  }

  string line;
  data.clear();
  while (getline(file, line))
  {
    istringstream iss(line);
    int num;
    while (iss >> num)
    {
      data.push_back(num);
    }
  }

  file.close();
}

vector<int> FileManager::getNeighbor(int pos)
{
  values.clear();
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

const vector<int> &FileManager::getVector() const
{
  return data;
}
