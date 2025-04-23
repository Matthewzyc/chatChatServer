#include "ReactorPool.hpp"

ReactorPool::ReactorPool(int size) : _pool_size(size)
{
    _reactor_index = 0;
}

void ReactorPool::init()
{
    for (int i = 0; i < _pool_size; i++)
    {
        _reactor_pool.emplace_back(std::make_shared<SubReactor>());
    }
}

void ReactorPool::start()
{
    for (int i = 0; i < _pool_size; i++)
    {
        _reactor_pool[i]->start();
    }
}

std::shared_ptr<SubReactor> ReactorPool::getNextReactor()
{
    _reactor_index = (_reactor_index++) % _pool_size;
    std::shared_ptr<SubReactor> reactor = _reactor_pool[_reactor_index];
    return reactor;
}