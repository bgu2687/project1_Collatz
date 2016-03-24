#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <stdio.h>
#include <string.h>
#include <fstream>

using namespace std;

#define MAX_BUF_SIZE 1048576
#define INIT_BUF_SIZE 40
#define MAX_GRP_BUF_SIZE 1048576
#define GROUP_RANGE 10000 

/**
 *  use the buffers to improve the performance for 3n + 1 problem.
 *  single buffer to save the computed cycles for each single integer
 *  group buffer to save the computed max cycles for a group of integers
 */

// ------------
// collatz_read
// ------------

bool collatz_read (istream& r, int& i, int& j)
{
    if (!(r >> i))
        return false;
    r >> j;
    return true;
}


int collatz_eval (int i, int j);
// 40 pre computed values
int pre_compute_buffer[] =
{
    0,
    1, 2, 8, 3, 6,
    9, 17, 4, 20, 7,
    15, 10, 10, 18, 18,
    5, 13, 21, 21, 8,

    8, 16, 16, 11, 24,
    11, 112, 19, 19, 19,
    107, 6, 27, 14, 14,
    22, 22, 22, 35, 9,
};


int single_buffer[MAX_BUF_SIZE] = {0};// buffer for each single number
int grp_buffer[MAX_GRP_BUF_SIZE] = {0};// buffer for a group. e.g.[1, 999], [1000, 1999]

void print_buffer(int buffer[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << buffer[i] << ",";
        if (i % 50 == 0)
        {
            std::cout << std::endl;
        }

        if (i % 10 == 0)
        {
            std::cout << "---";
        }
    }
}

// get value from buffer
int get_val(int buffer[], int size, int idx)
{
    if (idx < size && idx > 0)
    {
        if (buffer[idx] > 0)
        {
            return  buffer[idx];
        }
    }

    return 0;
}

// save value in the buffer
void set_val(int buffer[], int size, int idx, int val)
{
    if (idx < size && idx > 0)
    {
        buffer[idx] = val;
    }
}

// compute the cycle for an integer
int collatz_eval_helper(int i)
{
    if(i <= 1)
    {
        return 1;
    }
    else
    {
        int ret = 0, idx = 0;
        if (i % 2 == 1)
        {
            idx = 3 * i + 1;
        }
        else
        {
            idx = i / 2;
        }
        ret = get_val(single_buffer, MAX_BUF_SIZE, idx);

        if (ret > 0)
        {
            ret = ret + 1;
        }
        else
        {
            ret = collatz_eval_helper(idx) + 1;
        }

        set_val(single_buffer, MAX_BUF_SIZE, i, ret);
        return ret;
    }
}


// Compute the max cycle from i to j, 
// e.g. [i,j] = [1, GROUP_RANGE - 1], [GROUP_RANGE, GROUP_RANGE * 2 - 1]
// i and j have the same division result by GROUP_RANGE
int collatz_eval_grp_helper (int i, int j)
{
    int idx = i/GROUP_RANGE;
    int ret = 0;
    if (idx == j/GROUP_RANGE)
    {
        ret = get_val(grp_buffer, MAX_GRP_BUF_SIZE, idx);
        if (ret == 0)
        {
            ret = collatz_eval(i, j);
        }
        set_val(grp_buffer, MAX_GRP_BUF_SIZE, idx, ret);
    }

    return ret;
}

int collatz_eval_grp (int i, int j)
{
    int max = 0, ret = 0;
    if (i > j)
    {
        int tmp = i;
        i = j;
        j = tmp;
    }

    int start = i / GROUP_RANGE;
    int end  = j /GROUP_RANGE;

    //std::cout <<"start " << start << " , " << end << std::endl;
    // get the max cycle for the middle groups
    for (int k = start + 1; k <= end - 1; k++)
    {
        ret = collatz_eval_grp_helper(k * GROUP_RANGE, k * GROUP_RANGE + GROUP_RANGE - 1);
        max = std::max(max, ret);
    }

    end = (i / GROUP_RANGE  + 1) * GROUP_RANGE - 1;
    //std::cout <<"i " << i << " , " << end  << " j " << j << std::endl;
    if (j <= end)
    {
        // in the same group range
        ret = collatz_eval(i, j);
        max = std::max(max, ret);
    }
    else
    {
        // in different group range
        ret = collatz_eval(i, end);
        max = std::max(max, ret);
        ret = collatz_eval( j / GROUP_RANGE * GROUP_RANGE, j);
        max = std::max(max, ret);
    }

    //print_buffer(grp_buffer, MAX_GRP_BUF_SIZE);
    return max;
}

// ------------
// collatz_eval
// ------------

int collatz_eval (int i, int j)
{
    int max = 0;
    int cycle = 0;
    if (i > j)
    {
        int tmp = i;
        i = j;
        j = tmp;
    }
    for (int k = i; k <= j; k++)
    {
        cycle = collatz_eval_helper(k);
        if (cycle > max)
        {
            max = cycle;
        }
    }
    
    return max;
}


// -------------
// collatz_print
// -------------

void collatz_print (ostream& w, int i, int j, int v)
{
    w << i << " " << j << " " << v << endl;
}

// -------------
// collatz_solve
// -------------

void collatz_solve (istream& r, ostream& w)
{
    int i;
    int j;
    // initialize the buffer with pre computed value.
    memcpy (single_buffer, pre_compute_buffer, sizeof(pre_compute_buffer) );

    while (collatz_read(r, i, j))
    {
        //const int v = collatz_eval(i, j);
        const int v = collatz_eval_grp(i, j);
        collatz_print(w, i, j, v);
    }
}

int main ()
{

#if 0
    ofstream myfile;
    myfile.open("result.txt");
    collatz_solve(cin, myfile);
    myfile.close();
#else
    collatz_solve(cin, cout);

#endif
    return 0;
}



