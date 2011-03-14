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

namespace /* anonymous namespace */
{
	typedef std::tr1::false_type false_type;
	typedef std::tr1::true_type true_type;
	
	// MARK: internal template & specializations
	template <class T>
	struct _is_CFType : public false_type
	{ };
	
	template <>
	struct _is_CFType <CFAllocatorRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFArrayRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFAttributedStringRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFBagRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFBinaryHeapRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFBitVectorRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFBooleanRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFBundleRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFCalendarRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFCharacterSetRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFDataRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFDateRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFDateFormatterRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFDictionaryRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFErrorRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFFileDescriptorRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFLocaleRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMachPortRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMessagePortRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableArrayRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableAttributedStringRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableBagRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableBitVectorRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableCharacterSetRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableDataRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableDictionaryRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableSetRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFMutableStringRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFNotificationCenterRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFNullRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFNumberRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFNumberFormatterRef> : public true_type
	{ };
	
#if 0
	// CFPluginRef is typedef'd to the same opaque type as CFBundleRef
	template <>
	struct _is_CFType <CFPlugInRef> : public true_type
	{ };
#endif
	
	template <>
	struct _is_CFType <CFPlugInInstanceRef> : public true_type
	{ };
	
#if 0
	// CFTypeRef and CFPropertyListRef are both typedefs for void const *
	template <>
	struct _is_CFType <CFPropertyListRef> : public true_type
	{ };
#endif
	
	template <>
	struct _is_CFType <CFReadStreamRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFRunLoopRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFRunLoopObserverRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFRunLoopSourceRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFRunLoopTimerRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFSetRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFSocketRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFStringRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFStringTokenizerRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFTimeZoneRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFTreeRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFTypeRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFURLRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFUserNotificationRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFUUIDRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFWriteStreamRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFXMLNodeRef> : public true_type
	{ };
	
	template <>
	struct _is_CFType <CFXMLParserRef> : public true_type
	{ };
	
#if 0
	// CFXMLTreeRef is a typedef for CFTreeRef
	template <>
	struct _is_CFType <CFXMLTreeRef> : public true_type
	{ };
#endif
} /* anonymous namespace */

// MARK: public-facing is_CFType<> interface
template <class T>
/* we need to remove_cv (from the pointer-to-opaque-CF-type)
 * in order to ensure matching the above template specialization
 */
struct is_CFType : public _is_CFType < typename std::tr1::remove_cv<T>::type >
{ };


// MARK: -
// MARK: CFType_traits

template < class CF, bool = is_CFType<CF>::value >
struct CFType_traits
{
	static bool const is_CFType = false;
};

// partial specialization for is_CFType
template <class CF>
struct CFType_traits <CF, true>
{
	typedef CF value_type;
	static bool const is_CFType = true;
	static CFTypeID typeID(CF const &obj) { return CFGetTypeID(obj); }
	static value_type get(CF const &obj) { return obj; }
};

END_QC_NAMESPACE

#endif
