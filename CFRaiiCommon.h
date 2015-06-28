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
	return NULL == cf;
}

static inline bool isNotNull(CFTypeRef cf)
{
	return !isNull(cf);
}

/*
 * We templatized Retain in order to avoid the need to cast its return values.
 * The previous version was:
 * CFTypeRef Retain(CFTypeRef object)
 * {
 *		return isNull(object) ? object : CFRetain(object);
 * }
 * However, its return values required static_cast<>ing to get back to the originating type.
 */

template < class T >
inline T Retain(T ref) __attribute((enable_if(is_CFType<T>::value, "Retain is available only for Core Foundation types.")))
{
    if (isNotNull(object)) { CFRetain(ref) };
	return ref;
}

template < class T >
inline void Release(T ref) __attribute((enable_if(is_CFType<T>::value, "Release is available only for Core Foundation types.")))
{
    if (isNotNull(ref)) { CFRelease(ref); }
}

END_QC_NAMESPACE

#endif
