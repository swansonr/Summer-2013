#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    vector<int> matrix;
    int m = 4, n = 4;
    int g = m*n+3;
    
    matrix.push_back(2*m*n);
    matrix.push_back(2*m*n);
    matrix.push_back(2*m*n);
    matrix.push_back(2*m*n+2);
    matrix.push_back(2*m*n);
    matrix.push_back(1);
    matrix.push_back(2);
    matrix.push_back(2*m*n+1);
    matrix.push_back(2*m*n+2);
    matrix.push_back(2*m*n+2);
    matrix.push_back(2*m*n+2);
    matrix.push_back(3);
    matrix.push_back(2*m*n+1);
    matrix.push_back(2*m*n+1);
    matrix.push_back(2*m*n+1);
    matrix.push_back(4);

    cout << "Original Matrix:" << endl;
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            cout << matrix[i*m + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
     
    vector<int> adj(g*g);;
    
    for(int i=0; i<m*n; i++)
    {
        for(int j=(i%n)+1; j<n; j++)
        {
            adj[ ((i/n)*n + j) + i*g ]++;
        }
        for(int j=(i/n)*n + (i%n) + n; j<m*n; j+=n)
        {
            adj[ j + i*g ]++;
        }
    }

    vector< vector<int> > vals(m*n);

    for(int i=0; i<m; i++)
    for(int j=0; j<n; j++)
    {
        if( matrix[i*m + j] > m*n ) adj[ (m*n + matrix[i*m + j] - 2*m*n) + (i*m + j)*g ]++;
    }

    cout << "Adjacency Matrix: (Top Half Only)" << endl;
    for(int i=0; i<g; i++)
    {
        for(int j=0; j<g; j++)
        {
            cout << adj[i*g + j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    //printf("N(%d) = %c\n", g, g+63);
    cout << "graph6 Format: " << endl;
    cout << "N(" << g << ") = " << (char)(g+63) << endl;
    cout << "R(x) = (";

    int count = 1;
    unsigned char out;
    for(int i=1; i<g; i++)
    {
        for(int j=0; j<i; j++)
        {
            //cout << adj[j*g + i];
            out |= adj[j*g + i];
            if(count>0 && count%6==0)
            {
                cout << (char)(out+63);
                out &= 0;
            }
            else 
                out = out << 1;

            count++;
        }
    }

    if(count%6 != 0)
    {
        out = out << (6 - (count%6));
        cout << (char)(out+63);
    }

    cout << ")\n";
    cout << endl;

    return 0;
}

