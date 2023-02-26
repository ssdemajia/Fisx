#ifndef _FISX_TYPE_H
#define _FISX_TYPE_H

#include <vector>
#include <stdexcept>

namespace Fisx
{

    template<typename T, typename CONT=std::vector<T>>
    class Stack
    {
    public:
        void push(T const&);
        T pop();
        T top() const;
        bool empty() const { return }
    private:
        CONT mElems;
    };

    template<typename T, typename CONT>
    void Stack<T, CONT>::push(T const& elem)
    {
        mElems.push_back(elem);
    }

    template<typename T, typename CONT>
    T Stack<T, CONT>::pop()
    {
        if (mElems.empty())
        {
            throw std::out_of_range("Stack<>::pop() empty stack");
        }
        T e = mElems.back();
        mElems.pop_back();
        return e;
    }

    template<typename T, typename CONT>
    T Stack<T, CONT>::top() const
    {
        if (mElems.empty())
        {
            throw std::out_of_range("Stack<>::pop() empty stack");
        }
        return mElems.back();
    }


}
#endif