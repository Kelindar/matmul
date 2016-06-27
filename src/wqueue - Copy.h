#pragma once
#include "stdafx.h"


using std::deque;
using std::adopt_lock;

class Buffer
{
public:
	std::condition_variable *cond;
	std::mutex *mu;
	void add(int num) {
		while (true) {
			my_lock(*mu);
			std::unique_lock<std::mutex> locker(*mu, adopt_lock);
			cond->wait(locker, [this](){return buffer_.size() < size_; });
			buffer_.push_back(num);
			my_unlock(*mu);
			cond->notify_all();
			return;
		}
	}
	int remove() {
		while (true)
		{
			my_lock(*mu);
			std::unique_lock<std::mutex> locker(*mu, adopt_lock);
			cond->wait(locker, [this](){return buffer_.size() > 0; });
			int back = buffer_.back();
			buffer_.pop_back();
			my_unlock(*mu);
			cond->notify_all();
			return back;
		}
	}
	Buffer(std::mutex *mu, std::condition_variable *cond) { this->mu = mu; this->cond = cond; }
	Buffer() { }
private:
	deque<int> buffer_;
	const unsigned int size_ = 10;
};

class Producer
{
public:
	std::mutex *mu, *cout_mu;
	Producer(Buffer* buffer, std::mutex *mu, std::mutex * cout_mu)
	{
		this->buffer_ = buffer;
		this->mu = mu;
		this->cout_mu = cout_mu;
	}
	void run() {
		this->start = time(0);
		while ((time(0) - this->start) <= 5) {
			int num = std::rand() % 100;
			buffer_->add(num);
			my_lock(*cout_mu);
			std::cout << "Produced: " << num << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			my_unlock(*cout_mu);
		}

		cout << "producer finished work" << endl;
	}
private:
	Buffer *buffer_;
	time_t start;
};

class Consumer
{
public:
	std::mutex *mu, *cout_mu;
	Consumer(Buffer* buffer, std::mutex *mu, std::mutex * cout_mu)
	{
		this->buffer_ = buffer;
		this->mu = mu;
		this->cout_mu = cout_mu;
	}
	void run() {
		this->start = time(0);
		while ((time(0) - this->start) <= 5) {
			int num = buffer_->remove();
			my_lock(*cout_mu);
			std::cout << "Consumed: " << num << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			my_unlock(*cout_mu);
		}

		cout << "consumer finished work" << endl;
		EventWriteEnd();
	}
private:
	Buffer *buffer_;
	time_t start;
};

void wqueue(int p, int c) {
	std::mutex mu, cout_mu;
	std::condition_variable cond;
	Buffer b(&mu, &cond);
	std::vector<std::thread> producers;
	std::vector<std::thread> consumers;
	Producer tp(&b, &mu, &cout_mu);
	Consumer tc(&b, &mu, &cout_mu);

	cout << p << " producer(s) created" << endl;
	cout << c << " consumer(s) created" << endl;

	EventWriteBegin();
	
	for (int i = 0; i < p; ++i){
		producers.push_back(
			std::thread(&Producer::run, &tp)
			);
	}

	for (int i = 0; i < c; ++i){
		consumers.push_back(
			std::thread(&Consumer::run, &tc)
			);
	}

		
	for (int i = 0; i < c; ++i){
		consumers[i].join();
		cout << "consumer " << i << " finished" << endl;
	}

	for (int i = 0; i < p; ++i){
		producers[i].join();
		cout << "producer " << i << " finished" << endl;
	}

	cout << "end" << endl;
}
