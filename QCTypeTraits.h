/*
 *  QCTypeTraits.h
 *  CFRaii
 *
 * Copyright (c) 2011 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */


#ifndef _QC_TYPE_TRAITS_GUARD_
#define _QC_TYPE_TRAITS_GUARD_

#include <tr1/type_traits>

#include "QCMacrosInternal.h"

// MARK: -
// MARK: Is Core Foundation type trait

BEGIN_QC_NAMESPACE

namespace Detail // we would have liked to make this an anonymous namespace,
				 // but that's bad news in a header file.
{

	typedef std::tr1::false_type false_type;
	typedef std::tr1::true_type true_type;
	
	// MARK: internal template & specializations
	template <class T>
	struct _is_CFType : public false_type
	{ };
	
#define Is_CF_Type(Type) template <> \
struct _is_CFType <Type##Ref> : public true_type \
{ }
	
	Is_CF_Type(CFAllocator);
	
	Is_CF_Type(CFArray);
	
	Is_CF_Type(CFAttributedString);
	
	Is_CF_Type(CFBag);
	
	Is_CF_Type(CFBinaryHeap);
	
	Is_CF_Type(CFBitVector);
	
	Is_CF_Type(CFBoolean);
	
	Is_CF_Type(CFBundle);
	
	Is_CF_Type(CFCalendar);
	
	Is_CF_Type(CFCharacterSet);
	
	Is_CF_Type(CFData);
	
	Is_CF_Type(CFDate);
	
	Is_CF_Type(CFDateFormatter);
	
	Is_CF_Type(CFDictionary);
	
	Is_CF_Type(CFError);
	
	Is_CF_Type(CFFileDescriptor);
	
	Is_CF_Type(CFLocale);
	
	Is_CF_Type(CFMachPort);
	
	Is_CF_Type(CFMessagePort);
	
	Is_CF_Type(CFMutableArray);
	
	Is_CF_Type(CFMutableAttributedString);
	
	Is_CF_Type(CFMutableBag);
	
	Is_CF_Type(CFMutableBitVector);
	
	Is_CF_Type(CFMutableCharacterSet);
	
	Is_CF_Type(CFMutableData);
	
	Is_CF_Type(CFMutableDictionary);
	
	Is_CF_Type(CFMutableSet);
	
	Is_CF_Type(CFMutableString);
	
	Is_CF_Type(CFNotificationCenter);
	
	Is_CF_Type(CFNull);
	
	Is_CF_Type(CFNumber);
	
	Is_CF_Type(CFNumberFormatter);
	
#if 0
	// CFPluginRef is a typedef to the same opaque type as CFBundleRef
	Is_CF_Type(CFPlugIn);
#endif
	
	Is_CF_Type(CFPlugInInstance);
	
#if 0
	// CFTypeRef and CFPropertyListRef are both typedefs for void const *
	Is_CF_Type(CFPropertyList);
#endif
	
	Is_CF_Type(CFReadStream);
	
	Is_CF_Type(CFRunLoop);
	
	Is_CF_Type(CFRunLoopObserver);
	
	Is_CF_Type(CFRunLoopSource);
	
	Is_CF_Type(CFRunLoopTimer);
	
	Is_CF_Type(CFSet);
	
	Is_CF_Type(CFSocket);
	
	Is_CF_Type(CFString);
	
	Is_CF_Type(CFStringTokenizer);
	
	Is_CF_Type(CFTimeZone);
	
	Is_CF_Type(CFTree);
	
	Is_CF_Type(CFType);
	
	Is_CF_Type(CFURL);
	
	Is_CF_Type(CFUserNotification);
	
	Is_CF_Type(CFUUID);
	
	Is_CF_Type(CFWriteStream);
	
	Is_CF_Type(CFXMLNode);
	
	Is_CF_Type(CFXMLParser);
	
#if 0
	// CFXMLTreeRef is a typedef for CFTreeRef
	Is_CF_Type(CFXMLTree);
#endif
#undef Is_CF_Type
} /* Detail namespace */

// MARK: public-facing is_CFType<> interface
template <class T>
/* we need to remove_cv (from the pointer-to-opaque-CF-type)
 * in order to ensure matching the above template specialization
 */
struct is_CFType : public Detail::_is_CFType < typename std::tr1::remove_cv<T>::type >
{ };


// MARK: -
// MARK: CFType_traits

template < class CF, bool = is_CFType<CF>::value >
struct CFType_traits
{
	static bool const is_CFType = false;
};

// partial specialization for is_CFType
template < class CF >
struct CFType_traits < CF, true >
{
	typedef CF value_type;
	static bool const is_CFType = true;
	static value_type get(CF const &obj) { return obj; }
	static CFTypeID typeID(CF const &obj) { return CFGetTypeID(obj); }
	static CFAllocatorRef allocator(CF const &obj) { return CFGetAllocator(obj); }
	static CFIndex RetainCount(CF const &obj) { return CFGetRetainCount(obj); } // should never be called
};

END_QC_NAMESPACE

#endif
