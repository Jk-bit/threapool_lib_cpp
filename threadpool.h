#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>

typedef void (*task_fn)(void *);

class ThreadPoolTask
{
public:
	task_fn			m_fn;
	void			*m_args;
	task_fn			m_cb;
	void			*m_cb_args;
	ThreadPoolTask(task_fn fn, void *args, task_fn cb_fn, void *cb_args);
	~ThreadPoolTask();
};

typedef std::unique_ptr<ThreadPoolTask> tp_task;

class ThreadPool
{
private:
	int				m_nthreads;
	std::vector<std::thread>	m_threads;
	std::queue<tp_task>		m_tasks;
	std::mutex			m_lock;
	std::condition_variable		m_cv;
	bool				stop;
public:
	ThreadPool(int threads);
	~ThreadPool();
	void ExecuteTask(task_fn fn, void *args,
			 task_fn cb, void *cb_args);
};
