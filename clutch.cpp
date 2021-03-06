/*
 *  The first attempt at a program to find transversal counterexamples in rectangles
 *
 *  Procedure:
 *
 *  1)  Generate the canonical forms using util/gen_canon.cpp (instructions included in file.)
 *  2)  Create an input file as described below.
 *  3)  Compile the file (eg: clang++ clutch.cpp -O3 -oclutch)
 *  4)  Run (eg: ./clutch < input.txt)
 *
 *
 *  Input File Format:
 *
 *      N.B:    This format was created from combining previous utilities and experiments and
 *              will most likely be replaced with something better eventually.
 *
 *              Frequencies cannot exceed a single digit. (Shouldn't be a problem for the sizes
 *              we are expecting.)
 *
 *              Only attemps one configuration of frequencies, allowing for multiple instances
 *              to be run using different frequency setups as the program is not multithreaded
 *              whatsoever.
 *
 *      n m fmax                        (Number of rows, columns, and maximum frequency)
 *      f1f2f3f4...fn                   (list of frequencies starting with fmax)
 *      canon_list_fmax.file            (Path to canonical form files generated by util/gen_canon)
 *      canon_list_fmax-1.file
 *      ...
 *      canon_list_1.file
 *
 */

#include <omp.h>
#include <list>
//#include <boost/timer.hpp>
#include <boost/unordered_set.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "qnode.h"
#include "cform.h"

#ifdef DEBIAN
#define INFILE "temp_clutch_in.txt"
#define OUTFILE "temp_clutch_out.txt"
#define RESFILE "temp_clutch_results.txt"
#else
#define INFILE "/data/ugrad/umswans5/temp_clutch_in.txt"
#define OUTFILE "/data/ugrad/umswans5/temp_clutch_out.txt"
#define RESFILE "/data/ugrad/umswans5/temp_clutch_results.txt"
#endif

using namespace std;

qnode read_qnode(ifstream &readf, const vector<int> starts, const int m, const int n, bool &valid)
{
    qnode result;
    //Open INFILE
    //ifstream readf;
    //readf.open(INFILE, ios::in);
    if(readf.is_open())
    {
        int counter = 1;
        vector<int> matrix(m*n);
        int curr = 2*m*n;
        string line;

        getline(readf, line);   //Initial empty line
        while(readf.good() && line[0] != 'G')
        {
            getline(readf, line);
        }
            for(int i=0; i<m*n-1 && readf.good(); i++)
            {
                getline(readf, line);
                matrix[i] = i+1;
            }
            matrix[m*n-1] = m*n;

            getline(readf, line);
            getline(readf, line);
            while(line.compare("") != 0)
            {
                for(int i=0; i<line.size(); i++)
                {
                    if(line[i] != '0') matrix[i] = curr;
                }
                curr++;
                getline(readf, line);
            }
                    
            result = qnode(matrix, starts, m, n);
            valid = true;
            //output.push_front(temp);

            //if(counter%100000==0) cout << "!\tCount: " << counter << endl;
            //counter++;
        //}
        //readf.close();
    }
    else
    {
        fprintf(stderr, "!Error: Unable to open completed nauty files.");
        valid = false;
    }

    return result;
}

void nauty_cleanup(const vector<int> starts, const int m, const int n)
{
    pid_t pid = fork();
    if(pid==0)      //Child Again
    {
        cout << "!Nauty Step 1." << endl;
        execl("nauty/shortg", "nauty/shortg", INFILE, OUTFILE, "-k", "-v", "-T/data/ugrad/umswans5/tmp", (char *)0);
        exit(0);
    }
    else            //Parent Again
    {
        wait(NULL);

        pid = fork();
        if(pid==0)      //Last Child
        {
            cout << "!Nauty Step 2." << endl;
            execl("nauty/listg", "nauty/listg", OUTFILE, RESFILE, "-o1", "-a", (char *)0);
            exit(0);
        }
        else
        {
            wait(NULL);
        }
    }
}

int main(int argc, char **argv)
{
	//cout << "!Timer Started." << endl;
	//boost::timer::auto_cpu_timer t_dot;
    
    omp_lock_t writelock;
    //omp_lock_t hashlock;
    omp_lock_t queuelock;
    omp_lock_t outlock;

    omp_init_lock(&writelock);
    //omp_init_lock(&hashlock);
    omp_init_lock(&queuelock);
    omp_init_lock(&outlock);

    bool verbose = true;
    bool dreadly = true;
    int m, n, f, lim;
    qnode single;
    vector< vector<int> > cstart;
    string fline, freqs, initial;
    vector< vector<cform> > forms;

    //priority_queue< qnode, vector<qnode>, less<vector<qnode>::value_type> > nqueue;
    //list<qnode> nqueue;
    //boost::unordered_set< qnode, boost::hash<qnode> > nhash;
    //pair< boost::unordered_set<qnode>::iterator, bool> ihash;

    int read_count = 0;
    int curr_queue_size = 0;
    int valid_count = 0;
    int insert_count = 0;
    int counter_count = 0;
    int last_counter = 0;
    int last = 0;
    int bad_full = 0;
    int round = 1;
    
    //--------- Reading data from input files
    cin >> m >> n >> f >> lim;

    forms.resize(f);
    cstart.resize(f);

    int curr_freq = 1;
    int next_value = 2*m*n+1;

    if(verbose)
    {
        cout << "!m n f: " << m << " " << n << " " << f;
        if(lim >= 0) cout << " lim == " << lim;
        cout << endl;
    }

    getline(cin, freqs);
    getline(cin, freqs);
    if(verbose) cout << "!Freqs: " << freqs << endl;
    initial = get_initial(m,n);
    

    //-------- Reading canonical forms
    int curr = f;
    vector<int> starts(freqs[0] - '0');
    ofstream ofile;
    ofile.open(INFILE, ios::out);

    if(getline(cin, fline))
    {
        if(verbose) cout << "!Opening File: " << fline << endl;
        ifstream fhandle;
        string sline;

        fhandle.open(fline.c_str(), ifstream::in);

        if(fhandle.is_open() && ofile.is_open())
        {
            int lc=1;
            int count=0;

            while(getline(fhandle, sline))
            {
                if(lc%(m*n+4) == 0)
                {
                    if(lim < 0 || lim == count)
                    {
                        if(lim >= 0) starts[freqs[0] - '0' - 1] = lim;
                        else starts[freqs[0] - '0' - 1] = 0;

                        single = qnode(sline, starts, m, n);
                        //nqueue.push_front(single);
                        //ihash = nhash.insert(single);
                        ofile << single.string_g6(1);
                        //cout << "INSERT:" << endl;
                        //single.print_clean();
                        insert_count++;
                    }
                    count++;
                }
                lc++;
            }
        }
		else
		{
			fprintf(stderr, "Error: Unable to open file %s\n", fline.c_str());
		}

        fhandle.close();
    }

    ofile.close();

    //-------- Read canonical permutations
    int debug_count = 0;
    int debug_insert = 0;
    bool last_insert = false;

    while(getline(cin, fline) && curr >= 0)
    {
        if(verbose) cout << "!Opening File: " << fline << endl;
        ifstream fhandle;
        string sline;
        vector<cform> vec;

        fhandle.open(fline.c_str(), ifstream::in);
        if(fhandle.is_open())
        {
            while(getline(fhandle, sline))
            {
                //Then this is the newer version of input files that distinguish
                //which canonical form they came from.
                if(sline.length() < m*n)
                {
                    cstart[curr-1].push_back(debug_insert);
                }
                else
                {
                    debug_count++;
                    cform ttemp = cform(sline, m, n);
                    //ttemp.print_clean(m,n);

                    if( lim < 0 || (lim >= 0 && !single.overlap_check(ttemp)) )
                    {
                        debug_insert++;
                        vec.push_back(ttemp);
                    }
                }
            }
            sort(vec.begin(), vec.end());
            if(verbose) cout << "!Insert Forms " << curr << " " << vec.size() << endl;
            forms[--curr] = vec;
        }
        else
        {
			fprintf(stderr, "Error: Unable to open file %s\n", fline.c_str());
            //cout << "Could not open file: " << fline << endl;
            //return 1;
        }
    }

    if(verbose) cout << "!DEBUG INSERTS: " << debug_insert << "/" << debug_count << endl;
    //if(verbose) cout << "!Queue Size: " << nqueue.size() << endl;

    //Starting matrix:
    if(verbose && lim >= 0)
    {
        cout << "!Initial Insert:\n";
        //qnode single = nqueue.front();
        single.print_clean_comment();
    }
    
    nauty_cleanup(starts, m, n);

    // Start popping the queue and inserting canonical forms
    while( !last_insert )
    {
        ofstream tempf;
        ifstream tempr;
        curr_queue_size = 0;
        valid_count = 0;
        insert_count = 0;

        //Create input file
        tempf.open(INFILE, ios::trunc);
        if(!tempf.is_open())
        {
            fprintf(stderr, "!Error: Unable to open initial nauty files.");
            return 0;
        }
        tempf << ">>graph6<<";
        tempf.close();

        tempr.open(OUTFILE, ios::in);
        if(tempr.is_open())
        {
            string tline;
            while(getline(tempr, tline)) curr_queue_size++;
        }
        tempr.close();

        //Open results file for reading
        tempr.open(RESFILE, ios::in);
        if(!tempr.is_open())
        {
            fprintf(stderr, "!Error: Unable to open nauty result file.");
            return 0;
        }

        //If it's the second to last insertion we can do the trivial insertion as well.
        if(curr_freq == (freqs.size() - 2))
        {
            last_insert = true;
            last_counter++;
        }

        //Get the vector of canonical forms that have the same number of frequencies needed
        //by the current qnode
        int next_freq = freqs[curr_freq] - '0' - 1;
        vector<cform> curr_forms = forms.at(next_freq);
        int skip = 0;
        if(freqs[curr_freq] == freqs[0] && lim >= 0) skip = lim;

        read_count = 0;
        //while( !nqueue.empty() )
        #pragma omp parallel for
        for(int omp=0; omp<curr_queue_size; omp++)
        {
            bool read_result = false;
            omp_set_lock(&queuelock);
            qnode curr = read_qnode(tempr, starts, m, n, read_result);
            //nqueue.pop_front();
            //curr.print_clean();
            if(read_result) read_count++;
            omp_unset_lock(&queuelock);

            //If we're limiting values then we want to skip frequencies that we know won't work.
            //int skip = cstart[next_freq].at( curr.get_skip(next_freq) ); //((lim > 0) && (next_freq == 0)) ? cstart[lim] : 0;

            for(int i=skip; i<curr_forms.size() && read_result; i++)
            {
                //Check if the skip increases yet
                //if(cstart[next_freq].size() > curr.get_skip(next_freq)+1 && i > cstart[next_freq].at( curr.get_skip(next_freq)+1 ))
                    //curr.inc_skip(next_freq);

                //insert sets valid to false if the cform cannot be inserted
                bool valid = true;
                qnode temp = curr.insert(curr_forms[i], valid, next_value, last_insert);
                
                if(valid)
                {
                    //Check if the matrix is full or not
                    if(!last_insert)
                    {
                        valid_count++;
                        //omp_set_lock(&hashlock);
                        //ihash = nhash.insert(temp);
                        //omp_unset_lock(&hashlock);

                        //if(ihash.second)
                        {
                            omp_set_lock(&writelock);
                            insert_count++;
                            tempf.open(INFILE, ios::app);
                            tempf << temp.string_g6(round+1);
                            tempf.close();
                            omp_unset_lock(&writelock);
                        }
                    }
                    //Check if we have found a counter-example
                    else if(!temp.has_trans())
                    {
                        omp_set_lock(&outlock);
                        if(verbose) cout << "! #" << counter_count++ << " - Counter-Example Found:" << endl; 
                        if(dreadly) temp.print_dread(initial, counter_count);
                        else temp.print_clean();
                        omp_unset_lock(&outlock);
                        //return EXIT_SUCCESS;
                    }
                    //Else it is full but not a counter example
                    else
                    {
                        bad_full++;
                    }
                }

                if(verbose && insert_count % 1000000 == 0 && insert_count != last)
                {
                    last = insert_count;
                    if(verbose) cout << "!Valid: " << valid_count << "\tInsert: " << insert_count;
                    if(verbose) cout << "\tTotal Read: " << read_count << "/" << curr_queue_size;
                    if(verbose) cout << "\tLast Insertions: " << last_counter << " (" << bad_full << ")" << endl;// Left: " << nqueue.size() << endl;
                }
            }
        }
        //OpenMP Loop finished

        //return 0;
        //Clean up using nauty
        if(!last_insert)
        {
            cout << "!Calling nauty to cleanup on round " << round << ". Total Inserts: " << insert_count << " Read Count: " << read_count << endl;
            //nqueue.clear();
            ofstream cleaner;
            cleaner.open(RESFILE, ios::trunc);
            cleaner << endl;
            cleaner.close();
            nauty_cleanup(starts, m, n);
            cout << "!nauty completed. " << endl; //nqueue.size() << endl;
        }

        cout << "!Round " << round++ << " Finished." << endl; // New Size = " << nqueue.size() << endl;
        //The hash can't possibly be of any use to us anymore since all of the next values will be bigger.
        //We can safely empty it now
        //nhash.clear();

        //Increment to the next frequency
        curr_freq++;
        next_value++;

    }

    if(verbose) cout << "!Queue Cleared." << endl;
    if(verbose) cout << "!Valid Insertions: " << valid_count << "\tActual Insertions: " << insert_count << endl;

    return 0;
}

