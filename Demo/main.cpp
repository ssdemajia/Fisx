#include "FISX.h"
#include <atomic>
#include <iostream>
#include <thread>
#include <assert.h>
#include <mutex>
#include <condition_variable>
#include <string>

struct A {
	A* next;
};

std::atomic<A*> head = nullptr;
#ifdef _WIN32
#include <windows.h>
const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)


void SetThreadName(uint32_t dwThreadID, const char* threadName)
{

	// DWORD dwThreadID = ::GetThreadId( static_cast<HANDLE>( t.native_handle() ) );

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
void SetThreadName(const char* threadName)
{
	SetThreadName(GetCurrentThreadId(), threadName);
}

void SetThreadName(std::thread* thread, const char* threadName)
{
	DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread->native_handle()));
	SetThreadName(threadId, threadName);
}

#elif defined(__linux__)
#include <sys/prctl.h>
void SetThreadName(const char* threadName)
{
	prctl(PR_SET_NAME, threadName, 0, 0, 0);
}

#else
void SetThreadName(std::thread* thread, const char* threadName)
{
	auto handle = thread->native_handle();
	pthread_setname_np(handle, threadName);
}
#endif

class IEntity
{
public:
	__declspec(property(get = _GetTransform, put = _SetTransform))
		int _Transform;

	__forceinline int const& _GetTransform() const noexcept { 
		std::cout << "GetTransform\n";
		return _Transform; 
	}
	__forceinline void _SetTransform(int const& newValue) noexcept {
		std::cout << "_SetTransform\n";
		if (newValue != _TransformStorage)
		{
			_TransformStorage = newValue;
		}
		_OnTransformChange(this, newValue);
	}
	int _TransformStorage = 1;

private:
	static void _OnTransformChange(IEntity* entity, const int& transform) noexcept {}
};


template <typename T>
void foo()
{
	T i = T();
	std::cout << "i:" << i << std::endl;
}

template<typename T>
void ref(T const& x)
{
	std::cout << "ref " << typeid(x).name() << "\n";
}

template<typename T>
void nonref(T x)
{
	std::cout << "no ref " << typeid(x).name() << "\n";
}

int main(int argv, char** argc)
{
	/*Fisx::Vector3 v(1, 2, 3);
	Fisx::Stack<int> stack;
	stack.push(4);
	A* a = new A;
	a->next = head.load(std::memory_order_relaxed);
	A* b = new A;
	head = b;
	if (head.compare_exchange_weak(a->next, a, std::memory_order_release))
	{
		std::cout << a->next << std::endl;
	}*/
	size_t a = 0b111000;
	size_t mask = a & (~a + 1);

	unsigned long index = 0;
	unsigned long lMask = 0x0000ff00;
	_BitScanForward(&index, lMask);
	std::cout << index << std::endl;

	std::atomic<int> y = 0;
	std::atomic<int> x = 0;
	std::atomic<int> tindex = 0;
	std::vector<std::thread>  ts;
	std::mutex mutex;
	std::atomic<int> counter = 0;
	std::condition_variable cv;
	const int threadCount = 4;
	std::vector<int> v3, v4;
	for (int i = 0; i < threadCount; i++)
	{
		std::thread t1([&y, &x, &tindex, &counter, &cv, &mutex, &v3, &v4](int index) {
			if (index == 3)
			{
				for (int i = 0; i < 10; i += 2)
					y.store(i, std::memory_order_relaxed);
			}
			if (index == 2)
			{
				for (int i = 1; i < 10; i += 2)
					y.store(i, std::memory_order_relaxed);
			}
			if (index == 1)
			{
				for (int i = 0; i < 10; i++)
				{
					v3.push_back(y.load(std::memory_order_relaxed));
				}
			}
			if (index == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					v4.push_back(y.load(std::memory_order_relaxed));
				}
			}
			std::lock_guard<std::mutex> l{ mutex };
			counter += 1;
			cv.notify_all();
		}, i);
		std::string name = std::to_string(i);
		SetThreadName(&t1, name.c_str());
		//t1.detach();
		ts.push_back(std::move(t1));
	}
	

	{
		std::unique_lock lock(mutex);
		cv.wait(lock, [&counter, &threadCount]() {return counter == threadCount; });
	}
	std::cout << tindex << std::endl;
	for (int i = 0; i < threadCount; i++)
	{
		ts[i].join();
	}
	for (int i = 0; i < v3.size(); i++)
	{
		std::cout << v3[i] << " ";
	}
	std::cout << std::endl;
	for (int i : v4)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;

	IEntity entity;
	entity._Transform = 1;

	foo<int>();
	foo<double>();
	ref("123");
	nonref("123");
	return 0;
}