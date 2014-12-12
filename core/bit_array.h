#ifndef __CORE__BIT_ARRAY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__BIT_ARRAY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>


/* Classes */

#ifdef _MSC_VER
 #pragma warning(disable:4804)		// Unsafe use of type 'bool' in operation
 #pragma warning(disable:4800)		// Forcing value to bool 'true' or 'false' (performance warning)
#endif

/** Array of bit blocks. */
template <const int NUM_BLOCKS, class TYPE = bool, const int BLOCK_SIZE = 1> class BitArray
{
public:
	enum
	{
		SizeInBytes = (NUM_BLOCKS * BLOCK_SIZE + 7) / 8, 	/**< Size of the array in bytes. */
		BlockMask = (1 << BLOCK_SIZE) - 1,					/**< Bit mask for the single block. */
	};

	/** Helper class for read/write access. */
	class ReadWriteAccessor
	{
	public:
		/**
		 * Constructor.
		 * @param arr Reference to the bit array.
		 * @param idx Index of the item.
		 */
		inline ReadWriteAccessor(BitArray & arr, uint idx)
			: m_Array(arr), m_Index(idx)
		{
		}

		/** Returns value of the item. */
		inline operator TYPE() const
		{
			return m_Array.at(m_Index);
		}

		/**
		 * Sets value of the item.
		 * @param value New value of the item.
		 */
		inline ReadWriteAccessor & operator=(TYPE value)
		{
			m_Array.set(m_Index, value);
			return *this;
		}

		/**
		 * Comparison operator.
		 * @param val Value to compare with.
		 */
		inline bool operator==(TYPE value) const
		{
			return m_Array.at(m_Index) == value;
		}

		/**
		 * Comparison operator.
		 * @param val Value to compare with.
		 */
		inline bool operator!=(TYPE value) const
		{
			return m_Array.at(m_Index) != value;
		}

		/**
		 * Comparison operator.
		 * @param val Value to compare with.
		 */
		inline bool operator<(TYPE value) const
		{
			return m_Array.at(m_Index) < value;
		}

		/**
		 * Comparison operator.
		 * @param val Value to compare with.
		 */
		inline bool operator<=(TYPE value) const
		{
			return m_Array.at(m_Index) <= value;
		}

		/**
		 * Comparison operator.
		 * @param val Value to compare with.
		 */
		inline bool operator>(TYPE value) const
		{
			return m_Array.at(m_Index) > value;
		}

		/**
		 * Comparison operator.
		 * @param val Value to compare with.
		 */
		inline bool operator>=(TYPE value) const
		{
			return m_Array.at(m_Index) >= value;
		}

		/**
		 * Addition operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator+=(TYPE value)
		{
			m_Array.set(m_Index, (TYPE)((m_Array.at(m_Index) + value) & BitArray::BlockMask));
			return *this;
		}

		/**
		 * Subtraction operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator-=(TYPE value)
		{
			m_Array.set(m_Index, (TYPE)((m_Array.at(m_Index) - value) & BitArray::BlockMask));
			return *this;
		}

		/**
		 * Multiplication operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator*=(TYPE value)
		{
			m_Array.set(m_Index, (TYPE)((m_Array.at(m_Index) * value) & BitArray::BlockMask));
			return *this;
		}

		/**
		 * Division operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator/=(TYPE value)
		{
			m_Array.set(m_Index, (TYPE)((m_Array.at(m_Index) / value) & BitArray::BlockMask));
			return *this;
		}

		/**
		 * Bit AND operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator&=(TYPE value)
		{
			m_Array.set(m_Index, (TYPE)(m_Array.at(m_Index) & value));
			return *this;
		}

		/**
		 * Bit OR operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator|=(TYPE value)
		{
			Q_ASSERT(value <= BitArray::BlockMask);
			m_Array.set(m_Index, (TYPE)(m_Array.at(m_Index) | value));
			return *this;
		}

		/**
		 * Bit XOR operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator^=(TYPE value)
		{
			Q_ASSERT(value <= BitArray::BlockMask);
			m_Array.set(m_Index, (TYPE)(m_Array.at(m_Index) ^ value));
			return *this;
		}

		/**
		 * Shift left operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator<<=(TYPE value)
		{
			m_Array.set(m_Index, (TYPE)((m_Array.at(m_Index) << value) & BitArray::BlockMask));
			return *this;
		}

		/**
		 * Shift right operator.
		 * @param val Second argument.
		 */
		inline ReadWriteAccessor & operator>>=(TYPE value)
		{
			m_Array.set(m_Index, (TYPE)((m_Array.at(m_Index) >> value) & BitArray::BlockMask));
			return *this;
		}

		/** Negation operator. */
		inline TYPE operator-() const
		{
			return -m_Array.at(m_Index);
		}

		/** Boolean NOT operator. */
		inline TYPE operator!() const
		{
			return !m_Array.at(m_Index);
		}

		/** Bitwise NOT operator. */
		inline TYPE operator~() const
		{
			return ~m_Array.at(m_Index);
		}

		/** Pre-increment operator. */
		inline TYPE operator++()
		{
			TYPE value = m_Array.at(m_Index);
			TYPE value2 = (++value & BitArray::BlockMask);
			m_Array.set(m_Index, value2);
			return value2;
		}

		/** Pre-decrement operator. */
		inline TYPE operator--()
		{
			TYPE value = m_Array.at(m_Index);
			TYPE value2 = (--value & BitArray::BlockMask);
			m_Array.set(m_Index, value2);
			return value2;
		}

		/** Post-increment operator. */
		inline TYPE operator++(int)
		{
			TYPE value = m_Array.at(m_Index);
			TYPE value2 = value++;
			m_Array.set(m_Index, (value & BitArray::BlockMask));
			return value2;
		}

		/** Pre-decrement operator. */
		inline TYPE operator--(int)
		{
			TYPE value = m_Array.at(m_Index);
			TYPE value2 = value--;
			m_Array.set(m_Index, (value & BitArray::BlockMask));
			return value2;
		}

	private:
		BitArray & m_Array;	/**< Reference to the bit array. */
		uint m_Index;		/**< Index of the item. */	
	};

	/**
	 * Returns value of the specified item.
	 * @param idx Index of the item.
	 */
	inline TYPE at(uint idx) const
	{
		Q_ASSERT(BLOCK_SIZE < 8);
		Q_ASSERT(idx < NUM_BLOCKS);

		register uint byteOffset;
		register uint bitOffset;

		switch (BLOCK_SIZE)
		{
		case 1:
			byteOffset = idx >> 3;
			bitOffset = idx & 7;
			return (TYPE)((m_Data[byteOffset] >> bitOffset) & BlockMask);

		case 2:
			byteOffset = idx >> 2;
			bitOffset = (idx & 3) << 1;
			return (TYPE)((m_Data[byteOffset] >> bitOffset) & BlockMask);

		case 4:	
			byteOffset = idx >> 1;
			bitOffset = (idx & 1) << 2;
			return (TYPE)((m_Data[byteOffset] >> bitOffset) & BlockMask);

		default:
			byteOffset = (idx * BLOCK_SIZE) >> 3;
			bitOffset = (idx * BLOCK_SIZE) & 7;

			if (bitOffset <= 8 - BLOCK_SIZE)
				return (TYPE)((m_Data[byteOffset] >> bitOffset) & BlockMask);
			else
			{
				uint8 part1 = m_Data[byteOffset++] >> bitOffset;
				uint8 part2 = m_Data[byteOffset] & ((1 << (bitOffset + BLOCK_SIZE - 8)) - 1);

				return (TYPE)(part1 | (part2 << (8 - bitOffset)));
			}
		}
	}

	/**
	 * Sets value of the specified item.
	 * @param idx Index of the item.
	 * @param val Value of the item.
	 */
	inline void set(uint idx, TYPE val)
	{
		Q_ASSERT(BLOCK_SIZE < 8);
		Q_ASSERT(idx < NUM_BLOCKS);
		Q_ASSERT(val <= (TYPE)BlockMask);

		register uint8 value = (uint8)val & BlockMask;
		register uint byteOffset;
		register uint bitOffset;

		switch (BLOCK_SIZE)
		{
		case 1:
			byteOffset = idx >> 3;
			bitOffset = idx & 7;
			m_Data[byteOffset] &= ~(BlockMask << bitOffset);
			m_Data[byteOffset] |= value << bitOffset;
			return;

		case 2:
			byteOffset = idx >> 2;
			bitOffset = (idx & 3) << 1;
			m_Data[byteOffset] &= ~(BlockMask << bitOffset);
			m_Data[byteOffset] |= value << bitOffset;
			return;

		case 4:	
			byteOffset = idx >> 1;
			bitOffset = (idx & 1) << 2;
			m_Data[byteOffset] &= ~(BlockMask << bitOffset);
			m_Data[byteOffset] |= value << bitOffset;
			return;

		default:
			byteOffset = (idx * BLOCK_SIZE) >> 3;
			bitOffset = (idx * BLOCK_SIZE) & 7;

			if (bitOffset <= 8 - BLOCK_SIZE)
			{
				m_Data[byteOffset] &= ~(BlockMask << bitOffset);
				m_Data[byteOffset] |= value << bitOffset;
			}
			else
			{
				m_Data[byteOffset] &= ~((BlockMask << bitOffset) & 0xFF);
				m_Data[byteOffset++] |= (value << bitOffset) & 0xFF;

				const uint mask = ((1 << (bitOffset + BLOCK_SIZE - 8)) - 1);
				m_Data[byteOffset] &= ~mask;
				m_Data[byteOffset] |= (value >> (8 - bitOffset)) & mask;
			}
			return;
		}
	}

	/**
	 * Provides read-only access to the specified item.
	 * @param idx Index of the item.
	 */
	inline TYPE operator[](uint idx) const
	{
		return at(idx);
	}

	/**
	 * Provides read/write access to the specified item.
	 * @param idx Index of the item.
	 */
	inline ReadWriteAccessor operator[](uint idx)
	{
		return ReadWriteAccessor(*this, idx);
	}

private:
	uint8 m_Data[SizeInBytes];
};

#ifdef _MSC_VER
 #pragma warning(default:4804)
 #pragma warning(default:4800)
#endif

#endif // __CORE__BIT_ARRAY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
