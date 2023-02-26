#ifndef FISX_BROADPHASE_H
#define FISX_BROADPHASE_H
#include "foundation/Math.h"

namespace Fisx
{

	struct Node
	{
		AABB box;
		Node* parent = nullptr;
		union {
			Node* children[2];
			void* data;
			int dataAsInt;
		};
		inline bool isLeaf() const { return children[1] == nullptr; }
	};
	
	class BVH
	{
	public:
		BVH();

		int AddBox(const AABB& box);
		void RemoveBox(const int handle);
		void UpdateBox(const int handle, const AABB& box);

	private:
		Node* insert(const AABB& box, void* data);
		Node* createNode(const AABB& box, void* data, Node* parent);
		void deleteNode(Node* node);
		void insertLeaf(Node* root, Node* leaf);
		Node* mRoot = nullptr;  // root Node
		Node* mFree = nullptr;  // point to a free node
		int mLeafNums = 0;  // current leaf node nums
	};
}
#endif // !FISX_BROADPHASE_H
