/***********************************************************/
/***Test diffrent speed for rotating an container.       ***/
/***rotate1 is Method4 and rotate3 is almost equivalent. ***/
/***rotate2 is Method3 which uses reverse.               ***/
/***rotate4 is Method2 which complexity is lowest.       ***/
/***but theoretically Method3 is faster given to cache.  ***/
/******************THIS IS A PROBLEM!***********************/
/***********************************************************/

/**************************/
/***RESULT:             ***/
/***time1 = 0.0131469   ***/
/***time2 = 0.0133115   ***/
/***time3 = 0.0135443   ***/
/***time4 = 0.0035769   ***/
/**************************/

#include<iostream>
#include<numeric>
#include<chrono>
using namespace std;
using TIME = chrono::steady_clock;

const int _MAX = 1000000;
int a[_MAX] = { 0 }, b[_MAX] = { 0 }, c[_MAX] = { 0 }, d[_MAX] = { 0 };
double time1 = 0., time2 = 0., time3 = 0., time4 = 0.;

int gcd(int a, int b) { return a % b == 0 ? b : gcd(b, a % b); }

inline void iter_swap1(int* a,int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}

void reverse_(int* _Begin, int* _End)
{
    while (_Begin < _End)
    {
        iter_swap1(_Begin++, --_End);
    }
    return;
}

void rotate1(int* _UFirst, int* _UMid, int* _ULast)
{
    auto t1 = TIME::now();
    auto _UNext = _UMid;
    do { // rotate the first cycle
        iter_swap1(_UFirst, _UNext);
        ++_UFirst;
        ++_UNext;
        if (_UFirst == _UMid) {
            _UMid = _UNext;
        }
    } while (_UNext != _ULast);
    while (_UMid != _ULast) { // rotate subsequent cycles
        _UNext = _UMid;
        do {
            iter_swap1(_UFirst, _UNext);
            ++_UFirst;
            ++_UNext;
            if (_UFirst == _UMid) {
                _UMid = _UNext;
            }
        } while (_UNext != _ULast);
    }
    auto t2 = TIME::now();
    time1 = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
    return;
}

void rotate2(int* _UFirst, int* _UMid, int* _ULast)
{
    auto t1 = TIME::now();
    reverse_(_UFirst, _UMid);
    reverse_(_UMid, _ULast);
    reverse_(_UFirst, _ULast);
    auto t2 = TIME::now();
    time2 = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
    return;
}

void rotate3(int* first,int*mid,int*last)
{
    auto t1 = TIME::now();
    int* next = mid;
    while (first != next)
    {
        iter_swap1(first++, next++);
        if (next == last) next = mid;
        else if (first == mid) mid = next;
    }
    auto t2 = TIME::now();
    time3 = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
    return;
}

void rotate4(int* first, int* mid, int* last)
{
    auto t1 = TIME::now();
    int n = last - first, k = mid - first;
    int cnt = gcd(n, k);
    int* Init_first = first;
    while (cnt--)
    {
        int temp = *first;
        int* currpos = first + k, *lastpos = first;
        while (currpos != first)
        {
            *lastpos = *currpos;
            lastpos = currpos;
            currpos += k;
	    //Here we don't use % to be faster.
            if (currpos >= last) currpos = (currpos - last) + Init_first;
        }
        *lastpos = temp;
        first++;
    }
    auto t2 = TIME::now();
    time4 = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();
    return;
}

int main()
{
    const int pos = 2500;
    iota(a, a + _MAX, 0);
    rotate1(a, a + pos, a + _MAX);
    iota(b, b + _MAX, 0);
    rotate2(b, b + pos, b + _MAX);
    iota(c, c + _MAX, 0);
    rotate3(c, c + pos, c + _MAX);
    iota(d, d + _MAX, 0);
    rotate4(d, d + pos, d + _MAX);
    cout << time1 << " " << time2 <<" "<<time3 <<" "<<time4<< endl;
	return 0;
}
