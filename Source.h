#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace THREADPOOL {
	class ThreadPool
	{
	public:
		ThreadPool(size_t num_threads);
		~ThreadPool();
		void EnqueueJob(std::function<void()> job);

	private:
		size_t num_threads;
		std::vector<std::thread> worker_threads;//��Ŀ ������ �����ϴ� ����
		std::queue<std::function<void()>> jobs_;//��������� �� �� �����ϴ� queue
		std::condition_variable cv_job_q_;
		std::mutex m_job_q_;

		bool stop_all;//��羲����� ���� �ñ׳�
		void WorkerThread();//��Ŀ ������ �ϳ��� �Լ� 
	};
}

//namespace THREADPOOL {
//	class ThreadPool
//	{
//	public:
//		ThreadPool(size_t num_threads);
//		~ThreadPool();
//		void AdditionQueueJob(std::function<void()> job);
//
//	private:
//		size_t num_threads;//��Ŀ������ ����
//		std::vector <std::thread> worker_threads;//������ �����ϴ� ����
//		std::queue<std::function<void()>> threadjobs;//��������� �� �� �����ϴ� queue
//		std::condition_variable cv_threadjobs;
//		std::mutex mtxthreadjobs;
//
//		bool stop_all_threads;//��� ������� ���� �ñ׳�
//		void WorkerThread();//��Ŀ������ �� ���� �Լ�
//	};
//
//}




