//
// RWLock.cpp
//
// Library: Foundation
// Package: Threading
// Module:  RWLock
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RWLock.h"


#if defined(POCO_CXX11_RWLOCK_FINISHED) && defined(POCO_ENABLE_CPP14)
#include "RWLock_STD.cpp"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "RWLock_WINCE.cpp"
#else
#include "RWLock_WIN32.cpp"
#endif
#elif defined(POCO_ANDROID)
#include "RWLock_Android.cpp"
#elif defined(POCO_VXWORKS)
#include "RWLock_VX.cpp"
#else
#include "RWLock_POSIX.cpp"
#endif


namespace Poco {


RWLock::RWLock()
{
}

	
RWLock::~RWLock()
{
}


} // namespace Poco
