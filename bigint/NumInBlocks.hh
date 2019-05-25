#ifndef NUMINBLOCKS_H
#define NUMINBLOCKS_H

// Make sure we have NULL.
#ifndef NULL
#define NULL 0
#endif

/* A NumInBlocks<Block> object holds a heap-allocated array of Block with a
 * length and a capacity and provides basic memory management features.
 * BigUnsigned and BigUnsignedInABase both subclass it.
 */
template <class Block>
class NumInBlocks {
public:

	// Type for the index of a block in the array
	typedef unsigned int Index;
	// The number of bits in a block, defined below.
	static const unsigned int N;

    // The current allocated capacity of this NumInBlocks (in blocks)
	Index cap;
    // The actual length of the value stored in this NumInBlocks (in blocks)
	Index len;
	// Heap-allocated array of the blocks (can be NULL if len == 0)
    Block *blk;

    // Constructs a 'zero' NumInBlocks with the given capacity.
    NumInBlocks(Index c) : cap(c), len(0) {
        blk = (cap > 0) ? (new Block[cap]) : NULL;
	}

    // Constructs a zero NumInBlocks without allocating a backing array.
    NumInBlocks() : cap(0), len(0) {
		blk = NULL;
	}

    ~NumInBlocks() {
		delete [] blk;
	}

    void allocate(Index c);
	void allocateAndCopy(Index c);

    NumInBlocks(const NumInBlocks<Block> &x);
    void operator=(const NumInBlocks<Block> &x);

	// Constructor that copies from a given array of blocks
    NumInBlocks(const Block *b, Index blen);

	// ACCESSORS
	Index getCapacity()     const { return cap;      }
	Index getLength()       const { return len;      }
    Block   getBlock(Index i) const { return blk[i];   }
	bool  isEmpty()         const { return len == 0; }

    bool operator ==(const NumInBlocks<Block> &x) const;

    bool operator !=(const NumInBlocks<Block> &x) const {
		return !operator ==(x);
	}
};

template <class Block>
const unsigned int NumInBlocks<Block>::N = 8 * sizeof(Block);

template <class Block>
void NumInBlocks<Block>::allocate(Index c) {
	// If the requested capacity is more than the current capacity...
	if (c > cap) {
		// Delete the old number array
		delete [] blk;
		// Allocate the new array
		cap = c;
        blk = new Block[cap];
	}
}

template <class Block>
void NumInBlocks<Block>::allocateAndCopy(Index c) {
	// If the requested capacity is more than the current capacity...
	if (c > cap) {
        Block *oldBlk = blk;
		// Allocate the new number array
		cap = c;
        blk = new Block[cap];
		// Copy number blocks
		Index i;
		for (i = 0; i < len; i++)
			blk[i] = oldBlk[i];
		// Delete the old array
		delete [] oldBlk;
	}
}

template <class Block>
NumInBlocks<Block>::NumInBlocks(const NumInBlocks<Block> &x)
		: len(x.len) {
	// Create array
	cap = len;
    blk = new Block[cap];
	// Copy blocks
	Index i;
	for (i = 0; i < len; i++)
		blk[i] = x.blk[i];
}

template <class Block>
NumInBlocks<Block>::NumInBlocks(const Block *b, Index blen)
        : cap(blen), len(blen) {
    // Create array
    blk = new Block[cap];
    // Copy blocks
    Index i;
    for (i = 0; i < len; i++)
        blk[i] = b[i];
}


template <class Block>
void NumInBlocks<Block>::operator=(const NumInBlocks<Block> &x) {
	/* Calls like a = a have no effect; catch them before the aliasing
	 * causes a problem */
	if (this == &x)
		return;
	// Copy length
	len = x.len;
	// Expand array if necessary
	allocate(len);
	// Copy number blocks
	Index i;
	for (i = 0; i < len; i++)
		blk[i] = x.blk[i];
}

template <class Block>
bool NumInBlocks<Block>::operator ==(const NumInBlocks<Block> &x) const {
	if (len != x.len)
		// Definitely unequal.
		return false;
	else {
		// Compare corresponding blocks one by one.
		Index i;
		for (i = 0; i < len; i++)
			if (blk[i] != x.blk[i])
				return false;
		// No blocks differed, so the objects are equal.
		return true;
	}
}

#endif
