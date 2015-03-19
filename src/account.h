#pragma once
#include "stdafx.h"

//------------------------------------------------------------------------------------
// Program A
//------------------------------------------------------------------------------------
struct AccountA {
	long id;
	char name[50];
	char address[300];
	float reputation;

	static void test(int count)
	{
		// Make some data
		auto accounts = new AccountA[count];
		for (int i = 0; i < count; ++i) {
			accounts[i].id = 1;
			accounts[i].reputation = 0;
		}

		std::cout << "Accounts (A)... ";
		clock_t t0 = clock();

		// Start monitoring
		EventWriteBegin();
		{
			// we want to update and add some reputation every day
			for (int i = 0; i < 100; ++i){
				parallel_for(0, count, [&](int i) {
					accounts[i].reputation += 2.5;
				});
			}
		}
		EventWriteEnd();

		clock_t t1 = clock();
		std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
	}
};

//------------------------------------------------------------------------------------
// Program B
//------------------------------------------------------------------------------------

struct AccountInfo {
	char name[50];
	char address[300];
};

struct AccountB {
	long id;
	AccountInfo* info;
	float reputation;

	static void test(int count)
	{
		// Make some data
		auto accounts = new AccountB[count];
		for (int i = 0; i < count; ++i) {
			accounts[i].id = 1;
			accounts[i].info = new AccountInfo();
			accounts[i].reputation = 0;
		}

		std::cout << "Accounts (B)... ";
		clock_t t0 = clock();

		// Start monitoring
		EventWriteBegin();
		{
			// we want to update and add some reputation every day
			for (int i = 0; i < 100; ++i){
				parallel_for(0, count, [&](int i) {
					accounts[i].reputation += 2.5;
				});
			}
		}
		EventWriteEnd();

		clock_t t1 = clock();
		std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
	}
};