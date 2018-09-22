#include <mutex>
#include <queue>
#include <type_traits>
#include <utility>

template <
  typename T,
  typename = std::enable_if_t< std::is_move_assignable< T >::value && std::is_move_constructible< T >::value >
>
class SafeQueue
{
public:
    void push( T t )
    {
        std::lock_guard<std::mutex> lock( m_mutex );
        m_queue.push( std::move( t ) );
    }

    T pop()
    {
        std::lock_guard<std::mutex> lock( m_mutex );
        if ( m_queue.empty() )
            return T();
        T val = std::move( m_queue.front() );
        m_queue.pop();
        return val;
    }

    int size() const
    {
        std::lock_guard<std::mutex> lock( m_mutex );
        return m_queue.size();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock( m_mutex );
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
};
