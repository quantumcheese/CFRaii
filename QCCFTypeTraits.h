/*
 *  QCCFTypeTraits.h
 *  CFRaii
 *
 * Copyright (c) 2010 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */


#ifndef _QC_CF_TYPE_TRAITS_GUARD_
#define _QC_CF_TYPE_TRAITS_GUARD_

#include <tr1/type_traits>

#include "CFRaiiCommon.h"

// MARK: -
// MARK: Is Core Foundation type

// MARK: internal template & specializations
template <class T>
struct _is_CFType : public std::tr1::false_type
{ };

template <>
struct _is_CFType <CFAllocatorRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFArrayRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFAttributedStringRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFBagRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFBinaryHeapRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFBitVectorRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFBooleanRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFBundleRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFCalendarRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFCharacterSetRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFDataRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFDateRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFDateFormatterRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFDictionaryRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFErrorRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFFileDescriptorRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFLocaleRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMachPortRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMessagePortRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableArrayRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableAttributedStringRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableBagRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableBitVectorRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableCharacterSetRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableDataRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableDictionaryRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableSetRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFMutableStringRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFNotificationCenterRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFNullRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFNumberRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFNumberFormatterRef> : public std::tr1::true_type
{ };

#if 0
// CFPluginRef is typedef'd to the same opaque type as CFBundleRef
template <>
struct _is_CFType <CFPlugInRef> : public std::tr1::true_type
{ };
#endif

template <>
struct _is_CFType <CFPlugInInstanceRef> : public std::tr1::true_type
{ };

#if 0
// CFTypeRef and CFPropertyListRef are both typedefs for void const *
template <>
struct _is_CFType <CFPropertyListRef> : public std::tr1::true_type
{ };
#endif

template <>
struct _is_CFType <CFReadStreamRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFRunLoopRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFRunLoopObserverRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFRunLoopSourceRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFRunLoopTimerRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFSetRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFSocketRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFStringRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFStringTokenizerRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFTimeZoneRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFTreeRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFTypeRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFURLRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFUserNotificationRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFUUIDRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFWriteStreamRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFXMLNodeRef> : public std::tr1::true_type
{ };

template <>
struct _is_CFType <CFXMLParserRef> : public std::tr1::true_type
{ };

#if 0
// CFXMLTreeRef is a typedef for CFTreeRef
template <>
struct _is_CFType <CFXMLTreeRef> : public std::tr1::true_type
{ };
#endif


// MARK: public-facing is_CFType<> interface
template <class T>
struct is_CFType : public _is_CFType<typename std::tr1::remove_cv<T>::type>
{ };


// MARK: -
// MARK: CFType_traits

// only implement CFType_traits if is_CFType is true
template < class CF, bool = is_CFType<CF>::value >
struct CFType_traits;

template <class CF>
struct CFType_traits <CF, true>
{
	static bool const is_CFType = true;
	typedef CF value_type;
	static value_type get(CF const &obj) { return obj; }
};

#endif
