#pragma once
#include "stdafx.h"

// Article: http://divergentcoder.com/Coding/2011/02/22/aos-soa-explorations-part-1.html

//------------------------------------------------------------------------------------
// AoS Version
//------------------------------------------------------------------------------------
struct Particle {
	float x, y, z, w;
	float vx, vy, vz, vw;
};

struct Sequential_ParticleSystem
{
	Particle *particles;
	int count;

	void update(float dt)
	{
		for (int i = 0; i<count; i++)
		{
			auto p = particles[i];
			p.x += p.vx * dt;
			p.y += p.vy * dt;
			p.z += p.vz * dt;
			p.w += p.vw * dt;
		}
	}


	static void test(int count)
	{
		auto v = rand();
		Sequential_ParticleSystem system;
		system.particles = new Particle[count];
		system.count = count;
		for (int i = 0; i < count; ++i)
		{
			system.particles[i].x = 0;
			system.particles[i].y = 0;
			system.particles[i].z = 0;
			system.particles[i].w = 0;
			system.particles[i].vx = v;
			system.particles[i].vy = v;
			system.particles[i].vz = v;
			system.particles[i].vw = v;
		}
			
		std::cout << "Sequential ParticleSystem... ";
		clock_t t0 = clock();

		for (int t = 0; t < 500; ++t)
			system.update(0.16666);

		clock_t t1 = clock();
		std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
	}
};

struct Parallel_ParticleSystem
{
	Particle *particles;
	int count;

	void update(float dt)
	{
		parallel_for(0, count, [&](int i)
		{
			auto p = particles[i];
			p.x += p.vx * dt;
			p.y += p.vy * dt;
			p.z += p.vz * dt;
			p.w += p.vw * dt;
		});
	}

	static void test(int count)
	{
		auto v = rand();
		Parallel_ParticleSystem system;
		system.particles = new Particle[count];
		system.count = count;
		for (int i = 0; i < count; ++i)
		{
			system.particles[i].x = 0;
			system.particles[i].y = 0;
			system.particles[i].z = 0;
			system.particles[i].w = 0;
			system.particles[i].vx = v;
			system.particles[i].vy = v;
			system.particles[i].vz = v;
			system.particles[i].vw = v;
		}

		std::cout << "Parallel ParticleSystem... ";
		clock_t t0 = clock();

		for (int t = 0; t < 500; ++t)
			system.update(0.16666);

		clock_t t1 = clock();
		std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
	}
};
