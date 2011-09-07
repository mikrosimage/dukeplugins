/*
 * platform.h
 *
 *  Created on: 7 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifndef WINDOWS
 #if ( defined( WIN32 ) || defined( WIN64 ) || defined( _WIN32 ) || defined( _WIN64 ) || defined( __WINDOWS__ ) || defined( __TOS_WIN__ ) || defined( __WIN32__ ) )
  #define WINDOWS
 #endif
#endif

#if defined( _WIN32 ) || defined( _WIN64 )
 #define I386
#elif defined( __linux__ )
 #ifndef UNIX
  #define UNIX
 #endif
 #ifdef __i386__
  #define I386
 #elif defined( __amd64__ )
  #define AMD64
 #else
  #error cannot detect architecture
 #endif
#elif defined( __APPLE__ )
 #ifndef UNIX
  #define UNIX
 #endif
#else
 #error cannot detect operating system
#endif

#endif /* PLATFORM_H_ */
