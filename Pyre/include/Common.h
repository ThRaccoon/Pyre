#pragma once

#include <cstdint>

namespace pyre
{
#ifdef PYRE_USE_64BIT_HANDLES
	using EntityHandle = uint64_t;
#else
	using EntityHandle = uint32_t;
#endif
}
