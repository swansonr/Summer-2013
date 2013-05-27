/*
 *
 *  Collection of handy stuff from /utils
 *
 */

#ifndef _UTILS_H_
#define _UTILS_H_

using namespace std;

#define NOP 999 

//Count traversals
int trans_count(const vector<int> matrix, const int m, const int n)
{
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

    int count = 0;
    do
    {
        vector<int> check;
        for( int i = 0; i < vec.size(); i++ )
        {
            //Ignore columns with NOP values
            if(vec[i] != NOP)
            {
                check.push_back(matrix[vec[i]*n + i]);
            }
        }

        //Sort the values from 'a'
        sort(check.begin(), check.end());
        
        //If any of the values are the same it isn't a transversal
        bool same = false;
        for( int i = 0; i < check.size()-1 && !same; i++ )
        {
            if( check[i] == check[i+1] ) same = true;
        }
        
        if(!same) count++;   
    } 
    while(next_permutation(vec.begin(), vec.end()));
    
    return count;
}

//Simple factorial function
unsigned long factorial(unsigned long f)
{
    if ( f == 0 ) 
        return 1;
    return(f * factorial(f - 1));
}

#endif
