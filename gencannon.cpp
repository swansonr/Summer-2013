//Given a matrix of size m by n find the cannonical form
//of each possible combination of f 1's.

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print_dred(const vector<int> vect, const int m, const int n, const int count)
{
    vector<int> ones;
    vector<int> zeros;

    cout << "n=" << (m*n + 2) << "g !test" << count << endl;

    for(int i=0; i<m*n; i++)
    {
        cout << i << ":";
        for(int j=(i%n) + 1; j<n; j++)
        {
            cout << " " << (i/n)*n + j;
        }
        for(int j=(i/n)*n + (i%n) + n; j<m*n; j += n)
        {
            cout << " " << j;
        }

        cout << ";" << endl;

        if(vect[i] == 2)
            ones.push_back(i);
        else
            zeros.push_back(i);
    }

    cout << (m*n) << ": ";
    for( vector<int>::iterator i = ones.begin(); i != ones.end(); i++ )
    {
        cout << *i << " ";
    }
    cout << ";" << endl << (m*n + 1) << ": ";
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
    cin >> m >> n >> f;

    //Value verification
    if(m > n)
    {
        printf("ERROR: m must be <= n");
        return 0;
    }
    else if(m > 10 || m <=0 || n > 15 || n<=0)
    {
        printf("Error: m must be smaller than 10 and n smaller than 15 land both larger than 0.\n");
        return -1;
    }

    //Fill Matrix
    for(int i=0; i<f; i++)
    {
        vec.push_back(1);
    }
    for(int i=f; i<m*n; i++)
    {
        vec.push_back(2);    
    }

    //Print out all matrices in dreadnaut notation
    int count = 0;
    do
    {
        print_dred(vec, m, n, count);
        count++;
    } 
    while(next_permutation(vec.begin(), vec.end()));

    return 0;
}

