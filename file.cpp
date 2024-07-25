#include "header.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<vector<int>> readFile(const string &filename)
{
  ifstream file(filename);
  vector<vector<int>> board;

  if (!file.is_open())
  {
    cerr << "Erro ao abrir o arquivo!" << endl;
    return board;
  }

  string line;
  while (getline(file, line))
  {
    istringstream iss(line);
    vector<int> row;
    int num;
    int count = 0;

    while (iss >> num)
    {
      cout << num;
      row.push_back(num);
      count++;

      if (count == 3) // Para ler apenas 3 valores por linha
      {
        board.push_back(row);
        row.clear();
        count = 0;
      }
    }
  }

  file.close();
  return board;
}
