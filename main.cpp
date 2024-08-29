#include <iostream>
#include "header.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    string searchType;
    bool printSteps;

    if (argc < 2)
    {
        cout << "Use help para informações de como usar o programa." << endl;
        return 1;
    }

    string paramAux = argv[1];


    if (!paramAux.compare("help") || !paramAux.compare("-help") || !paramAux.compare("--help")) {
      cout << "./program [INSTPATH] [SEARCHOPTION] [PRINTSTEPS] \n\n";
      cout << "INSTPATH - Caminho para a instância.\n";
      cout << "SEARCHOPTION - use idaSearch ou aSearch. Usa aSearch como padrão\n";
      cout << "PRINTSTEPS - defina como print ou dontPrint para imprimir todos os passos da solução. Usa dontPrint como padrão.\n";
      return 1;
    }
    if(argc  >= 2) {
        searchType = "aSearch";
        printSteps = false;
    }
    if(argc >= 3){
      paramAux = argv[2];
      if (paramAux.compare("aSearch") && paramAux.compare("idaSearch")) {
        cout << "Insira um tipo de busca válida. Use help para informações de como usar o programa.\n";
        return 1;
      }
      searchType = paramAux;
    }
    if (argc >= 4) {
      paramAux = argv[3];
      if (paramAux.compare("print") && paramAux.compare("dontPrint")) {
        cout << "Insira um tipo de exibição do resultado válido. Use help para informações de como usar o programa.\n";
        return 1;
      }
      printSteps = !paramAux.compare("print");
    }
    if(argc >= 5){
      cout << "Insira número válido de parâmetros. Use help para informações de como usar o programa.\n";
      return 1;
    }

    string filename = argv[1];

    FileManager fileManager;
    if (!fileManager.readFile(filename)) {
      return 1;
    }

    const vector<vector<int>> &data = fileManager.getVector();
   
    for(unsigned i=0; i<data.size(); ++i)
    {
      Node node(data[i], 0, 0, 0, 0);
      if (!searchType.compare("idaSearch")) {
        ida_star(node, printSteps);
      }
      else if(!searchType.compare("aSearch")) {
        a_star(node, printSteps);
      }
    }

    return 0;
}
