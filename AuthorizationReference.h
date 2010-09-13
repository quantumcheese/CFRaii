/*
 *  AuthorizationReference.h
 *  CFRaii
 *
 *  Created by Richard Brown on 9/13/10.
 *  Copyright 2010 Javacool Software Development, LLC. All rights reserved.
 *
 */

#ifndef _AUTHORIZATION_REFERENCE_GUARD_
#define _AUTHORIZATION_REFERENCE_GUARD_

#include <Security/Security.h>

class AuthorizationReference
{
private:
	AuthorizationRef authRef;
	// no copying allowed.
	AuthorizationReference & operator = (AuthorizationReference const &);
	AuthorizationReference(AuthorizationReference const &);
	
public:
	// ctor
	AuthorizationReference(AuthorizationRef const & ref)
	: authRef(ref)
	{ }
	
	// dtor
	~AuthorizationReference()
	{
		AuthorizationFree(authRef, kAuthorizationFlagDestroyRights);
	}
};

// TODO: wrapper for AuthorizationRights?

#endif
