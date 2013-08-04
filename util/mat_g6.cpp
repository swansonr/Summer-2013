#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    bool debug = false;
    bool good = true;

    vector<int> matrix;
    int m = 5, n = 5;
    int g = m*n+3;
    
    matrix.push_back(50);
    matrix.push_back(50);
    matrix.push_back(3);
    matrix.push_back(4);
    matrix.push_back(5);
    matrix.push_back(50);
    matrix.push_back(50);
    matrix.push_back(8);
    matrix.push_back(51);
    matrix.push_back(51);
    matrix.push_back(52);
    matrix.push_back(52);
    matrix.push_back(51);
    matrix.push_back(14);
    matrix.push_back(15);
    matrix.push_back(16);
    matrix.push_back(17);
    matrix.push_back(18);
    matrix.push_back(19);
    matrix.push_back(52);
    matrix.push_back(21);
    matrix.push_back(22);
    matrix.push_back(51);
    matrix.push_back(24);
    matrix.push_back(52);


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
            if(debug) cout << adj[j*g + i];
            out |= adj[j*g + i];
            if(count>0 && count%6==0)
            {
                if(debug) cout << " (";
                if(good) cout << (char)(out+63);
                if(debug) cout << ") ";
                out &= 0;
            }
            else 
                out = out << 1;

            count++;
        }
    }

    if(count%6 != 1)
    {
        out = out << (6 - (count%6));
        if(good) cout << (char)(out+63);
    }

    cout << ")\n";
    cout << endl;

    return 0;
}

