//Cleans up input from gencanon.cpp

#include <iostream>
#include <vector>

#define NOP 999

using namespace std;

int main(int argc, char **argv)
{
    int m, n, f;
    string line;
    vector<string> vec;
    int count=0;

    cin >> m >> n >> f;
    //cout << m << " " << n << " " << f << endl;

    vector<int> mat;

    for(int i=1; i<=m*n; i++)
    {
        mat.push_back(0);
    }
    
    int lc = 1;
    while(getline(cin, line))
    {
        if(lc != 0 && lc%(m*n + 2 + f) == 19) //line.compare("") == 0 && last.size() > 0)
        {
            vector<int> cmat = mat;
            cout << "#" << count++ << " " << lc << endl;
            for(int i=0; i<f; i++)
            {
                //cout << lc << " " << line << endl;
                for(int s=0; s<m*n; s++)
                {
                    if(line[s] == '1') cmat[s] = i+1;
                }
                getline(cin, line);
                lc++;
            }

            for(int i=0; i<m; i++)
            {
                for(int j=0; j<n; j++)
                {
                    printf("%02d ", cmat[i*n+j]);
                    //cout << cmat[i*n+j] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
        lc++;
    }

    return 0;   
}
