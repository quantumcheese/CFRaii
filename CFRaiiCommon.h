/*
 *  CFRaii.h
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#ifndef _CFRAII_COMMON_GUARD_
#define _CFRAII_COMMON_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>

#import "QCMacrosInternal.h"

#include "QCTypeTraits.h"

BEGIN_QC_NAMESPACE
static inline bool isNull(CFTypeRef cf)
{
	return cf == NULL;
}

static inline bool isNotNull(CFTypeRef cf)
{
	return !isNull(cf);
}

/*
 * We templatized QCRetain in order to avoid the need to cast its return values.
 * The previous version was:
 * CFTypeRef QCRetain(CFTypeRef object)
 * {
 *		return isNull(object) ? object : CFRetain(object);
 * }
 * However, its return values required static_cast<>ing to get back to the originating type.
 */

template < class T >
inline T Retain(T const &object)
{
	if (isNotNull(object)) CFRetain(object);
	return object;
}

// QCRetain is being deprecated in favor of unprefixed Retain
template < class T >
inline T QCRetain(T const &object) DEPRECATED_ATTRIBUTE;
template < class T >
inline T QCRetain(T const &object)
{
	return Retain(object);
}

// non-template version -- is there any reason to templatize Release ?
inline void Release(CFTypeRef object)
{
	if (isNotNull(object)) CFRelease(object);
}

void QCRelease(CFTypeRef object) DEPRECATED_ATTRIBUTE;

inline void QCRelease(CFTypeRef object)
{
	Release(object);
	return;
}

END_QC_NAMESPACE

#endif
