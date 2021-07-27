/***********************************************/
/***RESULT:                                  ***/
/***_Size   time1    time2     expand_times  ***/
/***40    0.0001681 0.0001821    0 & 0       ***/
/***400   0.0001727 0.0001823    0 & 0       ***/
/***4000  0.0002284 0.0001939    0 & 0       ***/ 
/***40000 0.0007601 0.0002787    0 & 0       ***/
/***50010 0.0013890 0.0005844    1 & 1       ***/ 
/***40    0.0004862 0.0003467    1 & 1       ***/
/***400   0.0004704 0.0003601    1 & 1       ***/
/***4000  0.0005341 0.0003887    1 & 1       ***/
/***40000 0.0010425 0.0004620    1 & 1       ***/
/***50010 0.0016143 0.0004205    2 & 1       ***/
/***50010 0.0017071 0.0009561    2 & 2       ***/
/***The difference between the last two is   ***/
/***that one more insert(noted in code) is   ***/
/***given in the last one,which is fair given***/
/***to the second expanding way.             ***/
/***********************************************/
#include<iostream>
#include"Insert_test.h"
#include<chrono>
#include<numeric>
using namespace std;

const size_t _MAX = 100000;
const size_t _Size = 40;
int a[_MAX] = { 0 };
double time1 = 0., time2 = 0.;
int insert_test[_Size] = { 0 };

int main()
{
	for (int i = 0; i < _MAX; i++)
	{
		a[i] = rand();
	}
	Vector<int> test1(a, a + _MAX);
	Vector<int> test2(a, a + _MAX);
	iota(insert_test, insert_test + _Size, 0);
	test1.push_back(1), test2.push_back(1);//this statement can be noted.

	auto t1 = chrono::steady_clock::now();
	test1.insert1(0, insert_test, insert_test + _Size);
  //test1.push_back(1);
	auto t2 = chrono::steady_clock::now();
	time1 = chrono::duration_cast<chrono::duration<double>>(t2 - t1).count();

	auto t3 = chrono::steady_clock::now();
	test2.insert2(0, insert_test, insert_test + _Size);
  //test2.push_back(1);
	auto t4 = chrono::steady_clock::now();
	time2 = chrono::duration_cast<chrono::duration<double>>(t4 - t3).count();
	
	cout << time1 << " " << time2 << endl;
	return 0;
}
