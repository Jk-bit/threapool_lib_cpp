#include <iostream>
#include <thread>
#include <chrono>
#include "threadpool.h"

using namespace std;

void threadpool_task(void *args)
{
	int *elem = (int *)args;
	// cout << *elem << " threadpool task function sleeping" << endl;
	this_thread::sleep_for(chrono::milliseconds(3000));
	// cout << *elem << " threadpool task function returning" << endl;
}

int main(void)
{
	ThreadPool tp(10);
	vector<int> vec(20);

	for (int ii = 0; ii < 20; ii++) {
		vec[ii] = ii;
		tp.ExecuteTask(threadpool_task, &vec[ii], nullptr, nullptr);
	}

	for (auto ii : vec) {
		cout << ii << " ";
	}
	cout << endl;

	while(1);
}
