//Given a matrix of size m by n find the cannonical form
//of each possible combination of f 1's.

// Possible command to find a short list of only canonical forms
//./gencanon | ../nauty/dretog | ../nauty/shortg -k -v | ../nauty/listg -o1 -a > output.txt

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

//Initial matrix in dreadnaut form that all permutations will start with. Does not contain
//any information of 1's or 0's
string get_initial(const int m, const int n)
{
    string matrix = "";
    stringstream ss;
    
    for(int i=0; i<m*n; i++)
    {
        ss << i;
        matrix += ss.str();
        matrix += ":";
        for(int j=(i%n) + 1; j<n; j++)
        {
            ss.str(std::string());  //Clear stringstream
            ss << ((i/n)*n + j);
            matrix += " ";
            matrix += ss.str();// ((i/n)*n + j);
        }
        for(int j=(i/n)*n + (i%n) + n; j<m*n; j += n)
        {
            ss.str(std::string());  //Clear stringstream
            ss << j;
            matrix += " ";
            matrix += ss.str();// j;
        }

        matrix += ";\n";
        ss.str(std::string());
    }

    ss << (m*n);
    matrix += ss.str();
    matrix += ": ";

    return matrix;
}

//Given a matrix print the final two nodes corresponding to the 1's and 0's in dreadnaut form
void print_dred(const vector<int> vect, const int m, const int n)
{
    vector<int> ones;
    vector<int> zeros;

    //Find locations of 1's and 0's
    for(int i=0; i<m*n; i++)
    {
        if(vect[i] == 2)
            ones.push_back(i);
        else
            zeros.push_back(i);
    }

    //Print 0 node first
    for( vector<int>::iterator i = ones.begin(); i != ones.end(); i++ )
    {
        cout << *i << " ";
    }
    cout << ";" << endl << (m*n + 1) << ": ";

    //Print 1 node last
    for( vector<int>::iterator i = zeros.begin(); i != zeros.end(); i++ )
    {
        cout << *i << " ";
    }
    cout << ";" << endl;
}

int main(int argc, char **argv)
{
    vector<int> vec;
    int m, n, f;

    //Read size of matrix
    //cout << "Enter m n f:" << endl;       //Removed since we'll be piping everything
    cin >> m >> n >> f;

    //Value verification
    if(m > n)
    {
        cout << "ERROR: m must be <= n" << endl;
        return -1;
    }
    else if(m > 10 || m <=0 || n > 15 || n<=0)
    {
        cout << "Error: m must be smaller than 10 and n smaller than 15 land both larger than 0" << endl;
        return -2;
    }

    //Fill Matrix (Fill with f 1's and (m*n-f) of any other value
    for(int i=0; i<f; i++)
    {
        vec.push_back(1);
    }
    for(int i=f; i<m*n; i++)
    {
        vec.push_back(2);    
    }

    //All of the permutations will have the same initial matrix so we start with that
    string initial = get_initial(m, n);

    //Print out all matrices in dreadnaut notation
    int count = 0;
    do
    {
        cout << "n=" << (m*n + 2) << "g !test" << count << "\n";
        cout << initial;
        print_dred(vec, m, n);
        count++;
    } 
    while(next_permutation(vec.begin(), vec.end()));

    return 0;
}

