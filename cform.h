/*
 *      cform.h
 *
 *      This class is for holding on to canonical forms that we'll be using.
 *
 *      vecotr<int> matrix      The canonical form represented by a matrix of ints.
 *
 *      int kills               The number of transversals this form will kill.
 */

#ifndef _CFORM_H_
#define _CFORM_H_

#include "utils.h"

using namespace std;

class cform
{
    vector<int> matrix;
	vector<int> pos;
    int kills;

    public:

    cform()
    {
        kills = 0;
    }

    cform(vector<int> m, int k=0)
    {
        //matrix = m;
        pos = m;
        kills = k;
    }

    cform(string s, int m, int n, bool transpose=false)
    {
        vector<int> tempm;                  //used exclusively for kill-counting
        for(int i=0; i<s.size(); i++)
        {
            if(s[i] == '0')
            {
                matrix.push_back(0);
                tempm.push_back(i);
            }
            else
            {
				pos.push_back(i);
                matrix.push_back(1);
                tempm.push_back(m*n*2);
            }
        }

        //If we're inserting the transpose of the canonical form, transpose it
		/*

			No longer needed! Transpose and permutations of rows and columns are generated beforehand!

        if(transpose)
        {
            vector<int> tempt = tempm;
            vector<int> tmatrix = matrix;

            for(int i=0; i<m; i++)
            {
                for(int j=0; j<n; j++)
                {
                    matrix[i*n + j] = tmatrix[j*n + i];
                    tempm[i*n + j] = tempt[j*n + i];
                }
            }
        }
		*/

        //We know that an mxn matrix has m! transversals so we can
        //easily compute how many this form will kill
        kills = factorial(m) - trans_count(tempm, m, n);
    }

    int get_kills() const
    {
        return kills;
    }

    //Prints the raw data of the canonical form 
    void print() const
    {
        for(int i=0; i<matrix.size(); i++)
            cout << matrix[i];
        cout << "\t(" << kills << ")" << endl;
    }

    //Prints the canonical form in a manner that is easy to read
    void print_clean(const int m, const int n)
    {
        for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
            {
                cout << matrix[i*n + j] << " ";
            }
            cout << endl;
        }
    }

    vector<int> get_matrix()
    {
        return matrix;
    }

	vector<int> get_pos()
	{
		return pos;
	}
};

//Less than and greater than functions used by priority_queue and/or sort
bool operator< (const cform& lhs, const cform &rhs)
{
    return lhs.get_kills() > rhs.get_kills();
}

bool operator> (const cform& lhs, const cform &rhs)
{
    return lhs.get_kills() < rhs.get_kills();
}

#endif
