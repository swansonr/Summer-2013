/*
 * Test file for playing around with the ways that canonical forms
 * are inserted into matrices.
 *
 */


#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    //vector<int> matrix;
    vector<int> rows, cols;
    int m=4;
    int n=4;
    //              32 32 34 04 33 33 32 32 35 35 35 12 34 34 33 16 
                //  32 32 32 04 34 35 33 32 33 34 35 12 35 33 34 16
    int matrix[] = {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0};
    //int insert[] = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //int insert[] = {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int trans[] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //int insert[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};

    //int trans[] = {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0};
    vector<int> insert;

    
    /*
    for(int i=0; i<m*n; i++)
    {
        matrix.push_back(i);
    }
    */
    for(int i=1; i<=m; i++)
    {
        rows.push_back(i);
    }
    for(int i=1; i<=n; i++)
    {
        cols.push_back(i);
    }

    cout << "Matrix: " << endl;
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            cout << matrix[i*n + j] << " ";
        }
        cout << endl;
    }

    cout << "Insert: " << endl;
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            cout << trans[i*n + j] << " ";
        }
        cout << endl;
    }

    cout << "Transpose: " << endl;
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            cout << trans[j*n + i] << " ";
            insert.push_back(trans[j*n + i]);
        }
        cout << endl;
    }

    do
    {
        vector<int> curr_cols = cols;
        do
        {
            bool good = true;
            int curr_matrix[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            for(int i=0; i<m; i++)
            {
                for(int j=0; j<n; j++)
                {
                    cout << insert[ (rows[i]-1)*n + (curr_cols[j]-1) ] <<  " ";
                    //matrix == 0 && insert != 0 <-- INSERT GOOD!
                    if(matrix[i*n + j] == 0 && insert[ (rows[i]-1)*n + (curr_cols[j]-1) ] != 0)
                    {
                        curr_matrix[i*n + j] = 1;
                    }
                    //matrix != 0 && insert != 9 <-- INSERT BAD!
                    else if(matrix[i*n + j] != 0 && insert[ (rows[i]-1)*n + (curr_cols[j]-1) ] != 0)
                    {
                        good = false;
                        continue;
                    }
                    else
                    {
                        curr_matrix[i*n + j] = matrix[i*n + j];
                    }
                }
                cout << endl;
            }
            cout << endl;
            if(good)
            {
                cout << "GOOD ";
                for(int i=0; i<m*n; i++)
                {
                    cout << curr_matrix[i] << " ";
                }
                cout << endl;
            }
            /*else
            {
                cout << "BAD  ";
                for(int i=0; i<m*n; i++)
                {
                    cout << curr_matrix[i] << " ";
                }
                cout << endl;
            }*/
        }
        while(next_permutation(curr_cols.begin(), curr_cols.end()));
    }
    while(next_permutation(rows.begin(), rows.end()));

    return 0;
}
