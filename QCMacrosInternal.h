/*
 *  QCMacrosInternal.h
 *  CFRaii
 *
 *  Created by Richard Brown on 3/13/11.
 *  Copyright 2011 Javacool Software Development, LLC. All rights reserved.
 *
 */

#ifndef _QC_MACROS_INTERNAL_
#define _QC_MACROS_INTERNAL_

#ifndef BEGIN_NAMESPACE
#define BEGIN_NAMESPACE(x) namespace x {
#endif
#ifndef END_NAMESPACE
#define END_NAMESPACE }
#endif


#if 1
	#define BEGIN_QC_NAMESPACE	BEGIN_NAMESPACE(QC)
	#define END_QC_NAMESPACE	END_NAMESPACE
#else
	#define BEGIN_QC_NAMESPACE
	#define END_QC_NAMESPACE
#endif

#endif
