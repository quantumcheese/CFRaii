/*
 *  QCArray.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * The Software shall not be incorporated in any way into any work licensed
 * under the GPL version 3.0 or greater.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
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
	
	
	
	
	class CFTypeProxy
	{
	private:
		// data members
		CFMutableArrayRef		array;
		CFIndex					index;
		
		
		// no copy-assignment
		CFTypeProxy & operator = (CFTypeProxy const &);
		
	public:
		CFTypeProxy(CFMutableArrayRef const arrayRef, CFIndex const idx)
		: array(QCRetain(arrayRef)), index(idx)
		{ }
		
		CFTypeProxy(CFTypeProxy const &proxy)
		: array(QCRetain(proxy.array)), index(proxy.index)
		{ }
		
		~CFTypeProxy()
		{
			QCRelease(array);
		}
		
		// conversion operator
		operator CFTypeRef () const
		{
			return CFArrayGetValueAtIndex(array, index);
		}
		
		CFTypeProxy & operator = (CFTypeRef const rhs)
		{
			// 'this' doesn't actually change
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
		
	public:
		iterator(CFMutableArrayRef const arrayRef, CFIndex const idx)
		: array(QCRetain(arrayRef)), currentIndex(idx)
		{ }
		
		iterator(iterator const &it)
		: array(QCRetain(it.array)), currentIndex(it.currentIndex)
		{ }
		
		~iterator()
		{
			QCRelease(array);
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
		CFTypeProxy operator * () const
		{
			return CFTypeProxy(array, currentIndex);
		}
		
		/* // doesn't apply
		 CFTypeProxy operator -> () const
		 {
		 return CFTypeProxy(array, currentIndex);
		 }
		 */
		
		iterator operator + (CFIndex const arg)
		{
			return iterator(array, currentIndex + arg);
		}
		
		CFTypeProxy operator [] (CFIndex const arg)
		{
			return * (this -> operator + (arg));
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
	
	// ownership assignment
	QCArray & operator = (CFArrayRef const rhs)
	{
		if (rhs != array)
		{
			CFMutableArrayRef temp = CFMutableArrayFromCFArray(rhs);
			QCRelease(array);
			array = temp;
		}
		return *this;
	}
	
	QCArray & operator = (CFMutableArrayRef const rhs)
	{
		// avoid self-assignment
		if (rhs != array)
		{
			QCRelease(array);
			array = rhs;
		}
		return *this;
	}
	
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
				|| (CFEqual(array, rhs.array) == true);
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
	
	QCArray & operator += (CFMutableArrayRef const &rhs)
	{
		if (rhs != 0)
		{
			makeUnique();
			if (null())
			{
				CFRetain(rhs);
				array = rhs;
			}
			else
			{
				CFArrayAppendArray(array, rhs, CFRangeMake(0, CFArrayGetCount(rhs)));
			}
		}
		return *this;
	}
	
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
	
	CFTypeProxy operator [] (CFIndex const idx) const
	{
		return CFTypeProxy(array, idx);
	}
		
	void push_back(CFTypeRef value)
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
	
};

inline QCArray operator + (QCArray &lhs, QCArray const &rhs)
{
	QCArray temp(lhs);
	temp += rhs;
	return temp;
}

inline QCArray operator + (QCArray &lhs, CFMutableArrayRef const &rhs)
{
	QCArray temp(lhs);
	temp += rhs;
	return temp;
}

inline QCArray operator + (QCArray &lhs, CFArrayRef const &rhs)
{
	QCArray temp(lhs);
	temp += rhs;
	return temp;
}

typedef QCArray const QCFixedArray;

#endif
