/*
 *  QCSet.cpp
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#include "QCSet.h"

BEGIN_QC_NAMESPACE

void QCSet::show() const
{
#ifndef NDEBUG
	CFShow(set);
#endif
}

END_QC_NAMESPACE