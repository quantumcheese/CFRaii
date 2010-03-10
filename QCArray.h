/*
 *  QCArray.h
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_ARRAY_GUARD_
#define _QC_ARRAY_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

#include "QCString.h"
#include "QCURL.h"

class QCArray
{
private:
	CFMutableArrayRef array;
	
	// provides error checking
	CFMutableArrayRef CFMutableArrayFromCFArray(CFArrayRef const &inArray) const
	{
		CFMutableArrayRef temp(NULL);
		if (inArray != NULL)
		{
			temp = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, inArray);
		}
		return temp;
	}
	
public:
	QCArray( )
	: array( CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks) )
	{ }
	
	QCArray(CFMutableArrayRef const &inArray)
	: array( inArray )
	{ }
	
	QCArray(CFArrayRef const &inArray)
	: array( CFMutableArrayFromCFArray(inArray) )
	{
		QCRelease(inArray);
	}
	
	// copy constructor
	QCArray(QCArray const &inArray)
	: array( QCRetain(inArray.array) )
	{ }
	
	// destructor
	~QCArray( )
	{
		QCRelease(array);
	}
	
	// helper classes
	
	class CFTypeProxy
	{
	private:
		// data members
		CFArrayRef		array;
		CFIndex			index;
		
		// no copy-assignment
		CFTypeProxy & operator = (CFTypeProxy const &);
		
	public:
		// ctor
		CFTypeProxy(CFArrayRef const arrayRef, CFIndex const idx)
		: array(QCRetain(arrayRef)), index(idx)
		{ }
		
		// copy ctor
		CFTypeProxy(CFTypeProxy const &proxy)
		: array(QCRetain(proxy.array)), index(proxy.index)
		{ }
		
		// dtor
		~CFTypeProxy()
		{
			QCRelease(array);
		}
		
		// comparison operators
		bool operator == (CFTypeProxy const &rhs) const
		{
			return array == rhs.array && index == rhs.index;
		}
		
		bool operator != (CFTypeProxy const &rhs) const
		{
			return !(*this == rhs);
		}
		
		// conversion operator
		operator CFTypeRef () const
		{
			return CFArrayGetValueAtIndex(array, index);
		}
	};
	
	class const_iterator
	{
	private:
		// data members
		CFArrayRef		array;
		CFIndex			currentIndex;
		
	public:
		// ctor
		const_iterator(CFArrayRef const arrayRef, CFIndex const idx)
		: array(QCRetain(array)), currentIndex(idx)
		{ }
		
		// copy ctor
		const_iterator(const_iterator const &iter)
		: array(QCRetain(iter.array)), currentIndex(iter.currentIndex)
		{ }
		
		// dtor
		~const_iterator()
		{
			QCRelease(array);
		}
		
		// copy-assignment
		const_iterator & operator = (const_iterator const &rhs)
		{
			QCRelease(array);
			
			array = QCRetain(rhs.array);
			currentIndex = rhs.currentIndex;
			
			return *this;
		}
		
		// prefix operators (must retrn by reference)
		const_iterator & operator ++ ()
		{
			++ currentIndex;
			return *this;
		}
		
		const_iterator & operator -- ()
		{
			-- currentIndex;
			return *this;
		}
		
		// postfix operators (must not return by reference)
		const_iterator operator ++ (int)
		{
			const_iterator temp(*this);
			this -> operator ++();
			return temp;
		}
		
		const_iterator operator -- (int)
		{
			const_iterator temp(*this);
			this -> operator --();
			return temp;
		}
		
		// comparison operators
		bool operator == (const_iterator const &rhs) const
		{
			return array == rhs.array && currentIndex == rhs.currentIndex;
		}
		
		bool operator != (const_iterator const &rhs) const
		{
			return !(*this == rhs);
		}		
		
		// dereference operator
		CFTypeProxy const operator * () const
		{
			return CFTypeProxy(array, currentIndex);
		}
		
	};
	
	class CFMutableTypeProxy
	{
	private:
		// data members
		CFMutableArrayRef		array;
		CFIndex					index;
		
		// no copy-assignment
		CFMutableTypeProxy & operator = (CFMutableTypeProxy const &);
		
	public:
		// ctor
		CFMutableTypeProxy(CFMutableArrayRef const arrayRef, CFIndex const idx)
		: array(QCRetain(arrayRef)), index(idx)
		{ }
		
		// copy ctor
		CFMutableTypeProxy(CFMutableTypeProxy const &proxy)
		: array(QCRetain(proxy.array)), index(proxy.index)
		{ }
		
		// dtor
		~CFMutableTypeProxy()
		{
			QCRelease(array);
		}
		
		// conversion operators
		operator CFTypeRef () const
		{
			return CFArrayGetValueAtIndex(array, index);
		}
		
		operator CFTypeProxy () const
		{
			return CFTypeProxy(array, index);
		}
		
		// assignment replaces an element in the array
		CFMutableTypeProxy & operator = (CFTypeRef const rhs)
		{
			// 'this' doesn't change, but the pointed-to array does
			CFArraySetValueAtIndex(array, index, rhs);
			return *this;
		}
	};
	
	class iterator
	{
	private:
		// data members
		CFMutableArrayRef	array;
		CFIndex				currentIndex;
		friend class		QCArray; // for QCArray::insert()
		
		
	public:
		// ctor
		iterator(CFMutableArrayRef const arrayRef, CFIndex const idx)
		: array(QCRetain(arrayRef)), currentIndex(idx)
		{ }
		
		// copy ctor
		iterator(iterator const &iter)
		: array(QCRetain(iter.array)), currentIndex(iter.currentIndex)
		{ }
		
		// dtor
		~iterator()
		{
			QCRelease(array);
		}
		
		// copy-assignment
		iterator & operator = (iterator const &rhs)
		{
			QCRelease(array);
			
			array = QCRetain(rhs.array);
			currentIndex = rhs.currentIndex;
			
			return *this;
		}
		
		// prefix operators (must return by reference)
		iterator & operator ++ ()
		{
			++ currentIndex;
			return *this;
		}
		
		iterator & operator -- ()
		{
			-- currentIndex;
			return *this;
		}
		
		// postfix operators (must not return by reference)
		iterator operator ++ (int)
		{
			iterator temp(*this);
			this -> operator ++();
			return temp;
		}
		
		iterator operator -- (int)
		{
			iterator temp(*this);
			this -> operator --();
			return temp;
		}
		
		// comparison operators
		bool operator == (iterator const &rhs) const
		{
			return array == rhs.array && currentIndex == rhs.currentIndex;
		}
		
		bool operator != (iterator const &rhs) const
		{
			return !(*this == rhs);
		}
		
		// dereference operator
		CFMutableTypeProxy operator * () const
		{
			return CFMutableTypeProxy(array, currentIndex);
		}
		
		/* // doesn't apply
		 CFMutableTypeProxy operator -> () const
		 {
		 return CFMutableTypeProxy(array, currentIndex);
		 }
		 */
		
		iterator & operator += (CFIndex const arg)
		{
			currentIndex += arg;
			return *this;
		}
		
		iterator operator + (CFIndex const arg)
		{
			return iterator(array, currentIndex + arg);
			iterator temp(*this);
			return temp += arg;
		}
		
		// operator [] must be a non-static member function
		// and we use operator + in it, so we made operator + a member function rather than a free one
		CFMutableTypeProxy operator [] (CFIndex const arg)
		{
			return * (this -> operator + (arg));
		}
		
		// conversion operator
		
		operator const_iterator () const
		{
			return const_iterator(array, currentIndex);
		}
		
	}; // class iterator
	
	
	void makeUnique()
	{
		if (! null() && CFGetRetainCount(array) > 1)
		{
			CFMutableArrayRef newArray = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, array);
			QCRelease(array);
			array = newArray;
		}
	}
	
	bool null() const
	{
		return array == 0;
	}
	
	CFIndex count() const
	{
		return null() ? 0 : CFArrayGetCount(array);
	}
	
	bool empty() const
	{
		// either null or has 0 entries
		return count() == 0;
	}
	
	QCArray copy() const
	{
		return null() ? QCArray() : QCArray(CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, array));
	}
	
	// Operators

	// copy assignment
	QCArray & operator = (QCArray const &rhs)
	{
		QCArray temp(rhs);
		std::swap(array, temp.array);
		return *this;

	}
	
	// comparison operators
	bool operator == (QCArray const &rhs) const
	{
		return (array == rhs.array)
				|| (CFEqual(array, rhs.array) == true); // convert from Boolean
	}
	
	bool operator != (QCArray const &rhs) const
	{
		return !(*this == rhs);
	}
	
	// compound assignment -- concatenation
	QCArray & operator += (QCArray const &rhs)
	{
		// if rhs.array == 0, do nothing
		if (rhs.array != 0)
		{
			makeUnique();
			if (null())
			{
				CFRetain(rhs.array);
				array = rhs.array;
			}
			else
			{
				CFArrayAppendArray(array, rhs, CFRangeMake(0, rhs.count()));		
			}
		}
		return *this;
	}
	
	// CFArrayRef also includes CFMutableArrayRef
	QCArray & operator += (CFArrayRef const &rhs)
	{
		if (rhs != 0)
		{
			makeUnique();
			QCArray temp(rhs);
			if (null())
			{
				array = temp;
			}
			else
			{
				CFArrayAppendArray(array, temp, CFRangeMake(0, temp.count()));
			}
		}
		return *this;
	}
	
	// conversion operators
	operator CFMutableArrayRef ()
	{
		return array;
	}
	
	operator CFArrayRef () const
	{
		return array;
	}
	
	CFMutableTypeProxy operator [] (CFIndex const idx) const
	{
		return CFMutableTypeProxy(array, idx);
	}
		
	void push_back(CFTypeRef const value)
	{
		if (value == 0)
		{
			// don't add non-value
			return;
		}
		makeUnique();
		if (null())
		{
			// create array if there isn't one yet
			array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
		}
		CFArrayAppendValue(array, value);
	}
	
	void appendArray(CFArrayRef otherArray)
	{
		if (otherArray != NULL)
		{
			makeUnique();
			CFArrayAppendArray(array, otherArray, CFRangeMake(0, CFArrayGetCount(otherArray)));
		}
	}
	
	void append(CFTypeRef value)
	{
		if (value != NULL)
		{
			// makeUnique is called in push_back
			push_back(value);
		}
	}
	
	void removeValueAtIndex(CFIndex const idx)
	{
		makeUnique();
		if (idx < count()) // strict less-than because arrays are 0-indexed
		{
			CFArrayRemoveValueAtIndex(array, idx);
		}
		// else-case error reporting?
	}
	
	void show() const;
	
	bool writeToFile(QCString const &filePath, CFPropertyListFormat const format) const;
	
	static QCArray arrayFromFile(QCString const &filePath);
	
	const_iterator begin() const
	{
		return const_iterator(array, 0);
	}
	
	const_iterator end() const
	{
		return const_iterator(array, count());
	}
	
	iterator begin()
	{
		return iterator(array, 0);
	}
	
	iterator end()
	{
		return iterator(array, count());
	}
	
	size_t size() const
	{
		return static_cast<size_t> ( null() ? 0u : CFArrayGetCount(array) );
	}
	
	iterator insert(iterator where, CFTypeRef const val)
	{
		CFArrayInsertValueAtIndex(array, where.currentIndex, val);
		return iterator(array, where.currentIndex);
	}
	
};

inline QCArray operator + (QCArray &lhs, QCArray const &rhs)
{
	QCArray temp(lhs);
	return temp += rhs;
}

inline QCArray operator + (QCArray &lhs, CFMutableArrayRef const &rhs)
{
	QCArray temp(lhs);
	return temp += rhs;
}

inline QCArray operator + (QCArray &lhs, CFArrayRef const &rhs)
{
	QCArray temp(lhs);
	return temp += rhs;
}

typedef QCArray const QCFixedArray;

#endif
