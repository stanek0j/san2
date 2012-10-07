
#include "rpc/cirpcfunction.hpp"

class TestFunc
{
public:
	unsigned int getUniqueId()const;
	bool operator()(void);
	
	bool pack(San2::Utils::bytes &out);
	bool unpack(const San2::Utils::bytes &in);
};
