#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize) : queue(qsize){}

	void start (int nbthread){
		threads.reserve(nbthread);
		for(int i = 0; i<nbthread;i++){
			threads.emplace_back(&Pool::poolWorker, this);
    	}
	}

	void submit (Job * job){
		queue.push(job);
	} 

	void stop(){
		queue.setBlocking(false);
		for(auto & t: threads){
			t.join();
		}
    	threads.clear();
	}

	~Pool(){ stop();}

	void poolWorker(){
    	while(true){
			Job*j = queue.pop();
			if(j == nullptr){
				break;
			}
			j->run();
			delete j;
			}
		}
	};
}
