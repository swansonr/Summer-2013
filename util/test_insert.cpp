/*
 * Test file for playing around with the ways that canonical forms
 * are inserted into matrices.
 *
 */


#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "../qnode.h"
using namespace std;

int main(int argc, char **argv)
{
    //vector<int> matrix;
    vector<int> rows, cols;
    bool val = true;
    int m=5;
    int n=5;
    int vv[25] = {50, 50, 0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0};
    vector<int> insert(&vv[0], &vv[0]+25);
   
    for(int i=0; i<insert.size(); i++) cout << insert[i] << " ";
    cout << endl;

    qnode test = qnode(insert, 0, m, n, 51, 0); 
    cout << "INIT: " << endl;
    test.print_clean();
    cform temp;

    int vv2[4] = {2,7,13,18};
    vector<int> insert2(&vv2[0], &vv2[0]+4);
    temp = cform(insert2);
    test = test.insert(temp, val); 
    if(!val) cout << "ERROR2" << endl;
    else cout << "OK2" << endl;
    test.print_clean();

    int vv3[4] = {3,8,14,24};
    vector<int> insert3(&vv3[0], &vv3[0]+4);
    temp = cform(insert3);
    test = test.insert(temp, val); 
    if(!val) cout << "ERROR3" << endl;
    else cout << "OK3" << endl;
    test.print_clean();

    int vv4[4] = {4, 9, 17, 22};
    vector<int> insert4(&vv4[0], &vv4[0]+4);
    temp = cform(insert4);
    test = test.insert(temp, val); 
    if(!val) cout << "ERROR4" << endl;
    else cout << "OK4" << endl;
    test.print_clean();

    int vv5[3] = {10, 11, 19};
    vector<int> insert5(&vv5[0], &vv5[0]+3);
    temp = cform(insert5);
    test = test.insert(temp, val); 
    if(!val) cout << "ERROR5" << endl;
    else cout << "OK5" << endl;
    test.print_clean();

    int vv6[3] = {12, 20, 21};
    vector<int> insert6(&vv6[0], &vv6[0]+3);
    temp = cform(insert6);
    test = test.insert(temp, val); 
    if(!val) cout << "ERROR6" << endl;
    else cout << "OK6" << endl;
    test.print_clean();

    int vv7[3] = {15, 16, 23};
    vector<int> insert7(&vv7[0], &vv7[0]+3);
    temp = cform(insert7);
    test = test.insert(temp, val); 
    if(!val) cout << "ERROR7" << endl;
    else cout << "OK7" << endl;
    test.print_clean();

    return 0;
}
