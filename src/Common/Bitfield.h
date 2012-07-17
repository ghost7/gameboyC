#ifndef _BIT_FIELD_H_
#define _BIT_FIELD_H_

#include <stdint.h>

/*
 * This structure can be used in conjunction with a union to manipulate the bits
 * of value.
 *
 * When using the Bitfield struct it's important to have the "base" variable 
 * in the union. 
 * For example: Here is is "header"
 * union
 * {
 *      uint32_t header;
 *      Bitfield<0, 29> magic;      //<! the bottom 29 bits of "header"
 *      Bitfield<29, 3> bbt_flags;  //<! the top 3 bits of "header"
 * }; 
 *
 * @tparam lower index of the bits the manipulate.
 * @tparam numBits number of bits that will be afffected.
 * @tparam T "base" type.
 */
template<unsigned index, unsigned numBits = 1, typename T = uint32_t>
struct Bitfield
{
	T data;
	static T const mask = (1ull << numBits) - 1ull;
	
	template<typename T2>
	Bitfield &operator=(T2 rhs)
	{
		data = (data & ~(mask << index)) | ((rhs & mask) << index);
		return *this;
	}

	operator T() const { return (data >> index) & mask; }
};

#endif
