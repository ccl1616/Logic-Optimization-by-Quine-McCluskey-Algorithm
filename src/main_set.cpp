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

// set version, pass all testcases

//Main class
class QM
{
public:

int VARIABLES;
string dontcares;

QM(int a)
{
   VARIABLES=a;
   dontcares.append(a,'-');
}

// function to check if two terms differ by just one bit
bool isGreyCode(string a,string b)
{
    int flag = 0;
    for(int i = 0; i < a.length(); i ++)
    {
        if(a[i] != b[i])
        flag ++;
    }
    return (flag == 1);
}

// function to replace complement terms with don't cares
// Eg: 0110 and 0111 becomes 011-
string replace_complements(string a,string b)
{
    string temp="";
    for(int i = 0; i < a.length(); i ++)
    if(a[i] != b[i])
        temp = temp + "-";
    else
        temp = temp + a[i];

    return temp;
}

// function to reduce minterms
set<string> reduce(set<string> &minterms)
{

    set<string> newminterms;
    set<string> checked;
    int n = minterms.size();

    set<string>::iterator it = minterms.begin();
    set<string>::iterator it_b = minterms.begin();
    for(it = minterms.begin(); it != minterms.end(); it ++) {
        for(it_b = minterms.begin(); it_b != minterms.end(); it_b ++) {
            string temp_a = (*it);
            string temp_b = (*it_b);
            if(isGreyCode(temp_a, temp_b))
            {
                string replaced = replace_complements(temp_a, temp_b); 
                newminterms.insert(replaced);

                checked.insert(temp_a);
                checked.insert(temp_b);
            }
        }
    }

    // appending non checked
    for(it = minterms.begin(); it != minterms.end(); it ++){
        if(checked.find(*it) == checked.end())
            newminterms.insert(*it);
    }
    return newminterms;
}

// function to check if 2 sets are equal
bool SetEqual(set<string> a,set<string> b)
{
   if(a.size() != b.size())
      return false;

    set<string>::iterator it;
    set<string>::iterator it_b = b.begin();
    for(it = a.begin(); it != a.end(); it ++, it_b ++)
    {
        string temp_a = (*it);
        string temp_b = (*it_b);
        if( temp_a.compare(temp_b) != 0)
            return false;
    }
    return true;
}

// count the literals
int literal_count(set<string> minterms)
{
    int count = 0;
    for(auto i: minterms)
        for(auto j: i) 
            if(j != '-') count ++;
    return count;
}

};

// Main function
int main (int argc, char* argv[])
{
    string path(argv[1]);
    ifstream cin("./testcases/" + path); // depend on execution file relative path
    ofstream cout(argv[2]);

    int var, pro;
    cin >> var >> pro;
    QM q(var);

    // get inputs
    string temp;
    set<string> minterms;
    while(cin >> temp)
    {
        minterms.insert(temp);
    }
    int times = 0;
    do
    {
        times ++;
        minterms=q.reduce(minterms);
    }while(!q.SetEqual(minterms,q.reduce(minterms)) );

    cout << q.literal_count(minterms) << endl << minterms.size() << endl;
    for(auto i: minterms)
        cout << i << endl;
    return 0;
}