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
#include <stdexcept>

#include "CFRaiiCommon.h"
#include "QCSharedPtr.h"

#include "QCString.h"
#include "QCURL.h"

BEGIN_QC_NAMESPACE

class QCArray_shared_ptr : public QCSharedPtr < CFArrayRef >
{
public:
	/* Just like with normal CoreFoundation functions,
	 * it is the user's responsibility to ensure that the array is valid
	 * before calling any of these methods.
	 */
	CFIndex GetCount() const
	{
		return CFArrayGetCount(get());
	}
	
	// copy the underlying array; follows the Create rule
	CFArrayRef Copy() const
	{
		CFArrayRef array = get();
		return (array != NULL)
		? CFArrayCreateCopy(CFGetAllocator(array), array)
		: NULL;
	}
	CFMutableArrayRef MutableCopy() const
	{
		CFArrayRef array = get();
		return (array != NULL)
		? CFArrayCreateMutableCopy(CFGetAllocator(array), 0, array) // do not enforce a size limit
		: NULL;
	}
	
	CFTypeRef operator [](CFIndex const index)
	{
		return CFArrayGetValueAtIndex(get(), index);
	}
	
	// no conversion operator to CFMutableArrayRef; it's a bad idea
};

class QCMutableArray : public QCSharedPtr < CFMutableArrayRef >
{
public:
#if 0
	explicit
	QCMutableArray(CFMutableArrayRef array)
	: QCSharedPtr < CFMutableArrayRef > ( array )
	{ }
#endif
	CFIndex GetCount() const
	{
		return CFArrayGetCount(get());
	}
	
	// copy the underlying array; follows the Create rule
	CFArrayRef Copy() const
	{
		CFMutableArrayRef array = get();
		return (array != NULL)
		? CFArrayCreateCopy(CFGetAllocator(array), array)
		: NULL;
	}
	CFMutableArrayRef MutableCopy() const
	{
		CFMutableArrayRef array = get();
		return (array != NULL)
		? CFArrayCreateMutableCopy(CFGetAllocator(array), 0, array) // do not enforce a size limit
		: NULL;
	}
	
	void AppendValue(CFTypeRef const value)
	{
		CFArrayAppendValue(get(), value);
	}
	void AppendArray(CFArrayRef const array)
	{
		CFArrayAppendArray(get(), array, CFRangeMake(0, CFArrayGetCount(array)));
	}
	
	// conversion operator
	operator QCArray_shared_ptr ()
	{
		// not a problem to add const to the opaque __CFArray *
		return static_cast<QCArray_shared_ptr> (*this);
	}
};

class QCArray1
{
	// class invariant: at most one of array and mArray may be non-NULL at a time
private:
	CFMutableArrayRef	mArray;
	CFArrayRef			array;
	
public:
	QCArray1( )
	: array( NULL )
	, mArray( CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks) )
	{ }
	
	explicit QCArray1(CFMutableArrayRef const &inArray)
	: array( NULL )
	, mArray( inArray )
	{ }
	
	explicit QCArray1(CFArrayRef const &inArray)
	: array( inArray )
	, mArray( NULL )
	{ }
	
	// copy constructor
	QCArray1(QCArray1 const &inArray)
	: array( QCRetain(inArray.array) )
	, mArray( QCRetain(inArray.mArray) )
	{ }
	
	// destructor
	~QCArray1( )
	{
		QCRelease(array);
		QCRelease(mArray);
	}
	
	CFArrayRef Array() const
	{
		return isNotNull(mArray) ? mArray : array;
	}
	
	CFArrayRef CFArray() const
	{
		return isNotNull(array) ? array : mArray;
	}
	
	// MARK: -
	// MARK: Helper Classes
	
	// MARK: class CFTypeProxy
	class CFTypeProxy
	{
	private:
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
			// could use CFEqual(array, rhs.array)
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
	}; // class CFTypeProxy
	
	// MARK: class const_iterator
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
		
		// prefix operators (must return by reference)
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
	}; // class const_iterator
	
	// MARK: class CFMutableTypeProxy
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
	}; // class CFMutableTypeProxy
	
	// MARK: class iterator
	class iterator
	{
	private:
		// data members
		CFMutableArrayRef	array;
		CFIndex				currentIndex;
		
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
	
	void makeMutable()
	{
		if (mArray == NULL)
		{
			if (array != NULL)
			{
				mArray = CFArrayCreateMutableCopy(kCFAllocatorDefault
												  , 0
												  , array);
				CFRelease(array);
				array = NULL;
			}
			else
			{
				mArray = CFArrayCreateMutable(kCFAllocatorDefault
											  , 0
											  , &kCFTypeArrayCallBacks);
			}
			
		}
	}
	
	void makeUnique()
	{
		// don't care if 'array' is shared, just 'mArray'
		if (!isNull(mArray) && CFGetRetainCount(mArray) > 1)
		{
			CFMutableArrayRef newArray = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, mArray);
			CFRelease(mArray);
			mArray = newArray;
		}
		return;
		
		if (! null() && CFGetRetainCount(Array()) > 1)
		{
			CFMutableArrayRef newArray = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, Array());
			QCRelease(Array());
			array = NULL;
			mArray = newArray;
		}
	}
	
	bool null() const
	{
		return isNull( Array() );
	}
	
	CFIndex GetCount() const
	{
		return null() ? 0 : CFArrayGetCount(Array());
	}
	
	bool empty() const
	{
		// either null or has 0 entries
		return GetCount() == 0;
	}
	
	QCArray1 copy() const
	{
		return QCArray1(*this);// null() ? QCArray1() : QCArray1(CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, array));
	}
	
	// Operators
	
	// copy assignment
	QCArray1 & operator = (QCArray1 const &rhs)
	{
		QCArray1 temp(rhs);
		std::swap(array, temp.array);
		std::swap(mArray, temp.mArray);
		return *this;
		
	}
	
	// comparison operators
	bool operator == (QCArray1 const &rhs) const
	{
		return (Array() == rhs.Array())
		|| (CFEqual(Array(), rhs.Array()) == true); // convert from Boolean
	}
	
	bool operator != (QCArray1 const &rhs) const
	{
		return !(*this == rhs);
	}
	
	// compound assignment -- concatenation
	QCArray1 & operator += (QCArray1 const &rhs)
	{
		// if rhs.array == NULL do nothing
		if (rhs.array != NULL)
		{
			makeMutable();
			makeUnique();
			if (null())
			{
				// TODO: determine if rhs.Array() is mutable
				// doc: "CFMutableArray objects have the same type identifier as CFArray objects."
				array = QCRetain( rhs.Array() );
			}
			else
			{
				CFArrayAppendArray(mArray, rhs, CFRangeMake(0, rhs.GetCount()));		
			}
		}
		return *this;
	}
	
	// CFArrayRef also includes CFMutableArrayRef
	QCArray1 & operator += (CFArrayRef const &rhs)
	{
		if (rhs != NULL)
		{
			makeMutable();
			makeUnique();
			if (null()) // neither array nor mArray
			{
				array = rhs;
			}
			else
			{
				CFArrayAppendArray(mArray, rhs, CFRangeMake(0, CFArrayGetCount(rhs)));
			}
		}
		return *this;
	}
	
	// conversion operators
	operator CFArrayRef () const
	{
		return Array();
	}
	
	CFMutableTypeProxy operator [] (CFIndex const idx) // not const because it has the potential of assignment
	{
		makeMutable();
		makeUnique();
		return CFMutableTypeProxy(mArray, idx);
	}
	
	CFTypeProxy operator [] (CFIndex const idx) const
	{
		return CFTypeProxy(Array(), idx);
	}
	
	void AppendValue(CFTypeRef const value)
	{
		// don't add non-value
		if (value != NULL)
		{
			makeMutable();
			makeUnique();
			if (null())
			{
				// create array if there isn't one yet
				mArray = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
			}
			CFArrayAppendValue(mArray, value);
		}
	}
	
	void AppendArray(CFArrayRef otherArray)
	{
		if (otherArray != NULL)
		{
			makeMutable();
			makeUnique();
			CFArrayAppendArray(mArray, otherArray, CFRangeMake(0, CFArrayGetCount(otherArray)));
		}
	}
		
	// throws out_of_range exception for invalid index
	void RemoveValueAtIndex(CFIndex const idx)
	{
		if (idx < GetCount()) // strict less-than because arrays are 0-indexed
		{
			makeMutable();
			makeUnique();
			CFArrayRemoveValueAtIndex(mArray, idx);
		}
		else
		{
			// else-case error reporting
			throw std::out_of_range(std::string("Removing value for invalid index."));
		}
	}
	
	void show() const;
	
	bool writeToFile(QCString const &filePath, CFPropertyListFormat const format) const;
	
	static QCArray1 arrayFromFile(QCString const &filePath);
	
	const_iterator begin() const
	{
		return const_iterator(Array(), 0);
	}
	
	const_iterator end() const
	{
		return const_iterator(Array(), GetCount());
	}
	
	iterator begin()
	{
		makeMutable();
		makeUnique();
		return iterator(mArray, 0);
	}
	
	iterator end()
	{
		makeMutable();
		makeUnique();
		return iterator(mArray, GetCount());
	}
	
	size_t size() const
	{
		return static_cast<size_t> ( null() ? 0u : CFArrayGetCount(Array()) );
	}
	
	CFTypeRef at(CFIndex const idx) const
	{
		return CFArrayGetValueAtIndex(Array(), idx);
	}
};

typedef QCArray_shared_ptr			QCArray;
typedef QCMutableArray				QCMutableArray;
END_QC_NAMESPACE

#endif
