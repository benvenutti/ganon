#include <mutex>
#include <queue>

template <class T>
class SafeQueue{
    public:
        void push(T t){
            std::lock_guard<std::mutex> lock(m);
            q.push(t);
        }

        T pop(){
            std::lock_guard<std::mutex> lock(m);
            if ( q.empty() )
                return T();
            T val = q.front();
            q.pop();
            return val;
        }

        int size() const
        {
            std::lock_guard<std::mutex> lock(m);
            return q.size();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m);
            return q.empty();
        }

    private:
        std::queue<T> q;
        mutable std::mutex m;
};