#ifndef __POOL_UTILITY_HPP_1441205447__
#define __POOL_UTILITY_HPP_1441205447__
#include "framework/thread/thread.hpp"
namespace strangeio {
namespace thread {

class queue_utility {
public:
	virtual void add_package(pkg_task task) = 0;
};

} // thread
} // strangeio
#endif