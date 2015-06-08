#pragma once
#include "stdafx.h"

using std::cout;
using std::endl;
using std::adopt_lock;
using std::for_each;
using std::lock;
using std::mem_fn;
using std::move;
using std::to_string;
using std::exception;
using std::lock_guard;
using std::mutex;
using std::string;
using std::thread;
using std::unique_ptr;
using std::vector;

class Chopstick
{
public:
	Chopstick(){};
	mutex m;
};

int philosopher(int numPhilosophers)
{
	auto eat = [](Chopstick* leftChopstick, Chopstick* rightChopstick, int philosopherNumber, int leftChopstickNumber, int rightChopstickNumber)
	{
		if (leftChopstick == rightChopstick)
			throw exception("Left and right chopsticks should not be the same!");

		my_lock(leftChopstick->m, rightChopstick->m);				// ensures there are no deadlocks

		lock_guard<mutex> a(leftChopstick->m, adopt_lock);
		string sl = "   Philosopher " + to_string(philosopherNumber) + " picked " + to_string(leftChopstickNumber) + " chopstick.\n";
		cout << sl.c_str();

		lock_guard<mutex> b(rightChopstick->m, adopt_lock);
		string sr = "   Philosopher " + to_string(philosopherNumber) + " picked " + to_string(rightChopstickNumber) + " chopstick.\n";
		cout << sr.c_str();

		string pe = "Philosopher " + to_string(philosopherNumber) + " eats.\n";
		cout << pe;

		std::chrono::milliseconds timeout(500);
		std::this_thread::sleep_for(timeout);
	};
	EventWriteBegin();
	{

		// 5 utencils on the left and right of each philosopher. Use them to acquire locks.
		vector< unique_ptr<Chopstick> > chopsticks(numPhilosophers);

		for (int i = 0; i < numPhilosophers; ++i)
		{
			auto c1 = unique_ptr<Chopstick>(new Chopstick());
			chopsticks[i] = move(c1);
		}

		// This is where we create philosophers, each of 5 tasks represents one philosopher.
		vector<thread> tasks(numPhilosophers);

		tasks[0] = thread(eat,
			chopsticks[0].get(),						// left chopstick:  #1
			chopsticks[numPhilosophers - 1].get(),		// right chopstick: #5
			0 + 1,										// philosopher number
			1,
			numPhilosophers
			);

		for (int i = 1; i < numPhilosophers; ++i)
		{
			tasks[i] = (thread(eat,
				chopsticks[i - 1].get(),				// left chopstick
				chopsticks[i].get(),					// right chopstick
				i + 1,									// philosopher number
				i,
				i + 1
				)
				);
		}

		// May eat!
		for_each(tasks.begin(), tasks.end(), mem_fn(&thread::join));

	}
	EventWriteEnd();

	return 0;
}