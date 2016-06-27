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

int param1, param2;
int sizeOfArray, touchSize;
std::mutex ketchup, array_lock;
int* philosopher_array;

#define _USE_KETCHUP 1 // lock contention
#define _USE_TOUCH_RANDOM_PARTS_OF_ARRAY 2 // true data sharing

int philosopher_eat_method = 0;

class Chopstick
{
	public:
		Chopstick(){};
		mutex m;
};

void philosopher_think()
{
	std::chrono::milliseconds timeout(50);
	std::this_thread::sleep_for(timeout);
}

int getFirstPrimes(int number)
{
	int primeCount = 0, counter=0;
	//Determines if a number is prime
	for (int prime = 1; counter < number; prime++)
	{
		primeCount = 0;
		for (int i = 1; i <= prime; i++)
			if (prime % i == 0)
				primeCount++;
	
		if (primeCount == 2)
			counter++;
	}
	return counter;
}

std::vector<int> getListOfLocations(int a, int b)
{
	std::vector<int> retVal;
	std::vector<int> all_locations;
	for (int i = 0; i < a; ++i)
		all_locations.push_back(i);
	for (int i = 0; i < b; ++i)
	{
		int location = rand() % all_locations.size();
		retVal.push_back(all_locations[location]);
		all_locations.erase(all_locations.begin()+location);
	}
	return retVal;
}

int philosopher_eat()
{
	//std::chrono::milliseconds timeout(500);
	//std::this_thread::sleep_for(timeout);

	// high base gives low variability; low base and 
	// high variance gives low variability
	switch (philosopher_eat_method)
	{
		case _USE_KETCHUP:
		{
			my_lock(ketchup);
			int base = param1, variance = param2;
			int num = base + rand() % variance;
			int retVal = getFirstPrimes(num);
			my_unlock(ketchup);
			return retVal;
		}
		case _USE_TOUCH_RANDOM_PARTS_OF_ARRAY:
		{
			std::vector<int> listOfLocations(touchSize);
			listOfLocations = getListOfLocations(sizeOfArray, touchSize);
			int retVal = 0;
			my_lock(array_lock);
			for (int i = 0; i < listOfLocations.size(); ++i)
			{
				philosopher_array[listOfLocations[i]] += 1;
				retVal += philosopher_array[listOfLocations[i]];
			}
			my_unlock(array_lock);
			return (retVal);
		}
		default:
		{
			cout << "\nImproper eating method. Bad philosopher!";
			exit(-1);
		}
	}
}

int philosopher(int numPhilosophers, int numberOfRounds, int my_param1, int my_param2)
{
	param1 = my_param1;
	param2 = my_param2;
	auto eat = [](Chopstick* leftChopstick, Chopstick* rightChopstick, int philosopherNumber, int leftChopstickNumber, int rightChopstickNumber, int numberOfRounds)
	{
		int i;
		for (i = 0; i < numberOfRounds; i++) {

			if (leftChopstick == rightChopstick)
				throw exception("Left and right chopsticks should not be the same!");

			while (true) {
				if (my_try_lock(leftChopstick->m)) {
					if (my_try_lock(rightChopstick->m)) {
						break;
					}
					else {
						my_unlock(leftChopstick->m);
					}
				}

				// think
				philosopher_think();
			}

			string sr = "   Philosopher " + to_string(philosopherNumber) + " picked chopsticks " + to_string(leftChopstickNumber) + " and " + to_string(rightChopstickNumber) + ".\n";
			cout << sr.c_str();
			
			clock_t begin = clock();
			// eat
			
			int retVal = philosopher_eat();
			clock_t end = clock();
			sr = "   Philosopher " + to_string(philosopherNumber) + " ate " + to_string(retVal) + " spaghetti strings for " + to_string((end-begin))+ " milliseconds ...\n";
			cout << sr.c_str();		

			my_unlock(leftChopstick->m);
			my_unlock(rightChopstick->m);
			philosopher_think();
		}
		
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

		// True sharing
		// Create list of numbers of philosophers to play with while eating
		if (philosopher_eat_method == _USE_TOUCH_RANDOM_PARTS_OF_ARRAY)
		{
			sizeOfArray = param1;
			touchSize = param2;
			philosopher_array = new int[sizeOfArray];
			for (int i = 0; i < sizeOfArray; ++i)
				philosopher_array[i] = 0;
		}

		// This is where we create philosophers, each of 5 tasks represents one philosopher.
		vector<thread> tasks(numPhilosophers);

		tasks[0] = thread(eat,
			chopsticks[0].get(),						// left chopstick
			chopsticks[numPhilosophers - 1].get(),		// right chopstick
			0 + 1,										// philosopher number
			1,
			numPhilosophers,
			numberOfRounds
			);

		for (int i = 1; i < numPhilosophers; ++i)
		{
			tasks[i] = (thread(eat,
				chopsticks[i - 1].get(),				// left chopstick
				chopsticks[i].get(),					// right chopstick
				i + 1,									// philosopher number
				i,
				i + 1,
				numberOfRounds
				)
				);
		}

		// May eat!
		for_each(tasks.begin(), tasks.end(), mem_fn(&thread::join));

	}
	EventWriteEnd();

	return 0;
}