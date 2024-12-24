#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>

typedef void (*task_fn)(void *);

class threadpool_task
{
public:
	task_fn			m_fn;
	void			*m_args;
	task_fn			m_cb;
	void			*m_cb_args;
	threadpool_task(task_fn fn, void *args, task_fn cb_fn, void *cb_args);
	~threadpool_task();
};

typedef std::unique_ptr<threadpool_task> tp_task;

class threadpool
{
private:
	int				m_nthreads;
	std::vector<std::thread>	m_threads;
	std::queue<tp_task>		m_tasks;
	std::mutex			m_lock;
	std::condition_variable		m_cv;
	bool				stop;
public:
	threadpool(int threads);
	~threadpool();
	void execute_task(task_fn fn, void *args,
			 task_fn cb, void *cb_args);
};
