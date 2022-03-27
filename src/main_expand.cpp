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

bool expand_flag = false;
// expand version

// Main class
class QM
{
public:

int VARIABLES;
string dontcares;

QM(int a)
{
   VARIABLES = a;
}

// expand the dont care
void trans(set<string> &st)
{
    for(auto s: st) {
        for(int i = 0; i < s.size(); i ++) {
            if(s[i] == '-') {
                string b = s;
                b[i] = '0'; st.insert(b);
                b[i] = '1'; st.insert(b);
                st.erase(s);
            }
        }
    }
}
bool set_clean(set<string> st)
{
    for(auto i: st) {
        if(i.find('-') < i.size()) return false;
    }
    return true;
} 
// expand the dont care end

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

// function to replace complements with '-'
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

}; // class end

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

    // expand input or not
    if(expand_flag) {
        while(cin >> temp)
        {   
            set<string> st;
            st.insert(temp);
            while(!q.set_clean(st)) 
                q.trans(st);
            for(auto i: st) minterms.insert(i);
            // minterms.insert(temp);
        }
    }
    else {
        while(cin >> temp)
        {
            minterms.insert(temp);
        }
    }

    // reduce by iteration until can't be reduced anymore
    do
    {
        minterms=q.reduce(minterms);
    }while(!q.SetEqual(minterms,q.reduce(minterms)) );

    cout << q.literal_count(minterms) << endl << minterms.size() << endl;
    for(auto i: minterms)
        cout << i << endl;
    return 0;
}