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

// MARK: -
// MARK: Is Core Foundation type

template <class >
struct is_CFType
{ static bool const value = false; };

template <class CFAllocatorRef>
struct is_CFType
{ static bool const value = true; }

template <class CFArrayRef>
struct is_CFType
{ static bool const value = true; }

template <class CFAttributedStringRef>
struct is_CFType
{ static bool const value = true; }

template <class CFBagRef>
struct is_CFType
{ static bool const value = true; }

template <class CFBinaryHeapRef>
struct is_CFType
{ static bool const value = true; }

template <class CFBitVectorRef>
struct is_CFType
{ static bool const value = true; }

template <class CFBooleanRef>
struct is_CFType
{ static bool const value = true; }

template <class CFBundleRef>
struct is_CFType
{ static bool const value = true; }

template <class CFCalendarRef>
struct is_CFType
{ static bool const value = true; }

template <class CFCharacterSetRef>
struct is_CFType
{ static bool const value = true; }

template <class CFDataRef>
struct is_CFType
{ static bool const value = true; }

template <class CFDateRef>
struct is_CFType
{ static bool const value = true; }

template <class CFDateFormatterRef>
struct is_CFType
{ static bool const value = true; }

template <class CFDictionaryRef>
struct is_CFType
{ static bool const value = true; }

template <class CFErrorRef>
struct is_CFType
{ static bool const value = true; }

template <class CFFileDescriptorRef>
struct is_CFType
{ static bool const value = true; }

template <class CFLocaleRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMachPortRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMessagePortRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableArrayRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableAttributedStringRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableBagRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableBitVectorRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableCharacterSetRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableDataRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableDictionaryRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableSetRef>
struct is_CFType
{ static bool const value = true; }

template <class CFMutableStringRef>
struct is_CFType
{ static bool const value = true; }

template <class CFNotificationCenterRef>
struct is_CFType
{ static bool const value = true; }

template <class CFNullRef>
struct is_CFType
{ static bool const value = true; }

template <class CFNumberRef>
struct is_CFType
{ static bool const value = true; }

template <class CFNumberFormatterRef>
struct is_CFType
{ static bool const value = true; }

template <class CFPlugInRef>
struct is_CFType
{ static bool const value = true; }

template <class CFPlugInInstanceRef>
struct is_CFType
{ static bool const value = true; }

template <class CFPropertyListRef>
struct is_CFType
{ static bool const value = true; }

template <class CFReadStreamRef>
struct is_CFType
{ static bool const value = true; }

template <class CFRunLoopRef>
struct is_CFType
{ static bool const value = true; }

template <class CFRunLoopObserverRef>
struct is_CFType
{ static bool const value = true; }

template <class CFRunLoopSourceRef>
struct is_CFType
{ static bool const value = true; }

template <class CFRunLoopTimerRef>
struct is_CFType
{ static bool const value = true; }

template <class CFSetRef>
struct is_CFType
{ static bool const value = true; }

template <class CFSocketRef>
struct is_CFType
{ static bool const value = true; }

template <class CFStringRef>
struct is_CFType
{ static bool const value = true; }

template <class CFStringTokenizerRef>
struct is_CFType
{ static bool const value = true; }

template <class CFTimeZoneRef>
struct is_CFType
{ static bool const value = true; }

template <class CFTreeRef>
struct is_CFType
{ static bool const value = true; }

template <class CFTypeRef>
struct is_CFType
{ static bool const value = true; }

template <class CFURLRef>
struct is_CFType
{ static bool const value = true; }

template <class CFUserNotificationRef>
struct is_CFType
{ static bool const value = true; }

template <class CFUUIDRef>
struct is_CFType
{ static bool const value = true; }

template <class CFWriteStreamRef>
struct is_CFType
{ static bool const value = true; }

template <class CFXMLNodeRef>
struct is_CFType
{ static bool const value = true; }

template <class CFXMLParserRef>
struct is_CFType
{ static bool const value = true; }

template <class CFXMLTreeRef>
struct is_CFType
{ static bool const value = true; }

// MARK: -
// MARK: CFType_traits

// only implement CFType_traits if is_CFType is true
template <class CF, bool = is_CFType<class CF>::value>
struct CFType_traits;

template <class CF, true>
struct CFType_traits
{
	static bool const is_CFType = true;
	typedef CF type;
	static type get(CF const &obj) { return obj; }
};


#endif
