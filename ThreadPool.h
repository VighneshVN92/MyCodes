// C++ Program to demonstrate thread pooling 

#include <condition_variable> 
#include <functional> 
#include <mutex> 
#include <queue> 
#include <thread> 


// Class thread pool 
class ThreadPool { 
public: 
	//Constructor
	ThreadPool(size_t num_threads ) 
	{ 
		// Creating worker threads 
		for (size_t i = 0; i < num_threads; ++i) { 
			worker_threads_.emplace_back([this] { 
				while (true) { 
					std::function<void()> task; 
					{ 
						std::unique_lock<std::mutex> lock( queue_mutex_); 
						cv_.wait(lock, [this] { return !tasks_.empty() || stop_;}); 
						if (stop_ && tasks_.empty()) { 
							return; 
						} 
						task = std::move(tasks_.front()); 
						tasks_.pop(); 
					} 
					task(); 
				} 
			}); 
		} 
	} 
	// Destructor
	~ThreadPool() 
	{ 
		{ 
			std::unique_lock<std::mutex> lock(queue_mutex_); 
			stop_ = true; 
		} 
		cv_.notify_all(); 
		for (auto& thread : worker_threads_) { 
			thread.join(); 
		} 
	} 

	// Enqueue function
	void enqueue(std::function<void()> task) 
	{ 
		{ 
			std::unique_lock<std::mutex> lock(queue_mutex_); 
			tasks_.emplace(std::move(task)); 
		} 
		cv_.notify_one(); 
	} 

private: 
	std::vector<std::thread> worker_threads_;  
	std::queue<std::function<void()> > tasks_;  
	std::mutex queue_mutex_;
	std::condition_variable cv_;  
	bool stop_ = false; 
}; 

