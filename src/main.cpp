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

#include <fstream>

using namespace std;

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

//function to check if two terms differ by just one bit
bool isGreyCode(string a,string b)
{
   int flag=0;
   for(int i=0;i<a.length();i++)
   {
       if(a[i]!=b[i])
        flag++;
   }
   return (flag==1);
}

//function to replace complement terms with don't cares
// Eg: 0110 and 0111 becomes 011-
string replace_complements(string a,string b)
{
   string temp="";
   for(int i=0;i<a.length();i++)
   if(a[i]!=b[i])
       temp=temp+"-";
   else
       temp=temp+a[i];

   return temp;
}

//function to check if string b exists in vector a
bool in_vector(vector<string> a,string b)
{
   for(int i=0;i<a.size();i++)
     if(a[i].compare(b)==0)
      return true;
    return false;
}

//function to reduce minterms
vector<string> reduce(vector<string> minterms)
{

      vector<string> newminterms;

      int max=minterms.size();
      int* checked = new int[max];
      for(int i=0;i<max;i++)
      {
          for(int j=i;j<max;j++)
          {
               //If a grey code pair is found, replace the differing bits with don't cares.
               if(isGreyCode(minterms[i],minterms[j]))
               {
                  checked[i]=1;
                  checked[j]=1;
                  if(!in_vector(newminterms,replace_complements(minterms[i],minterms[j])))
                     newminterms.push_back(replace_complements(minterms[i],minterms[j]));
               }
          }
      }

      //appending all reduced terms to a new vector
      for(int i=0;i<max;i++)
      {
          //cout<<checked[i]<<endl;
          if(checked[i]!=1 && ! in_vector(newminterms,minterms[i]))
           newminterms.push_back(minterms[i]);
      }

       delete[] checked;
   
       return newminterms;
}

//function to check if 2 vectors are equal
bool VectorsEqual(vector<string> a,vector<string> b)
{
   if(a.size()!=b.size())
      return false;

    sort(a.begin(),a.end());
    sort(b.begin(),b.end());
    for(int i=0;i<a.size();i++)
    {
        if(a[i]!=b[i])
        return false;
    }
    return true;
}

// count the literals
int literal_count(vector<string> minterms)
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
    ifstream cin("./testcases/" + path); // depend on compiled relative path
    ofstream cout(argv[2]);

    int var, pro;
    cin >> var >> pro;
    QM q(var);

    // get inputs
    string temp;
    vector<string> minterms;
    while(cin >> temp)
    {
        minterms.push_back(temp);
    }

    sort(minterms.begin(),minterms.end());

    do
    {
        minterms=q.reduce(minterms);
        sort(minterms.begin(),minterms.end());
    }while(!q.VectorsEqual(minterms,q.reduce(minterms)));

    cout << q.literal_count(minterms) << endl << minterms.size() << endl;
    for(auto i: minterms)
        cout << i << endl;
    return 0;
}