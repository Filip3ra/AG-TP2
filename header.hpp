#pragma once

#include <vector>
#include <string>

using namespace std;

class FileManager
{
public:
  void readFile(const string &filename);
  const vector<int> &getVector() const;
  vector<int> getNeighbor(int);

private:
  vector<int> data;
  vector<int> values;
};
