// Count transversals given an mxn array

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define NOP 999 //Negative values don't work in permute so we have something really big

void print_vec(vector<int> vect)
{
    for( vector<int>::iterator i = vect.begin(); i != vect.end(); i++ )
    {
        cout << *i << " ";
    }
    cout << endl;
}

bool is_transversal(const vector<int> vec, const vector<int> a, const int m, const int n)
{
    vector<int> check;

    for( int i = 0; i < vec.size(); i++ )
    {
        if(vec[i] != NOP)
        {
            check.push_back(a[vec[i]*n + i]);
        }
    }

    sort(check.begin(), check.end());

    for( int i = 0; i < check.size()-1; i++ )
    {
        if( check[i] == check[i+1] ) return false;
    }
    
    return true;
}

int main(int argc, char **argv)
{
    int m;
    int n;

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

    printf("mxn\n%d %d\nThe Initial Matrix:\n", m, n);

    //Fill Initial Matrix
    vector<int> matrix;
    for(int i=0; i<m*n; i++)
    {
        int val;
        cin >> val;
        matrix.push_back(val);
        printf("%3d ", matrix[i]);
        if((i+1)%n==0 && i!=0) printf("\n");
    }

    cout << "Counting..." << endl;

    int count = 0;
    int perms = 0;
    do
    {
        int tcount = 0;
        perms++;
        vector<int> vec;

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
        do
        {
            if(is_transversal(vec, matrix, m, n))
            {
                tcount++;
            }
        } 
        while(next_permutation(vec.begin(), vec.end()));

        if(tcount > 0) count++;
    }
    while(next_permutation(matrix.begin(), matrix.end()));

    printf("Transversal Count: %d/%d\n", count, perms);
    
    return 0;
}
