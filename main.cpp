#include <iostream>
#include <thread>
#include <chrono>
#include "threadpool.h"

using namespace std;

void test_fn(void *args)
{
	int *elem = (int *)args;
	// cout << *elem << " threadpool task function sleeping" << endl;
	this_thread::sleep_for(chrono::milliseconds(3000));
	// cout << *elem << " threadpool task function returning" << endl;
}

int main(void)
{
	threadpool	tp(10);
	vector<int>	vec(20);

	for (int ii = 0; ii < 20; ii++) {
		vec[ii] = ii;
		tp.execute_task(test_fn, &vec[ii], nullptr, nullptr);
	}

	for (auto ii : vec) {
		cout << ii << " ";
	}
	cout << endl;

	while(1);
}
