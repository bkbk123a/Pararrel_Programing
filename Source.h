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
		std::vector<std::thread> worker_threads;//워커 쓰레드 보관하는 벡터
		std::queue<std::function<void()>> jobs_;//쓰레드들이 할 일 보관하는 queue
		std::condition_variable cv_job_q_;
		std::mutex m_job_q_;

		bool stop_all;//모든쓰레드들 종료 시그널
		void WorkerThread();//워커 쓰레드 일내용 함수 
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
//		size_t num_threads;//워커쓰레드 개수
//		std::vector <std::thread> worker_threads;//쓰레드 보관하는 벡터
//		std::queue<std::function<void()>> threadjobs;//쓰레드들이 할 일 보관하는 queue
//		std::condition_variable cv_threadjobs;
//		std::mutex mtxthreadjobs;
//
//		bool stop_all_threads;//모든 쓰레드들 종료 시그널
//		void WorkerThread();//워커쓰레드 일 내용 함수
//	};
//
//}




