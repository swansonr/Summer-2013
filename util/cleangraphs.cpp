//Cleans up input from gencanon.cpp

#include <iostream>
#include <vector>

#define NOP 999

using namespace std;

int main(int argc, char **argv)
{
    int m, n;
    std::string line;

    std::cin >> m >> n;
    std::cout << m << " " << n << std::endl;
    
    int lc = 1;
    while(std::getline(std::cin, line))
    {
        if(lc != 0 && lc%(m*n+4) == 0) //line.compare("") == 0 && last.size() > 0)
        {
            //Output matrix and canon number
            cout << line << endl;
        }
        lc++;
    }

    return 0;   
}
