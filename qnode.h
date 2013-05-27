#ifndef _QNODE_H
#define _QNODE_H

#include "utils.h"

using namespace std;

class qnode
{
    //int *matrix;
    vector<int> matrix;
    int trans;
    int start;
    int nextval;
    int m;
    int n;

    public:

    qnode()
    {
        trans = 0;
        start = 0;
        m = 0;
        n = 0;
        nextval = 0;
    }

    qnode(string line, int s, int mm, int nn)
    {
        start = s;
        m = mm;
        n = nn;
        nextval = 2*m*n;

        for(int i=0; i<m*n && i<line.size(); i++)
        {
            if(line[i] != '0')
            {
                matrix.push_back(nextval);
                //matrix.push_back(line[i]);
            }
            else
            {
                matrix.push_back(i+1);
            }
        }

        trans = trans_count(matrix, m, n);
        nextval++;
    }

    qnode(int t, int s = 0, int *m = NULL)
    {
        trans = t;
    }

    int get_trans() const
    {
        return trans;
    }

    void update_trans()
    {
        trans = trans_count(matrix, m, n);
    }

};

bool operator< (const qnode& lhs, const qnode &rhs)
{
    return lhs.get_trans() > rhs.get_trans();
}

bool operator> (const qnode& lhs, const qnode &rhs)
{
    return lhs.get_trans() < rhs.get_trans();
}

#endif
