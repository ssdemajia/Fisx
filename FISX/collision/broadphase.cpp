#include "collision/broadphase.h"

namespace Fisx {
	inline static float Proximity(const AABB& a, const AABB& b)
	{
		const Vector3 d = (a.mMin + a.mMax) - (b.mMin + b.mMax);
		return fabsf(d.x) + fabsf(d.y) + fabsf(d.z);
	}

	inline static int Select(const AABB& o, const AABB& a, const AABB& b)
	{
		return (Proximity(o, a) < Proximity(o, b) ? 0 : 1);
	}

	inline static void Merge(const AABB& a, const AABB& b, AABB& c)
	{
		for (int i = 0; i < 3; i++)
		{
			c.mMin[i] = fminf(a.mMin[i], b.mMin[i]);
			c.mMax[i] = fmaxf(a.mMax[i], b.mMax[i]);
		}
	}
	BVH::BVH()
	{
	}

	int BVH::AddBox(const AABB& box)
	{
		return 0;
	}

	void BVH::RemoveBox(const int handle)
	{
	}

	void BVH::UpdateBox(const int handle, const AABB& box)
	{
	}

	Node* BVH::insert(const AABB& box, void* data)
	{
		Node* leaf = createNode(box, data, nullptr);
		insertLeaf(mRoot, leaf);
		mLeafNums += 1;
		return nullptr;
	}

	void BVH::insertLeaf(Node* root, Node* leaf)
	{
		if (mRoot == nullptr)
		{
			mRoot = leaf;
		}
		else
		{
			if (!root->isLeaf())
			{
				do
				{
					root = root->children[Select(leaf->box, root->children[0]->box, root->children[1]->box)];
				} while (!root->isLeaf());
			}
			Node* parent = root->parent;
			//Node* node = createNode(parent)
		}
	}

	Node* BVH::createNode(const AABB& box, void* data, Node* parent)
	{
		Node* node = nullptr;
		if (mFree)
		{
			node = mFree;
			mFree = nullptr;
		}
		else
		{
			node = new Node();
		}
		node->parent = parent;
		node->data = data;
		node->children[1] = nullptr;
		return node;
	}
	
	void BVH::deleteNode(Node* node)
	{
		if (mFree)
		{
			delete mFree;
		}
		mFree = node;
	}
}