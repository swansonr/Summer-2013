/*
 *  qnode.h
 *
 *  Holds the nodes for our priority queue.
 *
 *  Each node describes a matrix in some stage of being filled with
 *  canonical forms.
 *
 *  vector<int> matrix  The current state of the matrix. Initially filled with [1,m*n] and
 *                      step by step replaced with values [2*m*n, ...] each representing
 *                      one canonical form.
 *
 *  int trans           The number of transversals possible in the current matrix
 *
 *  int start           The canonical form in which the matrix was started (will be used to
 *                      eliminate duplicate matrices/qnodes.)
 *
 *  int next_val        The next values from [2*m*n, ...] to be placed in the subsequent
 *                      qnode based on this one.
 *
 *  int m, n            represent the number of rows and columns respectively.
 *
 */
#ifndef _QNODE_H
#define _QNODE_H

#include "utils.h"
#include "cform.h"

using namespace std;

class qnode
{
    //int *matrix;
    vector<int> matrix;
    int trans;
    int start;
    int next_val;
    int next_freq;
    int m;
    int n;

    public:

    qnode()
    {
        trans = 0;
        start = 0;
        next_freq = 0;
        m = 0;
        n = 0;
        next_val = 0;
    }

    qnode(string line, int s, int mm, int nn, int nf=1)
    {
        start = s;
        m = mm;
        n = nn;
        next_val = 2*m*n;
        next_freq = nf;

        for(int i=0; i<m*n && i<line.size(); i++)
        {
            if(line[i] != '0')
            {
                matrix.push_back(next_val);
                //matrix.push_back(line[i]);
            }
            else
            {
                matrix.push_back(i+1);
            }
        }

        trans = trans_count(matrix, m, n);
        next_val++;
    }

    qnode(vector<int> mat, int s, int mm, int nn, int nv, int nf=1)
    {
        matrix = mat;
        start = s;
        m = mm;
        n = nn;
        next_freq = nf;
        next_val = nv;
        //printf("INSERT MATRIX: ");
        //print_matrix();
        trans = trans_count(matrix, m, n);
    }

    qnode insert(cform cf, vector<int> rows, vector<int> cols, bool &valid)
    {
        vector<int> new_mat = matrix;
        vector<int> cf_mat = cf.get_matrix();
        valid = true;

        for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(cf_mat[ (rows[i]-1)*n + (cols[j]-1) ] != 0)
                {
                    if(matrix[i*n + j] <= m*n)
                        new_mat[i*n + j] = next_val;
                    else
                        valid = false;
                }
            }
        }

        return qnode(new_mat, start, m, n, next_val+1, next_freq+1);
    }

    int get_trans() const
    {
        return trans;
    }

    void update_trans()
    {
        trans = trans_count(matrix, m, n);
    }

    int get_next_freq()
    {
        return next_freq;
    }

    //Prints the matrix in a manner easily read by the human eye
    void print_clean()
    {
        for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(matrix[i*n + j] <= m*n)
                    printf("%c%c ", 'X', 'X');//cout << matrix[i*n + j] << " ";
                else
                    printf("%02d ", matrix[i*n + j] - 2*m*n + 1); //cout << (matrix[i*n + j]-2*m*n + 1) << " ";
            }
            cout << endl;
        }
    }

    //Prints the raw data from the matrix
    void print_matrix()
    {
        for(int i=0; i<matrix.size(); i++)
            printf("%02d ", matrix[i]);
        printf("\n");
    }
};

//Less than and greater than functions used by priority_queue and/or sort
bool operator< (const qnode& lhs, const qnode &rhs)
{
    return lhs.get_trans() > rhs.get_trans();
}

bool operator> (const qnode& lhs, const qnode &rhs)
{
    return lhs.get_trans() < rhs.get_trans();
}

#endif
