//List the frequency combinations for a given n,m, and f-max

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print_vec(vector<int> vect)
{
    for( vector<int>::iterator i = vect.begin(); i != vect.end(); i++ )
    {
        cout << *i << " ";
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    int m,n,f;

    cin >> m >> n >> f;

    printf("Size: %dx%d\tFmax: %d\n", m, n, f);

    int total = 0;
    vector<int> initial;

    //Created initial list of frequencies
    for(int i=f; i>0 && total != m*n; i--)
    {
        while(total+i <= m*n)
        {
            initial.push_back(i);
            total += i;
        }
    }

    cout << "Initial Frequencies: (" << total << ")" << endl;
    print_vec(initial);

    //Print out all variations
    while(initial.size() > 0 && true)
    {
        int i=initial.size()-1;
        while(i!=0 && initial.at(i) == 1)
        {
            i--;
        }

        if(i!=0 && initial.at(i) != 1)
        {
            initial.at(i) = initial.at(i)-1;
            initial.push_back(1);
        }
        else if(i==0)
        {
            break;
        }

        print_vec(initial);
    }

    return 0;
}
