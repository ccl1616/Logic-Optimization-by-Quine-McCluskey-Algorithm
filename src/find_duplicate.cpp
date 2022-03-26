#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <set>
#include <fstream>

using namespace std;

int main (int argc, char* argv[])
{
    string path(argv[1]);
    ifstream cin("./testcases/" + path); // depend on compiled relative path
    // ofstream cout(argv[2]);

    set<string> inputs;
    int var, pro;
    cin >> var >> pro;

    // get inputs
    string temp;
    set<string> S;
    bool duplicate = false;
    while(cin >> temp)
    {
        if(S.find(temp) == S.end()) S.insert(temp);
        else duplicate = true;
    }
    cout << "duplicate: " << duplicate << endl;
    return 0;
}
