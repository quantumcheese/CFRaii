/*
 *  QCNumber.cpp
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#include "QCNumber.h"

BEGIN_QC_NAMESPACE

void QCNumber::show() const
{
#ifndef NDEBUG
	CFShow(number);
#endif
}

END_QC_NAMESPACE
