/*
 *  qnode.h
 *
 *  Holds the nodes for our priority queue.
 *
 *  Each node describes a matrix in some stage of being filled with
 *  canonical forms.
 *
 *  vector<int> matrix  The current state of the matrix. Initially filled with [1,m*n] and
 *                      step by step replaced with values [2*m*n, ...] each representing
 *                      one canonical form.
 *
 *  int trans           The number of transversals possible in the current matrix
 *
 *  vector<int> starts  The canonical form in which the matrix was started (will be used to
 *                      eliminate duplicate matrices/qnodes.)
 *
 *  int m, n            represent the number of rows and columns respectively.
 *
 */
#ifndef _QNODE_H
#define _QNODE_H

#include <boost/functional/hash.hpp>
#include "utils.h"
#include "cform.h"
#include <cmath>

using namespace std;

class qnode
{
    vector<int> matrix;
    vector<int> starts;
    bool trans;
	int weight;
    int m;
    int n;

    public:

    qnode()
    {
        trans = true;
		weight = 0;
        m = 0;
        n = 0;
    }

    qnode(string line, vector<int> s, int mm, int nn)
    {
        starts = s;
        m = mm;
        n = nn;

        for(int i=0; i<m*n && i<line.size(); i++)
        {
            if(line[i] != '0')
            {
                matrix.push_back(2*m*n);
            }
            else
            {
                matrix.push_back(i+1);
            }
        }

        //trans = trans_count(matrix, m, n);
        trans = trans_check(matrix, m, n);
		weight = calc_weight();
    }

    qnode(vector<int> mat, vector<int> s, int mm, int nn)
    {
        matrix = mat;
        starts = s;
        m = mm;
        n = nn;
        trans = trans_check(matrix, m, n);
		weight = calc_weight();
    }

	/*
    qnode inert(cform cf, vector<int> rows, vector<int> cols, bool &valid)
    {
        vector<int> new_mat = matrix;
        vector<int> cf_mat = cf.get_matrix();
        valid = true;

        for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(cf_mat[ (rows[i]-1)*n + (cols[j]-1) ] != 0)
                {
                    if(matrix[i*n + j] <= m*n)
                        new_mat[i*n + j] = next_val;
                    else
                        valid = false;
                }
            }
        }

        return qnode(new_mat, start, m, n, next_val+1, next_freq+1);
    }
	*/

	//	Attempts to insert a canonical form into the matrix.

    qnode insert(cform cf, bool &valid, int val, bool last=false)
    {
        vector<int> new_mat = matrix;
        //vector<int> cf_mat = cf.get_matrix();
		vector<int> cf_pos = cf.get_pos();
        valid = true;

        //Attempt to insert the passed canonical form
		
		/*		OLD VERSION
        for(int i=0; i<m*n; i++)
        {
            if(cf_mat[i] != 0)
            {
                if(matrix[i] <= m*n)
                    new_mat[i] = val;
                else
                {
                    valid = false;
                    continue;
                }
            }
        }
		*/

		for(int i=0; i<cf_pos.size(); i++)
		{
			if(matrix[ cf_pos[i] ] <= m*n)
				new_mat[ cf_pos[i] ] = val;
			else
				valid = false;
		}

        //Prune any matrices with empty rows or columns that are after
        //the first row/column and before the last row/column
        if(valid)
        {
            valid = (!empty_cols(new_mat) && !empty_rows(new_mat));
        }

        //If the next insertion is the last we can replace those as well
        //instead of doing all of those steps again!
        if(valid && last)
        {
            for(int i=0; i<m*n; i++)
            {
                if(new_mat[i] <= m*n)
                {
                    new_mat[i] = val+1;
                }
            }
            //return qnode(new_mat, starts, m, n);
        }

        return qnode(new_mat, starts, m, n);
    }

    bool has_trans() const
    {
        return trans;
    }

    int get_trans() const
    {
        cout << "get_trans() is no longer used" << endl;
        return -1;
    }

    void update_trans()
    {
        //trans = trans_count(matrix, m, n);
        trans = trans_check(matrix, m, n);
    }

    //  Tests if there is any overlap between a qnode and cform insertion.
    //  This is useful for eliminating insertions that can never happen.
    
    bool overlap_check(cform cf)
    {
        vector<int> cpos = cf.get_pos();

        for(int i=0; i<cpos.size(); i++)
        {
            if(matrix[cpos[i]] > m*n) return true;
        }

        return false;
    }

	//	We want our matrices to be processed in a top-left heavy
	//	first kind of way. The more values they have closer to
	//	the top left the weightier they are.

	int calc_weight()
	{
		int result = 0;

		for(int i=0; i<m; i++)
		{
			for(int j=0; j<n; j++)
			{
				if(matrix[i*n + j] > m*n)
					result += (m-i)*i + (n-j)*j;
			}
		}

		return result;
	}

    void inc_skip(const int i)
    {
        starts[i]++;
    }

    int get_skip(const int i) const
    {
        return starts[i];
    }

	int get_weight() const
	{
		return weight;
	}

    //Prints the matrix in a manner easily read by the human eye
    void print_clean() const
    {
        for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(matrix[i*n + j] <= m*n)
                    printf("%c%c ", 'X', 'X');//cout << matrix[i*n + j] << " ";
                else
                    printf("%02d ", matrix[i*n + j] - 2*m*n + 1); //cout << (matrix[i*n + j]-2*m*n + 1) << " ";
            }
            cout << endl;
        }
    }

    //Prints the matrix as above but commented for ignoring
    void print_clean_comment() const
    {
        for(int i=0; i<m; i++)
        {
            cout << "!";
            for(int j=0; j<n; j++)
            {
                if(matrix[i*n + j] <= m*n)
                    printf("%c%c ", 'X', 'X');//cout << matrix[i*n + j] << " ";
                else
                    printf("%02d ", matrix[i*n + j] - 2*m*n + 1); //cout << (matrix[i*n + j]-2*m*n + 1) << " ";
            }
            cout << endl;
        }
    }

    //Prints the raw data from the matrix
    void print_matrix() const
    {
        for(int i=0; i<matrix.size(); i++)
            printf("%02d ", matrix[i]);
        printf("\n");
    }

    string string_g6(const int round) const
    {
        int g = m*n+round;            // Size of the graph we're making
        vector<int> adj(g*g);       //Adjacency matrix
        vector< vector<int> > vals(m*n);    
        string result = "";

        //Create the (top half) of the adjacency matrix:
        //First the simple junk
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

        //Populate it with the actual values
        for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
            {
                if( matrix[i*m + j] > m*n ) adj[ (m*n + matrix[i*m+j] - 2*m*n) + (i*m + j)*g]++;
            }
        }

        //Calculate the g6
        result += (char)(g+G6OFF);    //R(x) = g + 63

        int count = 1;
        unsigned char out;

        for(int i=1; i<g; i++)
        {
            for(int j=0; j<i; j++)
            {
                out |= adj[j*g + i];
                if(count>0 && count%6 == 0)
                {
                    result += (char)(out+G6OFF);
                    if(out+G6OFF > 126 || out+G6OFF < 63)
                        fprintf(stderr, "ERROR: INVALID G6 CHAR: %d %c\n", (int)(out+G6OFF), (char)(out+G6OFF));
                    out &= 0;
                }
                else
                {
                    out = out << 1;
                }

                count++;
            }
        }

        if(count%6 != 1)
        {
            out = out << (6 - (count%6));
            result += (char)(out+G6OFF);
            if(out+G6OFF > 126 || out+G6OFF < 63)
                fprintf(stderr, "ERROR: INVALID G6 CHAR: %d %c\n", (int)(out+G6OFF), (char)(out+G6OFF));
        }

        result += "\n";

        return result;
    }

    string string_dread(const string initial, const int val) const
    {
        string output;
        vector< vector<int> > vals;
        vals.resize(m*n);
        char buff[100];
        int count = 0;

        for(int i=0; i<m*n; i++)
        {
            if(matrix[i]-2*m*n+1 > count) count = matrix[i]-2*m*n+1;
            if(matrix[i] > m*n)
            {
                (vals.at( matrix[i] - 2*m*n + 1)).push_back(i);
            }
        }

        sprintf(buff, "\nn=%d g\n", (m*n + count));
        output = buff;
        output += initial;

        for(int i=1; i<=count; i++)
        {
            //cout << (m*n + i - 1) << ": ";
            sprintf(buff, "%d: ", (m*n + i - 1));
            output += buff;

            for(int j=0; j< vals[i].size(); j++)
            {
                //output += (vals.at(i)).at(j);
                //output += " ";
                sprintf(buff, "%d ", (vals.at(i)).at(j));
                output += buff;
            }
            output += ";\n";
        }

        return output;
    }

	//Prints the matrix in the dreadnaut format used by nauty
    void print_dread(const string initial, const int val) const
    {
        vector< vector<int> > vals;
        vals.resize(m*n);
        int count = 0;

        for(int i=0; i<m*n; i++)
        {
            if(matrix[i]-2*m*n+1 > count) count = matrix[i]-2*m*n+1;
            if(matrix[i] > m*n)
            {
                (vals.at( matrix[i] - 2*m*n + 1)).push_back(i);
            }
            else
            {
                //The matrix contains un-occupied spaces
                return;
            }
        }

        cout << "n=" << (m*n + count) << "g !test" << val << "\n";
        cout << initial;
        for(int i=1; i<=count; i++)
        {
            cout << (m*n + i - 1) << ": ";
            for(int j=0; j< vals[i].size(); j++)
            {
                cout << (vals.at(i)).at(j) << " ";
            }
            cout << ";\n";
        }
    }   

    //Gets matrix
    vector<int> get_matrix() const
    {
        vector<int> mat;

        for(int i=0; i<m*n; i++)
        {
            if(matrix[i] <= m*n) mat.push_back(matrix[i]);
            else mat.push_back(matrix[i] - 2*m*n + 1);
        }
            
        return mat;
    }

    bool is_equal(const qnode other) const
    {
        vector<int> omat = other.get_hash();
		vector<int> tmat = this->get_hash();
        bool result = true;

        if(omat.size() != matrix.size() || m != other.m || n != other.n)
        {
            result = false;
        }

        for(int i=0; i<m*n && result; i++)
        {
            if(omat[i] != tmat[i])
            {
                result = false;
            }
        }

        return result;
    }

	//	Creates a vector representation of the matrix suitable for hashing
	//	Specifically, it re-arranges the matrix values depending on where
	//	they first appear in the matrix and not when they were added. This
	//	removes many duplicate matrices.

    vector<int> get_hash() const
    {
        int next = 1;
        vector<int> res(m*n);
        vector<int> vals(m*n*3);

        for(int i=0; i<m*n; i++)
        {
            if( matrix[i] > m*n )
            {
                if( vals[matrix[i]] == 0 )
                {
                    vals[matrix[i]] = next++;
                }
                res[i] = vals[matrix[i]];
            }
        }

        return res;
    }

	//	Empty rows checks if any rows after the first and before the last.
	//	We don't want these cases because they are equivalent to the case
	//	where all of the empty rows are at the bottom of the matrix and we
	//	attempt to maintain a top-left heavy matrix whenever possible.

    bool empty_rows(const vector<int> mat) const
    {
        int empt = m*n;
        int full = -1;
        for(int i=1; i<m-1; i++)
        {
            bool empty = true;
            for(int j=0; j<n; j++)
            {
                if(mat[i*n + j] > m*n)
                {
                    empty = false;
                }
            }
            if(empty) empt = i;
            else full = i;
        }

        return empt < full;
    }

	//	Same as empty_rows but for columns instead!

    bool empty_cols(const vector<int> mat) const
    {
        int empt = m*n;
        int full = -1;
        for(int j=1; j<n-1; j++)
        {
            bool empty = true;
            for(int i=0; i<m; i++)
            {
                if(mat[i*n + j] > m*n)
                {
                    empty = false;
                }
            }
            if(empty) empt = j;
            else full = j;
        }

        return empt < full;
    }
};
typedef class qnode qnode;

//Less than and greater than functions used by priority_queue and/or sort
bool operator< (const qnode& lhs, const qnode &rhs)
{
	if(lhs.get_trans() == rhs.get_trans())
		return lhs.get_weight() > rhs.get_weight();
	else
		return lhs.get_trans() > rhs.get_trans();
}

bool operator> (const qnode& lhs, const qnode &rhs)
{
	if(lhs.get_trans() == rhs.get_trans())
		return lhs.get_weight() < rhs.get_weight();
	else
		return lhs.get_trans() < rhs.get_trans();
}

bool operator== (const qnode& lhs, const qnode &rhs)
{
    return lhs.is_equal(rhs);
}
/*
//Hashing equality function
struct node_equal_to
    : binary_function<qnode, qnode, bool>
{
    bool operator()(qnode const& x, qnode const& y) const
    {
        return x.is_equal(y);
    }
};

//Hashing hashing function
struct node_hash
    : unary_function<qnode, size_t>
{
    size_t operator()(qnode const& x) const
    {
        return x.get_hash();
    }
};
*/
//Hashing function
size_t hash_value(qnode const& q)
{
    boost::hash< vector<int> > hasher;

    return hasher(q.get_hash());
}

#endif
