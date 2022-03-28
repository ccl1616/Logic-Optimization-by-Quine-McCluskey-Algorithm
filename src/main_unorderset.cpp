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
#include <map>
#include <queue>
#include <unordered_set>
#include <fstream>
using namespace std;

bool expand_flag = true;
bool cc_flag = true; // column covering or not
// expand new version + column covering ; unordered_set, error output

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
void trans(unordered_set<string> &st)
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
bool set_clean(unordered_set<string> st)
{
    for(auto i: st) {
        if(i.find('-') < i.size()) return false;
    }
    return true;
} 
unordered_set<string> expand(string in)
{
    unordered_set<string> ret;
    queue<string> Q;
    // insert all terms into Q
    Q.push(in);
    // while Q not empty
    while(!Q.empty()) {
        string temp = Q.front();
        Q.pop();
        if(temp.find('-') == string::npos) {
            ret.insert(temp);
            continue;
        }
        for(int i = 0; i < temp.size(); i ++) {
            if(temp[i] == '-') {
                temp[i] = '0'; Q.push(temp);
                temp[i] = '1'; Q.push(temp);
                break;
            }
        }
    }
    return ret;
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
unordered_set<string> reduce(unordered_set<string> &minterms)
{

    unordered_set<string> newminterms;
    unordered_set<string> checked;
    int n = minterms.size();

    unordered_set<string>::iterator it = minterms.begin();
    unordered_set<string>::iterator it_b = minterms.begin();
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
bool SetEqual(unordered_set<string> a,unordered_set<string> b)
{
   if(a.size() != b.size())
      return false;

    unordered_set<string>::iterator it;
    unordered_set<string>::iterator it_b = b.begin();
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
int literal_count(unordered_set<string> minterms)
{
    int count = 0;
    for(auto i: minterms)
        for(auto j: i) 
            if(j != '-') count ++;
    return count;
}

}; // class end

class CC
{
public:
    unordered_set<string> minterms;
    unordered_set<string> implicants;
    

CC(unordered_set<string> m, unordered_set<string> imp)
{
    minterms = m;
    implicants = imp;
}

bool imp_covers_min(string min, string imp)
{
    int n = imp.size();
    for(int i = 0; i < n; i ++) {
        if(min[i] != imp[i] && imp[i] != '-') 
            return false;
    }
    return true;
}

unordered_set<string> check_column_cover()
{
    map<string, vector<string> > mp1; // minterms-implicants
    map<string, vector<string> > mp2; // implicants-minterms
    unordered_set<string> essential;
    // build two map
    for(auto i: minterms) {
        // cout << "minterms: " << i << endl;
        for(auto j: implicants) {
            if(imp_covers_min(i, j)) {
                mp1[i].push_back(j);
                mp2[j].push_back(i);
                // cout << j << endl;
            }
        }
        if(mp1[i].size() == 1) essential.insert(mp1[i][0]); // this minterm only has this implicant
    }
    // go through essential, kick out its minterms
    for(auto i: essential) {
        for(auto j: mp2[i])
            mp1.erase(j);
    }
    // pick up remainder minterm's implicant 
    for (auto const& x : mp1)
        essential.insert(x.second[0]);

    // return result
    return essential;
}
}; // CC end

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
    unordered_set<string> minterms; // going to be reduced
    
    // expand input or not
    if(expand_flag) {
        while(cin >> temp)
        {   
            unordered_set<string> st;
            // st.insert(temp);
            // while(!q.set_clean(st)) 
            //     q.trans(st);
            st = q.expand(temp);
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
    unordered_set<string> minterms_copy(minterms); // non reduced minterms

    // reduce by iteration until can't be reduced anymore
    do
    {
        minterms=q.reduce(minterms);
    }while(!q.SetEqual(minterms,q.reduce(minterms)) );
    // }while( ! (minterms == q.reduce(minterms)));

    if(!cc_flag) {
        // non column covering
        cout << q.literal_count(minterms) << endl << minterms.size() << endl;
        for(auto i: minterms)
            cout << i << endl;
    }
    else {
        // column covering
        CC table(minterms_copy, minterms);
        unordered_set<string> essential;
        essential = table.check_column_cover();
        cout << q.literal_count(essential) << endl << essential.size() << endl;
        for(auto i: essential)
            cout << i << endl;
    }
    return 0;
}