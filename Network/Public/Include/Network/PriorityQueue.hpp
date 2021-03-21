#pragma once
#include <queue>

template <typename T, typename Comp = std::less<>>
class PriorityQueue : public std::priority_queue<T, std::vector<T>, Comp>
{
public:
    bool erase(const T& elem)
    {
        const auto it = std::find(this->c.begin(), this->c.end(), elem);

        if (it == this->c.end())
        {
            return false;
        }

        this->c.erase(it);
        std::make_heap(this->c.begin(), this->c.end(), Comp);

        return true;
    }

    bool update(const T& elem)
    {
        const auto it = std::find(this->c.begin(), this->c.end(), elem);

        if (it == this->c.end())
        {
            return false;
        }

        *it = elem;
        std::make_heap(this->c.begin(), this->c.end(), Comp);

        return true;
    }

    bool contain(const T& elem) const
    {
        const auto it = std::find(this->c.begin(), this->c.end(), elem);

        return it != this->c.end();
    }

    void clear() { this->c.clear(); }
};