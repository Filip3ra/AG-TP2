#include "header.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

bool FileManager::readFile(const string &filename)
{
  ifstream file(filename);

  if (!file.is_open())
  {
    cerr << "Não foi possível abrir o arquivo passado como parâmetro!" << endl;
    return false;
  }

  string line;
  data.clear();
  while (getline(file, line))
  {
    vector<int> auxVec;
    istringstream iss(line);
    int num;
    while (iss >> num)
    {
      auxVec.push_back(num);
    }
    data.push_back(auxVec);
  }

  file.close();

  return true;
}

const vector<vector<int>> &FileManager::getVector() const
{
  return data;
}
