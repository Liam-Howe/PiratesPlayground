#include "stdafx.h"
#include "ThreadPool.h"

WorkQueue::WorkQueue()
	: m_condition(SDL_CreateCond())
	, m_lockQueue(SDL_CreateMutex())
{
}

// ADDING TASKS TO THE QUEUE
///////////////////////////////////////////////////////
void WorkQueue::addTask(std::function<void()> newTask)
{
	if (!finished)
	{
		SDL_LockMutex(m_lockQueue);
		m_tasks.push(newTask);
		SDL_CondSignal(m_condition);
		SDL_UnlockMutex(m_lockQueue);
	}
}
// ADDING TASKS TO THE QUEUE
///////////////////////////////////////////////////////
std::function<void()> WorkQueue::nextTask()
{
	std::function<void()> nextJob = 0;
	SDL_LockMutex(m_lockQueue);
	if (finished && m_tasks.size() == 0)
	{
		nextJob = 0;
	}
	else
	{
		while (m_tasks.size() == 0)
		{
			SDL_CondWait(m_condition, m_lockQueue);
		}
		nextJob = m_tasks.front();
		m_tasks.pop();
	}
	SDL_UnlockMutex(m_lockQueue);
	return nextJob;
}
// FINISH
///////////////////////////////////////////////////////
void WorkQueue::finish()
{
	SDL_LockMutex(m_lockQueue);
	finished = true;
	SDL_CondSignal(m_condition);
	SDL_UnlockMutex(m_lockQueue);

	SDL_DestroyMutex(m_lockQueue);
	SDL_DestroyCond(m_condition);
}

bool WorkQueue::hasWork()
{
	return m_tasks.size() > 0;
}

// THREADED FUNCTION THAT TAKES WORKQUEUE DATA AS A PARAMETER
///////////////////////////////////////////////////////
int getWork(void * param)
{
	std::function<void()> moreWork;
	WorkQueue * workQueue = static_cast<WorkQueue*>(param);
	while (moreWork = workQueue->nextTask())
	{
		moreWork();
	}
	return 0;
}
// THREADEDPOOL 
///////////////////////////////////////////////////////
ThreadPool::ThreadPool()
{
	int numOfCores = SDL_GetCPUCount() - 1;
	for (int i = 0; i < numOfCores; i++)
	{
		m_threads.push_back(SDL_CreateThread(getWork, "", &workQueue));
	}
}
// THREADEDPOOL DESTRUCTOR
///////////////////////////////////////////////////////
ThreadPool::~ThreadPool()
{
	workQueue.finish();
	waitForCompletion();
	for (int i = 0; i < m_threads.size(); i++)
	{
		SDL_WaitThread(m_threads[i], NULL);
	}
}

void ThreadPool::addTask(std::function<void()> newTask)
{
	workQueue.addTask(newTask);
}

void ThreadPool::waitForCompletion()
{
	while (workQueue.hasWork())
	{
	}
}
