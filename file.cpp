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

const vector<int> &FileManager::getVector() const
{
  return data;
}