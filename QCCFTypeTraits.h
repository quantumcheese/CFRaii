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

// MARK: -
// MARK: Is Core Foundation type

template <class >
struct is_CFType
{ static bool const value = false; };

template <>
struct is_CFType <CFAllocatorRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFArrayRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFAttributedStringRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFBagRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFBinaryHeapRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFBitVectorRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFBooleanRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFBundleRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFCalendarRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFCharacterSetRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFDataRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFDateRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFDateFormatterRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFDictionaryRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFErrorRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFFileDescriptorRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFLocaleRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMachPortRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMessagePortRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableArrayRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableAttributedStringRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableBagRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableBitVectorRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableCharacterSetRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableDataRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableDictionaryRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableSetRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFMutableStringRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFNotificationCenterRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFNullRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFNumberRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFNumberFormatterRef>
{ static bool const value = true; };

#if 0
// CFPluginRef is typedef'd to the same opaque type as CFBundleRef
template <>
struct is_CFType <CFPlugInRef>
{ static bool const value = true; };
#endif

template <>
struct is_CFType <CFPlugInInstanceRef>
{ static bool const value = true; };

#if 0
// CFTypeRef and CFPropertyListRef are both typedefs for void const *
template <>
struct is_CFType <CFPropertyListRef>
{ static bool const value = true; };
#endif

template <>
struct is_CFType <CFReadStreamRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFRunLoopRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFRunLoopObserverRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFRunLoopSourceRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFRunLoopTimerRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFSetRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFSocketRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFStringRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFStringTokenizerRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFTimeZoneRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFTreeRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFTypeRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFURLRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFUserNotificationRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFUUIDRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFWriteStreamRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFXMLNodeRef>
{ static bool const value = true; };

template <>
struct is_CFType <CFXMLParserRef>
{ static bool const value = true; };

#if 0
// CFXMLTreeRef is a typedef for CFTreeRef
template <>
struct is_CFType <CFXMLTreeRef>
{ static bool const value = true; };
#endif


#if 0
// MARK: -
// MARK: CFType_traits

// only implement CFType_traits if is_CFType is true
template <class CF, bool = is_CFType<CF>::value>
struct CFType_traits;

template <class CF, true>
struct CFType_traits
{
	static bool const is_CFType = true;
	typedef CF type;
	static type get(CF const &obj) { return obj; }
};
#endif

#endif
