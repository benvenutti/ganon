#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <type_traits>

template < class T,
           class Enabled = std::enable_if_t< std::is_move_assignable< T >::value && //
                                             std::is_move_constructible< T >::value > >
class SafeQueue
{

private:
    std::queue< T >           q;
    mutable std::shared_mutex m;
    // Very high max_size - default to std::numeric_limits< size_t >::max() - virtually no limit on size of the queue
    size_t                      max_size;
    bool                        push_over = false;
    std::condition_variable_any cv_push;
    std::condition_variable_any cv_pop;

public:
    SafeQueue()
    : max_size( std::numeric_limits< size_t >::max() )
    {
    }

    SafeQueue( size_t max )
    : max_size( max )
    {
    }

    void set_max_size( size_t max )
    {
        std::unique_lock lock( m );
        max_size = max;
    }

    void push( T t )
    {
        std::unique_lock lock( m );
        while ( q.size() >= max_size )
            cv_push.wait( lock );
        q.push( t );
        cv_pop.notify_one();
    }

    T pop()
    {
        std::unique_lock lock( m );
        while ( q.size() == 0 )
        {
            if ( push_over )
                return T();
            cv_pop.wait( lock );
        }
        T val = std::move( q.front() );
        q.pop();
        cv_push.notify_one();
        return val;
    }

    void notify_push_over()
    {
        std::unique_lock lock( m );
        push_over = true;
        cv_pop.notify_all();
    }

    int size() const
    {
        std::shared_lock lock( m );
        return q.size();
    }

    bool empty() const
    {
        std::shared_lock lock( m );
        return q.empty();
    }
};
