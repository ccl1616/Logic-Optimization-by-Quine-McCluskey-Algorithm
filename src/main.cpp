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
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <fstream>
using namespace std;

bool expand_flag = true;
bool classify = true;
// stable final version *************************************************************************

// QM class
class QM
{
public:

int var, pro;

QM(int v, int p)
{
   var = v;
   pro = p;
}
// function to count the literals
int literal_count(unordered_set<string> minterms)
{
    int count = 0;
    for(auto i: minterms)
        for(auto j: i) 
            if(j != '-') count ++;
    return count;
}

// function to expand the dont care
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
bool differ_by_one_bit(string a,string b)
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
    string temp = "";
    for(int i = 0; i < a.length(); i ++)
    if(a[i] != b[i])
        temp = temp + "-";
    else
        temp = temp + a[i];

    return temp;
}
// non-classified QM methods ===============================================================
// function to reduce one QM column
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
            if(differ_by_one_bit(temp_a, temp_b))
            {
                string replaced = replace_complements(temp_a, temp_b); 
                newminterms.insert(replaced);

                checked.insert(temp_a);
                checked.insert(temp_b);
            }
        }
    }

    // appending non checked terms
    for(it = minterms.begin(); it != minterms.end(); it ++){
        if(checked.find(*it) == checked.end())
            newminterms.insert(*it);
    }
    return newminterms;
}
// function to check if 2 unordered_sets are equal
bool unordered_setEqual(unordered_set<string> a,unordered_set<string> b)
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
// non-classified QM methods ends ===============================================================

// classified QM methods ===============================================================
// function to classify unordered_set into unordered_map with groups
unordered_map<int, unordered_set<string> > classifier(unordered_set<string> minterms)
{
    unordered_map<int, unordered_set<string> > ret;
    for(auto i: minterms) {
        int count = 0;
        for(auto c: i) {
            if(c == '1') count ++;
        }
        ret[count].insert(i);
    }
    return ret;
}
// function to reduce one QM column (classified)
unordered_map<int, unordered_set<string> > classify_reduce(unordered_map<int, unordered_set<string>> cm) // cm for classify_minterms
{
    unordered_map<int, unordered_set<string>> ret;
    unordered_set<string> checked; // saving checked minterms
    // cm, group by #ones
    for(auto group: cm) {
        int cur = group.first;
        int next = cur + 1;
        auto it = cm.find(next);
        if(it == cm.end()) continue; // no next group exist

        auto unordered_set1 = cm[cur];
        auto unordered_set2 = cm[next];

        // check pairs within these two unordered_sets
        for(auto it1 = unordered_set1.begin(); it1 != unordered_set1.end(); it1 ++) {
            for(auto it2 = unordered_set2.begin(); it2 != unordered_set2.end(); it2 ++) {
                string temp_a = *it1;
                string temp_b = *it2;
                if(differ_by_one_bit(temp_a, temp_b))
                {
                    string replaced = replace_complements(temp_a, temp_b); 
                    ret[ cur ].insert(replaced);

                    checked.insert(temp_a);
                    checked.insert(temp_b);
                }
            }
        }
    }
    // appending non checked minterms
    for(auto i: cm) {
        for(auto j: i.second) {
            if( checked.find(j) == checked.end() ) ret[i.first].insert(j);
        }
    }
    return ret;
}
// fundtion to check two unordered_maps are equal
bool unordered_mapEqual(unordered_map<int, unordered_set<string>> a, unordered_map<int, unordered_set<string>> b)
{
    if(a.size() != b.size())
      return false;

    unordered_map<int, unordered_set<string>>::iterator it;
    unordered_map<int, unordered_set<string>>::iterator it_b = b.begin();
    for(it = a.begin(); it != a.end(); it ++, it_b ++)
    {
        unordered_set<string> temp_a = (it->second);
        unordered_set<string> temp_b = (it_b->second);
        if( !unordered_setEqual(temp_a, temp_b) )
            return false;
    }
    return true;
}
// flatten classified minterms to set of minterms 
unordered_set<string> cm_to_unordered_set(unordered_map<int, unordered_set<string>> cm)
{
    unordered_set<string> ret;
    for(auto group: cm) {
        for(auto i: group.second) 
            ret.insert(i);
    }
    return ret;
}
// classified QM methods end ===============================================================

}; // QMclass end

// CC class (column covering & finding essential implicant & cover the rest minterms)
class CC
{
public:
    unordered_set<string> minterms;
    unordered_set<string> implicants;
    unordered_set<string> essential;
    
    unordered_map<string, vector<string> > mp1; // minterm-implicants
    map<string, vector<string> > mp2; // implicant-minterms // order DOES matter here

CC(unordered_set<string> m, unordered_set<string> imp)
{
    minterms = m;
    implicants = imp;
}
// function to check whether a implicant covers a minterms
bool imp_covers_min(string min, string imp)
{
    int n = imp.size();
    for(int i = 0; i < n; i ++) {
        if( (min[i] != imp[i]) && (imp[i] != '-') ) 
            return false;
    }
    return true;
}
// after taking 'implicant', update the prime implicant chart
void update_table(string implicant)
{
    // kill imp's minterms (delete col)
    vector<string> todo(mp2[implicant]);
    for(auto minterms: todo) {
        for(auto imp: mp1[minterms]) {
            // imp that has this minterms, its minterm recorder should be updated
            vector<string>::iterator it = find(mp2[imp].begin(), mp2[imp].end(), minterms);
            if(it != mp2[imp].end()){
                mp2[imp].erase(it);
            }
        }
        mp1.erase(minterms); // delete col
    }
    // kill this implicant (delete row)
    mp2.erase(implicant);

    // TODO: if any implicant cover 0 minterms -> erase this implicant
    // for(auto i: mp2) {
    //     if((int)i.second.size() == 0) mp2.erase(i.first);
    // }

    return;
}
// function to perform column covering
unordered_set<string> check_column_cover()
{
    // build two unordered_map for the prime implicant table
    // mp1: the col of chart, minterm-implicants
    // mp2: the row of the chart, implicants-minterms

    // find ESSENTIAL ======================================================
    for(auto i: minterms) {
        for(auto j: implicants) {
            if(imp_covers_min(i, j)) {
                mp1[i].push_back(j);
                mp2[j].push_back(i);
            }
        }
        if(mp1[i].size() == 1)
            essential.insert(mp1[i][0]); // this minterm only has this implicant(mp1[i][0])
    }

    // remove essential from table
    for(auto i: essential) {
        update_table(i);
    }
    
    if(!mp1.size()) return essential;

    // after ESSENTIAL , pick up the remaining =============================
    unordered_map<string, vector<string> >::iterator tmp;
    // until all minterms are gone
    while(!mp1.empty()) {
        // get implicant that covers the most minterms
        int max = -1;
        string imp;
        for(auto i: mp2) {
            if((int)i.second.size() > max) {
                max = i.second.size(); // max = #implicant
                imp = i.first;
            }
        }

        // take this implicant and update table
        essential.insert(imp);
        update_table(imp);
    }
    
    return essential;
}
}; // CC end

// main function
int main (int argc, char* argv[])
{
    // string path(argv[1]);
    // ifstream cin("./testcases/" + path); // depend on execution file relative path
    ifstream cin(argv[1]);
    ofstream cout(argv[2]);

    int var, pro;
    cin >> var >> pro;
    QM q(var, pro);

    // get inputs
    string temp;
    unordered_set<string> minterms; // minterms that are going to be reduced
    unordered_map<int, unordered_set<string> > c_minterms; // classify-minterms
    
    if(var >= 25) expand_flag = false;
    // expand input or not
    if(expand_flag) {
        while(cin >> temp)
        {   
            unordered_set<string> st;
            st = q.expand(temp);
            for(auto i: st) minterms.insert(i);
        }
    }
    else {
        while(cin >> temp)
        {
            minterms.insert(temp);
        }
    }
    unordered_set<string> minterms_copy(minterms); // non reduced minterms

    // classify
    c_minterms = q.classifier(minterms);

    // reduce by iteration until can't be reduced anymore
    if(!classify) {
        do
        {
            minterms=q.reduce(minterms);
        // }while(!q.unordered_setEqual(minterms, q.reduce(minterms)) );
        }while(!(minterms == q.reduce(minterms)) );
    }
    else {
        do
        {
            c_minterms = q.classify_reduce(c_minterms);
        // } while ( !q.unordered_mapEqual( c_minterms, q.classify_reduce(c_minterms) ) );
        } while ( !( c_minterms == q.classify_reduce(c_minterms) ) );
        minterms = q.cm_to_unordered_set(c_minterms);
    }

    // column covering
    CC table(minterms_copy, minterms);
    unordered_set<string> essential;
    essential = table.check_column_cover();
    cout << q.literal_count(essential) << endl << essential.size() << endl;
    for(auto i: essential)
        cout << i << endl;
    return 0;
}