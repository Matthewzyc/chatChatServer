#ifndef _REACTOR_POOL_H_
#define _REACTOR_POOL_H_

#include <vector>
#include <thread>
#include <memory>

#include "SubReactor.hpp"

class ReactorPool
{
public:
    ReactorPool(int size = 2);

    void setPoolSize(int size) { _pool_size = size; }
    int getPoolSize() const { return _pool_size; }
    void init();
    void start();
    std::shared_ptr<SubReactor> getNextReactor();
private:
    int _pool_size;
    int _reactor_index;
    std::vector<std::shared_ptr<SubReactor>> _reactor_pool;
};

#endif