#include <iostream>
#include "header.hpp"

using namespace std;

int main(int argc, char *argv[]) {

    string filename = argv[1];

    vector<vector<int>> matrix = readFile(filename);
    
    cout << "Matriz lida do arquivo:" << endl;
    for (const auto& row : matrix) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
    
    return 0;
}
