#include <iostream>
#include "header.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Por favor, forneça o nome do arquivo como argumento." << endl;
        return 1;
    }

    string filename = argv[1];

    FileManager fileManager;
    fileManager.readFile(filename);

    // Printa o tabuleiro
    const vector<int> &data = fileManager.getVector();
    /*for (int i = 0; i < data.size(); ++i)
    {
        int num = data[i];
        cout << num << " ";
        if ((i + 1) % 3 == 0)
            cout << endl;
    }
    cout << endl;*/

    //vector<int> instancia = {1,4,2,0,3,5,6,7,8};
    Node node(data, 0, 0, 0, 0);
    a_star(node);
    /*
        // Mostrando todos os vizinhos do block vazio
        vector<int> n;
        for (int x = 0; x < data.size(); x++)
        {
            cout << "Vizinhos do " << data[x] << ": " << endl;
            n = fileManager.getNeighbor(x);
            for (int j = 0; j < n.size(); j++)
                cout << data[n[j]] << " ";
            cout << endl;
        }*/

    return 0;
}
