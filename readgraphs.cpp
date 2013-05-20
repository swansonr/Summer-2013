//Take in the output from listg -o1 -a and prints them out in regular matrix
//notation. Since our graphs were generated with a single node representing
//a value of 1 we only need to look at the last line to find out what the matrix
//looks like!
//
//We then count how many traversals each of these matrices kills and the kill ratio.

#include <iostream>
#include <vector>

#define NOP 999

using namespace std;

unsigned long factorial(unsigned long f)
{
    if ( f == 0 ) 
        return 1;
    return(f * factorial(f - 1));
}

void printmatrix(const std::string line, const int m, const int n)
{
    for(int i=0; i<m*n && line.size() >= m*n; i++)
    {
        std::cout << line[i] << " ";
        if((i+1)%n == 0) std::cout << std::endl;
    }
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
    int m, n, total;
    std::string line;
    vector<int> matrix;
    vector<int> vec;

    std::cin >> m >> n;
    std::cout << m << " " << n << std::endl;
    
    //Since we're comparing the matrices we read to a matrix with m*n different values
    //the comparable matrix will have m! transversals.
    total = factorial(m);
    
    for(int i=0; i<m*n; i++)
    {
        matrix.push_back(i);
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

    int lc = 1;
    int cancount = 0;
    while(std::getline(std::cin, line))
    {
        if(lc != 0 && lc%(m*n+4) == 0) //line.compare("") == 0 && last.size() > 0)
        {
            cout << ++cancount << ":" << endl;
            printmatrix(line, m, n);
 
            //Count the number of possible transversals
            vector<int> counter = matrix;       //Matrix with m*n different values
            vector<int> perm = vec;            //Copy the initial permutation vector
            
            //make counter have an unreasonably large value where the 1's are in the
            //matrix we're getting.
            for(int i=0; i<line.size(); i++)    
            {
                if(line[i] == '1')
                {
                    counter[i] = (m*n+1);
                }
            }

            int tcount = 0;
            //Count traversals
            do
            {
                if(is_transversal(perm, counter, m, n))
                {
                    tcount++;
                }
            } 
            while(next_permutation(perm.begin(), perm.end()));

            cout << "Killed: " << total - tcount << "/" << total << " (";
            cout << ((double)(total-tcount))/total << ")" << endl << endl;
            
        }
        lc++;
    }

    return 0;   
}
