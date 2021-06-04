#include "Source.h"

THREADPOOL::ThreadPool::ThreadPool(size_t num_threads)
	:num_threads(num_threads), stop_all(false)
{
	worker_threads.reserve(num_threads);//������ ����ŭ �̸� �Ҵ�
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
		});//ť�� ��ų�, stop_all���� ��� cv�� ���� wait���·� ����. 


		if (stop_all && this->jobs_.empty())
		{
			return;
		}

		//�Ǿ��� job�� ����
		std::function<void()> job = std::move(jobs_.front());
		jobs_.pop();
		lock.unlock();

		//job�����Ѵ�.
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
//	worker_threads.reserve(num_threads);//num_threads��ŭ �̸� �Ҵ�
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