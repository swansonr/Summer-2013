/*
 *
 *  The first attempt at a program to find transversal counterexamples in rectangles
 *
 *
 *  Outline:
 *
 *  1)  Read in canonical forms
 *          i) Read in all frequency files.
 *          ii) Order them by highest kill count
 *  2)  Begin placing canonical forms by frequency and then by kill count into heap/queue
 *      and expand them.
 *  3)  At each expansion we add additional canon shapes to the matrix, calculate the new
 *      amount of transversals in those matrices and place them back into the heap/queue.
 *  4)  Loop expanding the matrices until a counter example has been found or we run out of nodes.
 *
 *  Current Plan:
 *
 *      Branch and bound. Priority queue/heap holds nodes with matrices and states sorted by the
 *      least amount of transversals left. First nodes are the initial canon layouts. When a node
 *      is selected we expand it by adding all configurations of the next available frequencies 
 *      canon form. We also track the initial canon form used so we only use forms of its kill
 *      count or less.
 *
 *
 *  Input File Format:
 *
 *      n m fmax
 *      f1f2f3f4...fn
 *      canon_list_fmax.file
 *      canon_list_fmax-1.file
 *      ...
 *      canon_list_1.file
 *
 *
 *  TODO:
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "qnode.h"
#include "cform.h"

using namespace std;

int main(int argc, char **argv)
{
    bool verbose = true;
    int m, n, f;
    string fline, freqs;
    vector< vector<cform> > forms;

    priority_queue< qnode, vector<qnode>, less<vector<qnode>::value_type> > nqueue;
    
    cin >> m >> n >> f;
    forms.resize(f);
    if(verbose) cout << "m n f: " << m << " " << n << " " << f << endl;

    getline(cin, freqs);
    getline(cin, freqs);
    if(verbose) cout << "Freqs: " << freqs << endl;
    
    int curr = f-1;

    while(getline(cin, fline) && curr >= 0)
    {
        if(verbose) cout << "Opening File: " << fline << endl;
        ifstream fhandle;
        string sline;
        vector<cform> vec;

        fhandle.open(fline.c_str(), ifstream::in);
        if(fhandle.is_open())
        {
            int lc=1;
            int count=0;
            while(getline(fhandle, sline))
            {
                if(lc%(m*n+4) == 0)
                {
                    //Output matrix
                    if(verbose) cout << sline << endl;
                    nqueue.push( qnode(sline, count++, m, n) );
                    vec.push_back( cform(sline, m, n) );
                }
                lc++;
            }
            sort(vec.begin(), vec.end());
            forms[curr] = vec;
        }
        else
        {
            cout << "Could not open file: " << fline << endl;
            //return 1;
        }
    }

    //TODO: Literally Everything


    if(verbose) cout << "Queue Size: " << nqueue.size() << endl;
    while( !nqueue.empty() )
    {
        cout << nqueue.top().get_trans() << endl;
        nqueue.pop();
    }

    return 0;
}
