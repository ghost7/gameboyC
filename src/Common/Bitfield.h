#ifndef _BIT_FIELDS_H_
#define _BIT_FIELDS_H_

#include <stdint.h>
#include <stdio.h>

template<unsigned index, unsigned num_bits = 1, typename T = uint32_t>
struct Bitfield
{
	T data;
	static T const mask = (1ull << num_bits) - 1ull;
	
	template<typename T2>
	Bitfield &operator=(T2 rhs)
	{
		data = (data & ~(mask << index)) | (((num_bits > 1) ? rhs & mask : !!data) << index);
		return *this;
	}

	operator T() const { return (data >> index) & mask; }
};

#endif
