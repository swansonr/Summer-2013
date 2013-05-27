#ifndef _CFORM_H_
#define _CFORM_H_

#include "utils.h"

using namespace std;

class cform
{
    vector<int> matrix;
    int kills;

    public:

    cform()
    {
        kills = 0;
    }

    cform(vector<int> m, int k)
    {
        matrix = m;
        kills = k;
    }

    cform(string s, int m, int n)
    {
        for(int i=0; i<s.size(); i++)
        {
            if(s[i] == '0')
            {
                matrix.push_back(0);
            }
            else
            {
                matrix.push_back(1);
            }
        }

        kills = factorial(m) - trans_count(matrix, m, n);
    }

    int get_kills() const
    {
        return kills;
    }
};

bool operator< (const cform& lhs, const cform &rhs)
{
    return lhs.get_kills() > rhs.get_kills();
}

bool operator> (const cform& lhs, const cform &rhs)
{
    return lhs.get_kills() < rhs.get_kills();
}

#endif
