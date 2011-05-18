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
struct _is_CFType <Type> : public true_type \
{ }
	
	Is_CF_Type(CFAllocatorRef);
	
	Is_CF_Type(CFArrayRef);
	
	Is_CF_Type(CFAttributedStringRef);
	
	Is_CF_Type(CFBagRef);
	
	Is_CF_Type(CFBinaryHeapRef);
	
	Is_CF_Type(CFBitVectorRef);
	
	Is_CF_Type(CFBooleanRef);
	
	Is_CF_Type(CFBundleRef);
	
	Is_CF_Type(CFCalendarRef);
	
	Is_CF_Type(CFCharacterSetRef);
	
	Is_CF_Type(CFDataRef);
	
	Is_CF_Type(CFDateRef);
	
	Is_CF_Type(CFDateFormatterRef);
	
	Is_CF_Type(CFDictionaryRef);
	
	Is_CF_Type(CFErrorRef);
	
	Is_CF_Type(CFFileDescriptorRef);
	
	Is_CF_Type(CFLocaleRef);
	
	Is_CF_Type(CFMachPortRef);
	
	Is_CF_Type(CFMessagePortRef);
	
	Is_CF_Type(CFMutableArrayRef);
	
	Is_CF_Type(CFMutableAttributedStringRef);
	
	Is_CF_Type(CFMutableBagRef);
	
	Is_CF_Type(CFMutableBitVectorRef);
	
	Is_CF_Type(CFMutableCharacterSetRef);
	
	Is_CF_Type(CFMutableDataRef);
	
	Is_CF_Type(CFMutableDictionaryRef);
	
	Is_CF_Type(CFMutableSetRef);
	
	Is_CF_Type(CFMutableStringRef);
	
	Is_CF_Type(CFNotificationCenterRef);
	
	Is_CF_Type(CFNullRef);
	
	Is_CF_Type(CFNumberRef);
	
	Is_CF_Type(CFNumberFormatterRef);
	
#if 0
	// CFPluginRef is a typedef to the same opaque type as CFBundleRef
	Is_CF_Type(CFPlugInRef);
#endif
	
	Is_CF_Type(CFPlugInInstanceRef);
	
#if 0
	// CFTypeRef and CFPropertyListRef are both typedefs for void const *
	Is_CF_Type(CFPropertyListRef);
#endif
	
	Is_CF_Type(CFReadStreamRef);
	
	Is_CF_Type(CFRunLoopRef);
	
	Is_CF_Type(CFRunLoopObserverRef);
	
	Is_CF_Type(CFRunLoopSourceRef);
	
	Is_CF_Type(CFRunLoopTimerRef);
	
	Is_CF_Type(CFSetRef);
	
	Is_CF_Type(CFSocketRef);
	
	Is_CF_Type(CFStringRef);
	
	Is_CF_Type(CFStringTokenizerRef);
	
	Is_CF_Type(CFTimeZoneRef);
	
	Is_CF_Type(CFTreeRef);
	
	Is_CF_Type(CFTypeRef);
	
	Is_CF_Type(CFURLRef);
	
	Is_CF_Type(CFUserNotificationRef);
	
	Is_CF_Type(CFUUIDRef);
	
	Is_CF_Type(CFWriteStreamRef);
	
	Is_CF_Type(CFXMLNodeRef);
	
	Is_CF_Type(CFXMLParserRef);
	
#if 0
	// CFXMLTreeRef is a typedef for CFTreeRef
	Is_CF_Type(CFXMLTreeRef);
#endif
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
