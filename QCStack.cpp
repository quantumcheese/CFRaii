/*
 *  QCStack.cpp
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#include "QCStack.h"

void QCStack::show() const
{
#ifndef NDEBUG
	CFShow(stack);
#endif
}

CFStringRef QCStack::concatenateStringsWithJoiningString(CFStringRef const joiningString) const
{
	return CFStringCreateByCombiningStrings(kCFAllocatorDefault, stack, joiningString);
}
