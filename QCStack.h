/*
 *  QCStack.h
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 *
 * Note that nothing here is thread-safe.
 */

#ifndef _QC_STACK_GUARD_
#define _QC_STACK_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <exception>
#include <stdexcept>
#include "CFRaiiCommon.h"

class QCStack
{
private:
	CFMutableArrayRef stack;
	CFIndex stackCount; // we could just call CFArrayGetLength, but this seems easier
	size_t *refCount;
	
	// non-assignable (for now)
	QCStack & operator = (QCStack &);
	
public:
	// default constructor
	QCStack(CFAllocatorRef allocator = kCFAllocatorDefault, CFIndex capacity = 0)
	: stack( CFArrayCreateMutable(allocator, capacity, &kCFTypeArrayCallBacks) )
	, stackCount( 0 )
	, refCount( new size_t(1) )
	{
		// ensure valid stack
		if (stack == NULL)
		{
			throw std::bad_alloc();
		}
	}
	
	// copy constructor
	QCStack(QCStack const &inStack)
	: stack( inStack.stack )
	, stackCount( inStack.stackCount )
	, refCount( inStack.refCount ) // pointer to shared value
	{
		if (stack == NULL) // copying a malformed stack
		{
			throw std::bad_alloc();
		}
		// reference counting
		incrementRefCount();
	}
	
	// destructor
	~QCStack()
	{
		// lock
		decrementRefCount();
		if (*refCount == 0)
		{
			// last one out please close and lock the door
			QCRelease(stack);
			delete refCount;
			// unlock and delete the mutex
		}
		else
		{
			// unlock the mutex
		}
	}
	
	// reference counting
	
	inline void incrementRefCount()
	{
		// lock
		++ (*refCount);
		// unlock
	}
	inline void decrementRefCount()
	{
		// lock
		-- (*refCount);
		// unlock
	}
	
	void makeUnique()
	{
		// lock
		// ensure that refCount isn't changing while we're inspecting / modifying it
		if (*refCount > 1)
		{
			CFMutableArrayRef newStack = CFArrayCreateMutableCopy(CFGetAllocator(stack) // use the same allocator
																  , 0					// is there a way to get the array's capacity?
																  , stack);
			decrementRefCount();
			refCount = new size_t(1);
			stack = newStack;
			// create new mutex
		}
		// unlock
	}
	
	// stack operations
	
	void push(CFTypeRef const obj)
	{
		makeUnique();
		CFArraySetValueAtIndex(stack, stackCount++, obj);
	}
	
	// returns a value that requires releasing
	CFTypeRef pop()
	{
		if (stackCount > 0)
		{
			makeUnique();
			// lock
			--stackCount;
			// retain item to offset its removal
			CFTypeRef poppedItem = CFRetain( CFArrayGetValueAtIndex(stack, stackCount) );
			CFArrayRemoveValueAtIndex(stack, stackCount);
			// unlock
			return poppedItem;
		}
		// popping from empty stack
		throw std::range_error(std::string("popping empty stack"));
		return NULL;
	}
	
	CFTypeRef peek() const
	{
		if (stackCount <= 0)
		{
			throw std::range_error(std::string("peeking at empty stack"));
		}
		return CFArrayGetValueAtIndex(stack, stackCount - 1);
	}
	
	inline CFIndex count() const
	{
		return stackCount;
	}
	
	inline bool empty() const
	{
		return stackCount == 0;
	}
	
	void erase()
	{
		/* naive method:
		while (!empty())
		{
			CFRelease(pop());
		}
		 */
		CFIndex i = stackCount;
		while (i != 0)
		{
			CFArrayRemoveValueAtIndex(stack, --i);
		}
		stackCount = 0;
	}
	
	CFStringRef concatenateStringsWithJoiningString(CFStringRef const joiningString) const;
	void show() const;
};

#endif
