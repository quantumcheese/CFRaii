/*
 *  QCUtilities.h
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */


/*
 * Copyright (c) 2000-2004 Apple Computer, Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */


#ifndef _QC_UTILITIES_GUARD_
#define _QC_UTILITIES_GUARD_

#include <tr1/type_traits>
#include <stdexcept>
#include <stdio.h>

#include "QCMacrosInternal.h"

BEGIN_QC_NAMESPACE

class CFRaiiException
{
	CFTypeID _expectedID, _actualID;
protected:
	char *_description;
public:
	CFRaiiException(CFTypeID const &expected, CFTypeID const &actual)
	: _expectedID(expected), _actualID(actual), _description(NULL)
	{ }
	
	virtual char const *what()
	{
		if (_description == NULL)
		{
			(void)asprintf(&_description, "actual type id was %lu; expected %lu", _actualID, _expectedID);
		}
		return _description;
	}
	
	virtual ~CFRaiiException()
	{
		if (_description != NULL)
		{
			free(_description);
		}
	}
};


//
// Traits of popular CF types
//
template <class CFType>
class CFTraits : public std::tr1::false_type
{ };

template <>
class CFTraits<CFTypeRef> : public std::tr1::true_type
{ };

#define __CFType(name) \
template <> class CFTraits<name##Ref> : public std::tr1::true_type\
{ \
public:  \
	static CFTypeID cfid() { return name##GetTypeID(); } \
	static CFTypeID GetTypeID(name##Ref ref) { return CFGetTypeID(ref); }\
	static bool check(name##Ref ref) { return GetTypeID(ref) == cfid(); } \
};

__CFType(CFNull)
__CFType(CFBoolean)
__CFType(CFNumber)
__CFType(CFString)
__CFType(CFData)
__CFType(CFURL)
__CFType(CFBundle)
__CFType(CFArray)
__CFType(CFDictionary)
__CFType(CFSet)



//
// Initialize-only self-releasing CF object handler (lightweight).
//
template <class CFType> class CFRef
{
public:
    CFRef() : mRef(NULL) { }
    CFRef(CFType ref) : mRef(ref) { }
    ~CFRef() { this->release(); }
	CFRef(const CFRef &ref) : mRef(ref) {}
	template <class _T> CFRef(const CFRef<_T> &ref) : mRef(ref) {}
	
	CFRef(CFTypeRef ref)
	: mRef(CFType(ref))
	{
		if (ref != NULL && !CFTraits<CFType>::check(ref))
		{
			throw CFRaiiException(CFTraits<CFType>::GetTypeID(ref), CFTraits<CFType>::cfid());
		}
	}
	
	CFRef &take(CFType ref)
	{ this->release(); mRef = ref; return *this; }
	
	CFType yield()
	{ CFType r = mRef; mRef = NULL; return r; }
	
    CFRef &operator = (CFType ref)
    { if (ref) CFRetain(ref); return take(ref); }
	
	CFRef &operator = (const CFRef &ref)
	{ if (ref) CFRetain(ref); return take(ref); }
	
	// take variant for when newly created CFType is returned
	// via a ptr-to-CFType argument.
	CFType *take()
	{ if (mRef) CFRelease(mRef); mRef = NULL; return &mRef; }
	
    operator CFType () const { return mRef; }
    operator bool () const { return mRef != NULL; }
    bool operator ! () const { return mRef == NULL; }
	
	CFType get() const { return mRef; }
	
	CFType &aref()
	{ take(NULL); return mRef; }
	
	CFType retain() const
	{ if (mRef) CFRetain(mRef); return mRef; }
	
	void release() const
	{ if (mRef) CFRelease(mRef); }
	
	template <class NewType>
	bool is() const { return CFTraits<NewType>::check(mRef); }
	
	template <class OldType>
	static CFType check(OldType cf, OSStatus err)
	{
		if (cf && !CFTraits<CFType>::check(cf))
		{
			throw CFRaiiException(CFTraits<CFType>::GetTypeID(cf), CFTraits<CFType>::cfid());
		}
		return CFType(cf);
	}
	
	template <class NewType>
	NewType as() const { return NewType(mRef); }
	
	template <class NewType>
	NewType as(OSStatus err) const { return CFRef<NewType>::check(mRef, err); }
	
private:
    CFType mRef;
};


template <class CFType>
class CFCopyRef : public CFRef<CFType>
{
	typedef CFRef<CFType> _Base;
public:
    CFCopyRef() { }
    CFCopyRef(CFType ref) : _Base(ref) { this->retain(); }
    CFCopyRef(const CFCopyRef &ref) : _Base(ref) { this->retain(); }
	template <class _T> CFCopyRef(const CFRef<_T> &ref) : _Base(ref) { this->retain(); }
	CFCopyRef(CFTypeRef ref, OSStatus err) : _Base(ref, err) { this->retain(); }
	
	CFCopyRef &take(CFType ref)
	{ _Base::take(ref); return *this; }
	
    CFCopyRef &operator = (CFType ref)
    { if (ref) CFRetain(ref); return take(ref); }
	
	CFCopyRef &operator = (const CFCopyRef &ref)
	{ _Base::operator = (ref); return *this; }
};

// MARK: -
// MARK: CFNumber Templates
//
// This is nasty because CFNumber does not support unsigned types, and there's really no portably-safe
// way of working around this. So the handling of unsigned numbers is "almost correct."
//
template <class Number>
class CFNumberTraits;

template <>
struct CFNumberTraits<char>
{
	static const CFNumberType numberType = kCFNumberCharType;
	typedef char ValueType;
};
template <>
struct CFNumberTraits<short>
{
	static const CFNumberType numberType = kCFNumberShortType;
	typedef short ValueType;
};
template <>
struct CFNumberTraits<int>
{
	static const CFNumberType numberType = kCFNumberIntType;
	typedef int ValueType;
};
template <>
struct CFNumberTraits<long>
{
	static const CFNumberType numberType = kCFNumberLongType;
	typedef long ValueType;
};
template <>
struct CFNumberTraits<long long>
{
	static const CFNumberType numberType = kCFNumberLongLongType;
	typedef long long ValueType;
};
template <>
struct CFNumberTraits<float>
{
	static const CFNumberType numberType = kCFNumberFloatType;
	typedef float ValueType;
};
template <>
struct CFNumberTraits<double>
{
	static const CFNumberType numberType = kCFNumberDoubleType;
	typedef double ValueType;
};
// don't use the variable-width number types like CFIndex

template <>
struct CFNumberTraits<unsigned char>
{
	static const CFNumberType numberType = kCFNumberIntType;
	typedef int ValueType;
};
template <>
struct CFNumberTraits<unsigned short>
{
	static const CFNumberType numberType = kCFNumberIntType;
	typedef int ValueType;
};
template <>
struct CFNumberTraits<unsigned int>
{
	static const CFNumberType numberType = kCFNumberLongLongType;
	typedef long long ValueType;
};
template <>
struct CFNumberTraits<unsigned long>
{
	static const CFNumberType numberType = kCFNumberLongLongType;
	typedef long long ValueType;
};
template <>
struct CFNumberTraits<unsigned long long>
{
	static const CFNumberType numberType = kCFNumberLongLongType;
	typedef long long ValueType;
};

template <class N>
bool GetCFNumberValue(CFNumberRef number, N &value)
{
	if (std::tr1::is_arithmetic<N>::value)
	{
		return CFNumberGetValue(number
								, CFNumberTraits<N>::numberType
								, &value
								);
	}
	else
	{
		return false;
	}
}

#if 0
template <class Number>
Number cfNumber(CFNumberRef number)
{
	typename CFNumberTraits<Number>::ValueType value;
	if (CFNumberGetValue(number, CFNumberTraits<Number>::numberType, &value))
	{
		return value;
	}
	else
	{
		CFError::throwMe();
	}
}

template <class Number>
Number cfNumber(CFNumberRef number, Number defaultValue)
{
	typename CFNumberTraits<Number>::ValueType value;
	if (CFNumberGetValue(number, CFNumberTraits<Number>::numberType, &value))
		return value;
	else
		return defaultValue;
}

template <class Number>
CFNumberRef makeCFNumber(Number value)
{
	typename CFNumberTraits<Number>::ValueType cfValue = value;
	return CFNumberCreate(NULL, CFNumberTraits<Number>::numberType, &cfValue);
}
#endif

END_QC_NAMESPACE

#endif
