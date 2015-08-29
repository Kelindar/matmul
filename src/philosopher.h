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
using std::srand;


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

		while (true) {
			if (my_try_lock(leftChopstick->m)) {
				if (my_try_lock(rightChopstick->m)) {

					string sr = "   Philosopher " + to_string(philosopherNumber) + " picked chopsticks " + to_string(leftChopstickNumber) + " and " + to_string(rightChopstickNumber) + ".\n";
					cout << sr.c_str();
					break;
				}
				else {
					my_unlock(leftChopstick->m);
				}
			}

			std::chrono::milliseconds timeout(10);
			std::this_thread::sleep_for(timeout);
		}


		std::chrono::milliseconds timeout(500);
		string sr = "   Philosopher " + to_string(philosopherNumber) + " eating for " + to_string(timeout.count()) + " milliseconds ...\n";
		cout << sr.c_str();

		std::this_thread::sleep_for(timeout);

		my_unlock(leftChopstick->m);
		my_unlock(rightChopstick->m);

		
	};

	EventWriteBegin();
	{

		// utencils on the left and right of each philosopher. Use them to acquire locks.
		vector< unique_ptr<Chopstick> > chopsticks(numPhilosophers);

		for (int i = 0; i < numPhilosophers; ++i)
		{
			auto c1 = unique_ptr<Chopstick>(new Chopstick());
			chopsticks[i] = move(c1);
		}

		// This is where we create philosophers, each of 5 tasks represents one philosopher.
		vector<thread> tasks(numPhilosophers);

		tasks[0] = thread(eat,
			chopsticks[0].get(),						// left chopstick
			chopsticks[numPhilosophers - 1].get(),		// right chopstick
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