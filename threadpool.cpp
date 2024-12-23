#include "threadpool.h"
#include <iostream>

using namespace std;

threadpool_task::threadpool_task(task_fn fn, void *args, task_fn cb, void *cb_args)
	: m_fn(fn),
	  m_args(args),
	  m_cb(cb),
	  m_cb_args(cb_args)
{
}

threadpool_task::~threadpool_task()
{

}

threadpool::threadpool(int threads) : m_nthreads(threads), stop(false)
{
	for (int ii = 0; ii < m_nthreads; ii++) {
		m_threads.emplace_back([this, ii]{
			tp_task		task;
			void		*args;

			while (true) {
				unique_lock<mutex> lock(m_lock);
				m_cv.wait(lock, [this]{ return !m_tasks.empty() || stop; });
				cout << ii << " thread woke up and running" << endl;
				if (stop) {
					lock.unlock();
					break;
				}
				task = move(m_tasks.front());
				m_tasks.pop();
				lock.unlock();
				task->m_fn(task->m_args);
				if (task->m_cb) {
					task->m_cb(task->m_cb_args);
				}
			}
		});
	}
}

threadpool::~threadpool()
{
	unique_lock<mutex> lock(m_lock);
	stop = true;
	m_cv.notify_all();
	lock.unlock();

	for (auto &th: m_threads) {
		th.join();
	}
}

void threadpool::execute_task(task_fn fn, void *args,
			      task_fn cb, void *cb_args)
{
	unique_ptr<threadpool_task> task = make_unique<threadpool_task>(fn, args,
								        cb, cb_args);
	unique_lock<mutex> lock(m_lock);
	m_tasks.push(move(task));
	m_cv.notify_one();
	lock.unlock();
}
