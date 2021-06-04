#include "Source.h"

THREADPOOL::ThreadPool::ThreadPool(size_t num_threads)
	:num_threads(num_threads), stop_all(false)
{
	worker_threads.reserve(num_threads);//쓰레드 수만큼 미리 할당
	for (size_t i = 0; i < num_threads; i++)
	{
		worker_threads.emplace_back([this]{
			this->WorkerThread();
		});//Lamda capture by this
	}
}


THREADPOOL::ThreadPool::~ThreadPool()
{
	stop_all = true;
	cv_job_q_.notify_all();

	for (auto& threads : worker_threads)
	{
		threads.join();
	}
}



void THREADPOOL::ThreadPool::WorkerThread()
{
	while (1)
	{
		std::unique_lock<std::mutex> lock(m_job_q_);
		cv_job_q_.wait(lock, [this] {
			return this->jobs_.empty() || stop_all;
		});//큐가 비거나, stop_all들어올 경우 cv를 통해 wait상태로 들어간다. 


		if (stop_all && this->jobs_.empty())
		{
			return;
		}

		//맨앞의 job을 뺀다
		std::function<void()> job = std::move(jobs_.front());
		jobs_.pop();
		lock.unlock();

		//job수행한다.
		job();
	}


}


//void THREADPOOL::ThreadPool::WorkerThread()
//{
//	while (1)
//	{
//		std::unique_lock<std::mutex> lock(mtxthreadjobs);
//		cv_threadjobs.wait(lock, [this]() {
//			return !this->threadjobs.empty() || stop_all_threads;
//		});
//
//		if (stop_all_threads && this->threadjobs.empty())
//		{
//			return;
//		}
//
//	}
//}


//THREADPOOL::ThreadPool::ThreadPool(size_t num_threads)
//	: num_threads(num_threads), stop_all_threads(false)
//{
//	worker_threads.reserve(num_threads);//num_threads만큼 미리 할당
//	for (size_t i = 0; i < num_threads; i++)
//	{
//		worker_threads.emplace_back([this]() {//Lamda capture by this
//			this->WorkerThread();
//		});
//	}
//}
//
//THREADPOOL::ThreadPool::~ThreadPool()
//{
//	stop_all_threads = true;
//	cv_threadjobs.notify_all();
//
//	for (auto& threads : worker_threads)
//	{
//		threads.join();
//	}
//}
//