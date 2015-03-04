#pragma once
#include "stdafx.h"


struct TreeNode {
	TreeNode* left;  //! Pointer to left subtree
	TreeNode* right; //! Pointer to right subtree
	long node_count; //! Number of nodes in this subtree, including this node.
	int value;       //! Value associated with the node.
};

/*
 Recursively creates a tree
*/
static TreeNode* CreateTree(long number_of_nodes) {
	if (number_of_nodes == 0) {
		return NULL;
	}
	else {
		TreeNode* n = new TreeNode();
		n->node_count = number_of_nodes;
		n->value = int(1);
		--number_of_nodes;
		n->left = CreateTree(number_of_nodes / 2);
		n->right = CreateTree(number_of_nodes - number_of_nodes / 2);
		return n;
	}
}


//------------------------------------------------------------------------------------
// SERIAL
//------------------------------------------------------------------------------------
int Serial_SumTree(TreeNode* root) {
	int result = root->value;
	if (root->left)
		result += Serial_SumTree(root->left);
	if (root->right)
		result += Serial_SumTree(root->right);
	return result;
};


void Sequential_SumTree(TreeNode* root) {
	// Begin measurement
	std::cout << "Sequential SumTree v1... ";
	clock_t t0 = clock();

	Serial_SumTree(root);

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
}


//------------------------------------------------------------------------------------
// SIMPLE PARALLEL
//------------------------------------------------------------------------------------
void SubSum(TreeNode* root, int* sum) {
	if (root->node_count<1000) {
		*sum = Serial_SumTree(root);
	}
	else {
		int x, y;
		int count = 1;
		concurrency::task_group list;
		if (root->left) {
			list.run([&](){
				SubSum(root->left, &x);
			});
		}
		if (root->right) {
			list.run([&](){
				SubSum(root->right, &y);
			});
		}

		list.wait();
		*sum = root->value;
		if (root->left) *sum += x;
		if (root->right) *sum += y;
	}
}

void Parallel_SumTree(TreeNode* root) {
	// Begin measurement
	std::cout << "Parallel SumTree v1... ";
	clock_t t0 = clock();

	int sum;
	SubSum(root, &sum);

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
}

