#include <iostream>
#include "header.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    string searchType;

    if (argc < 2)
    {
        cerr << "Por favor, forneça o nome do arquivo como argumento." << endl;
        return 1;
    }
    else if(argc < 3) {
        searchType = "aStar"; 
    }
    else {

        searchType = argv[2];
    }

    string filename = argv[1];

    FileManager fileManager;
    fileManager.readFile(filename);

    const vector<vector<int>> &data = fileManager.getVector();
    
    cout << "passos tempo" << endl;
    for(unsigned i=0; i<data.size(); ++i)
    {
      Node node(data[i], 0, 0, 0, 0);
      if (!searchType.compare("idaStar")) {
        ida_star(node);
      }
      else if(!searchType.compare("aStar")) {
        a_star(node);
      }
      else {
        cerr << "Selecione um método de pesquisa válido." << endl;
      }
    }

    return 0;
}
