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

	inline static bool NotEqual(const AABB& a, const AABB& b)
	{
		return a.mMin.x != b.mMin.x &&
			a.mMin.y != b.mMin.y &&
			a.mMin.z != b.mMin.z &&
			a.mMax.x != b.mMax.x &&
			a.mMax.y != b.mMax.y &&
			a.mMax.z != b.mMax.z;
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

	void BVH::remove(Node* leaf)
	{
		removeLeaf(leaf);
		deleteNode(leaf);
		mLeafNums -= 1;
	}

	Node* BVH::insert(const AABB& box, void* data)
	{
		Node* leaf = createNode(nullptr, box, data);
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
			Node* node = createNode(parent, root->box, leaf->box, nullptr);
			if (parent)
			{
				parent->children[root->index()] = node;
				node->children[0] = root;
				root->parent = node;

				node->children[1] = leaf;
				leaf->parent = node;
				do
				{
					if (parent->box.Contain(node->box))
					{
						break;
					}
					Merge(parent->box, node->box, parent->box);
					node = parent;
				} while ((parent = node->parent) != nullptr);
			}
			else
			{
				node->children[0] = root;
				root->parent = node;

				node->children[1] = leaf;
				leaf->parent = node;

				mRoot = node;
			}
		}
	}

	Node* BVH::removeLeaf(Node* leaf)
	{
		if (leaf == mRoot)
		{
			mRoot = nullptr;
			return nullptr;
		}
		else
		{
			Node* parent = leaf->parent;
			Node* pp = parent->parent;
			Node* sibling = parent->children[1 - leaf->index()];
			if (pp)
			{
				pp->children[parent->index()] = sibling;
				sibling->parent = pp;
				deleteNode(parent);
				while (pp)
				{
					AABB box = pp->box;
					Merge(pp->children[0]->box, pp->children[1]->box, pp->box);
					if (NotEqual(box, pp->box))
					{
						pp = pp->parent;
					}
					else
					{
						break;
					}
				}
				return pp ? pp : mRoot;
			}
			else
			{
				mRoot = sibling;
				sibling->parent = nullptr;
				deleteNode(parent);
				return mRoot;
			}
		}
	}

	Node* BVH::createNode(Node* parent, void* data)
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
	
	Node* BVH::createNode(Node* parent, const AABB& box, void* data)
	{
		Node* node = createNode(parent, data);
		node->box = box;
		return node;
	}

	Node* BVH::createNode(Node* parent, const AABB& box0, const AABB& box1, void* data)
	{
		Node* node = createNode(parent, data);
		Merge(box0, box1, node->box);
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

	void BVH::update(Node* leaf, const AABB& box)
	{
		Node* root = removeLeaf(leaf);
		root = mRoot;
		leaf->box = box;
		insertLeaf(root, leaf);
	}
}