/*
 *  QCSet.cpp
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#include "QCSet.h"

void QCSet::show() const
{
#ifndef NDEBUG
	CFShow(set);
#endif
}