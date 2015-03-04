#pragma once
#include "stdafx.h"

// Based on: http://codereview.stackexchange.com/questions/7684/parallel-merge-sort
// More info: http://www.drdobbs.com/parallel/parallel-in-place-merge-sort/240169094



template <class In>
void merge(In b, In m, In e) { // this is the merger, doing the important job
	std::vector<typename In::value_type> left(b, m);
	std::vector<typename In::value_type> right(m, e);

	// guards for the two ranges
	left.push_back(std::numeric_limits<typename In::value_type>::max());
	right.push_back(std::numeric_limits<typename In::value_type>::max());

	auto itl = std::begin(left);
	auto itr = std::begin(right);

	while (b != e) {
		if (*itl <= *itr)
			*b++ = *itl++;
		else
			*b++ = *itr++;
	}
}

template <class In>
void Parallel_MergeSort(In begin, In end) {
	if (begin != end - 1) {
		auto dis = std::distance(begin, end);
		In middle = begin + dis / 2;
		concurrency::parallel_invoke(
			[&]() { Parallel_MergeSort(begin, middle); },
			[&]() { Parallel_MergeSort(middle, end); }
		);
		merge(begin, middle, end);
	}
}


template <class In>
void Sequential_MergeSort(In begin, In end) {
	if (begin != end - 1) {
		auto dis = std::distance(begin, end);
		In middle = begin + dis / 2;
		Sequential_MergeSort(begin, middle);
		Sequential_MergeSort(middle, end);
		merge(begin, middle, end);
	}
}

