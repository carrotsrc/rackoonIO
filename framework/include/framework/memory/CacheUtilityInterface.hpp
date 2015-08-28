#ifndef __CACHEUTILITYINTERFACE_HPP_1440753748__
#define __CACHEUTILITYINTERFACE_HPP_1440753748__
#include "framework/fwcommon.hpp"
namespace StrangeIO {
namespace Memory {	

class cache_ptr; // Forward declaration
	
class CacheUtilityInterface {
public:
	virtual const PcmSample* alloc_raw(unsigned int num) = 0;
	virtual void free_raw(const PcmSample*) = 0;
};


}
}
#endif