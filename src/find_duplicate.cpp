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

// count the literals
int literal_count(set<string> minterms)
{
    int count = 0;
    for(auto i: minterms)
        for(auto j: i) 
            if(j != '-') count ++;
    return count;
}

int main (int argc, char* argv[])
{
    string path(argv[1]);
    ifstream cin("./testcases/" + path); // depend on compiled relative path
    ofstream cout(argv[2]);

    int var, pro;
    cin >> var >> pro;

    // get inputs
    string temp;
    set<string> input_set;
    bool duplicate = false;
    set<string> dup;
    while(cin >> temp)
    {
        if(input_set.find(temp) == input_set.end()) input_set.insert(temp);
        else {
            dup.insert(temp);
        }
    }
    // if(S_d.size() != 0) 
    //     for(auto i: S_d) cout << i << endl;
    // else cout << "none duplicate\n";

    // print out the non-duplicate input list, and do the verify
    cout << literal_count(input_set) << endl;
    cout << input_set.size() << endl;
    for(auto i: input_set)
        cout << i << endl;

    return 0;
}
