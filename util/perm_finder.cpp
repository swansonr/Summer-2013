/*
 * New way of cleaning up output from gen_canon:
 *  Permutes all rows and columns of incoming matrices
 *  Inserts into list
 *  Sorts list
 *  Removes duplicates
 *  Outputs unique permutations to be used in clutch
 *
 */

#include <iostream>
#include <vector>
#include <list>

#define NOP 999

using namespace std;

int main(int argc, char **argv)
{
    int m, n;
    string line;
    list<string> uniques;
    vector<string> vec;
    vector<int> init_rows, init_cols;

    cin >> m >> n;
    //cout << m << " " << n << std::endl;
    
    int lc = 1;
    while(std::getline(std::cin, line))
    {
        if(lc != 0 && lc%(m*n+4) == 0) //line.compare("") == 0 && last.size() > 0)
        {
            //Output matrix and canon number
            //cout << line << endl;
            vec.push_back(line);
        }
        lc++;
    }

    for(int i=1; i<=m; i++)
        init_rows.push_back(i);
    for(int i=1; i<=n; i++)
        init_cols.push_back(i);

    for(int v=0; v<vec.size(); v++)
    {
        vector<int> curr_rows = init_rows;
        do
        {
            vector<int> curr_cols = init_cols;
            do
            {
                string curr = vec[v];

                for(int i=0; i<m; i++)
                {
                    for(int j=0; j<n; j++)
                    {
                        curr[i*n + j] = vec[v].at( (curr_rows[i]-1)*n + curr_cols[j]-1 );
                    }
                }
                //cout << curr << endl;
                uniques.push_back(curr);
            }
            while(next_permutation(curr_cols.begin(), curr_cols.end()));
        }
        while(next_permutation(curr_rows.begin(), curr_rows.end()));
    }

    //cout << "Total: " << uniques.size() << endl;

    uniques.sort();
    uniques.unique();

    //cout << "Unique: " << uniques.size() << endl;

    for (list<string>::iterator i=uniques.begin(); i != uniques.end(); ++i)
    {
        cout << *i << endl;
    }

    return 0;   
}
