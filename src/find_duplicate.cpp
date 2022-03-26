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
    set<string> S_d;
    while(cin >> temp)
    {
        if(S.find(temp) == S.end()) S.insert(temp);
        else {
            S_d.insert(temp);
        }
    }
    if(S_d.size() != 0) 
        for(auto i: S_d) cout << i << endl;
    else cout << "none duplicate\n";
    return 0;
}
