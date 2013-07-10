// Count transversals given an mxn array

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define NOP 999 //Negative values don't work in permute so we have something really big

//Checks for transversals given locations in vec of an mxn matrix 'a'
bool is_transversal(const vector<int> vec, const int *a, const int m, const int n)
{
    vector<int> check;

    //Get values from 'a' as specified by vec
    for( int i = 0; i < vec.size(); i++ )
    {
        //Ignore columns with NOP values
        if(vec[i] != NOP)
        {
            check.push_back(a[vec[i]*n + i]);
        }
    }

    //Sort the values from 'a'
    sort(check.begin(), check.end());

    //If any of the values are the same it isn't a transversal
    for( int i = 0; i < check.size()-1; i++ )
    {
        if( check[i] == check[i+1] ) return false;
    }
    
    return true;
}

int main(int argc, char **argv)
{
    vector<int> vec;
    int *a;
    int m, n;

    //Read size of matrix
    cin >> m >> n;

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

    printf("mxn\n%d %d\nThe Matrix:\n", m, n);

    //Fill Matrix
    a = (int*)malloc(sizeof(int)*m*n);
    for(int i=0; i<m*n; i++)
    {
        cin >> a[i];
        printf("%3d ", a[i]);
        if((i+1)%n==0 && i!=0) printf("\n");
    }

    //Fill permutation vector
    for(int i=0; i<m; i++)
    {
        vec.push_back(i);
    }
    for(int i=m; i<n; i++)
    {
        vec.push_back(NOP);    
    }

    //Count traversals
    int count = 0;
    do
    {
        if(is_transversal(vec, a, m, n))
        {
            count++;
        }
    } 
    while(next_permutation(vec.begin(), vec.end()));

    printf("Transversal Count: %d\n", count);

    return 0;
}

