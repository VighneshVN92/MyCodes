#include <iostream>
#include"ThreadPool.h"

int main() 
{ 
	// Create a thread pool
	ThreadPool Thread_pool(5); 
	// Enqueue tasks for execution 
	for (int i = 0; i < 5; ++i) { 
		Thread_pool.enqueue([i] { 
			std::cout << "Task " << i << " is running on thread "
				<< std::this_thread::get_id() << std::endl; 
			// Adding a delay
			std::this_thread::sleep_for( std::chrono::milliseconds(50)); 
		}); 
	} 

	return 0; 
}