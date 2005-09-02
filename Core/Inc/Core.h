/*=============================================================================

	Main core header

=============================================================================*/


#ifndef __CORE_H__
#define __CORE_H__


//!! add checking of CORE_VERSION (in most cases, executable will not be loaded, but ...)
#define CORE_VERSION	0


#ifndef CORE_API
#	define CORE_API DLL_IMPORT
#endif


// C standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

// Build options
#include "Build.h"

// Platform/compiler-specific code
#ifdef _MSC_VER
#	include "VcWin32.h"
#else
#	error Unknown compiler
#endif


// guard/unguard/unguardf[Slow] macros
#if !DO_GUARD
#	undef guard
#	undef unguard
#	undef unguardf
#	define guard(func)
#	define unguard
#	define unguardf(msg)
#endif

#if DO_GUARD_SLOW
#	define guardSlow(func)		guard(func)
#	define unguardSlow			unguard
#	define unguardfSlow(msg)	unguardf(msg)
#else
#	define guardSlow(func)		{
#	define unguardSlow			}
#	define unguardfSlow(msg)	}
#endif


#ifndef NULL
#	define NULL ((void *)0)
#endif


/*----------------------------------------------------------------------------
	Forward declarations
----------------------------------------------------------------------------*/

class CMemoryChain;


//----- miscellaneous ?

#define BIG_NUMBER				0x1000000


#include "Macro.h"
#include "Strings.h"


/*-----------------------------------------------------------------------------
	Text output system
-----------------------------------------------------------------------------*/

// Color codes

#define C_BLACK		0
#define C_RED		1
#define C_GREEN		2
#define C_YELLOW	3
#define C_BLUE		4
#define C_MAGENTA	5
#define C_CYAN		6
#define C_WHITE		7

#define COLOR_ESCAPE	'^'			// may be used for quick location of color-processing code

#define S_BLACK		"^0"
#define S_RED		"^1"
#define S_GREEN		"^2"
#define S_YELLOW	"^3"
#define S_BLUE		"^4"
#define S_MAGENTA	"^5"
#define S_CYAN		"^6"
#define S_WHITE		"^7"


// Output device
class CORE_API COutputDevice
{
public:
	bool	NoColors;			// when true, color codes will be removed from output
	bool	FlushEveryTime;		// when true, Flush() will be called after every Write()
	bool	GrabOutput;			// when true, appPrintf() will output to this device only (if this device registered last)
	COutputDevice ()
	:	NoColors(false), FlushEveryTime(false), GrabOutput(false)
	{}
#if 0
	virtual ~COutputDevice ()
	{
		Unregister ();
	}
#endif
	virtual void Write (const char *str) = 0;
	virtual void Flush ()
	{ /* empty */ }
	virtual void Close ()		// may be used instead of destructor
	{ /* empty */ }
	void Printf (const char *fmt, ...);
	void Register ();
	void Unregister ();
};

class CORE_API COutputDeviceNull : public COutputDevice
{
public:
	void Write (const char *str)
	{ /* empty */ }
};

CORE_API void appPrintf (const char *fmt, ...);
CORE_API void appWPrintf (const char *fmt, ...);


/*-----------------------------------------------------------------------------
	Error management
-----------------------------------------------------------------------------*/

class CErrorHandler
{
public:
	bool	swError;			// true when error was thrown by appError() call; will not dump CPU context
	bool	wasError;			// used for error history formatting
	TString<256>  Message;		// error message
	TString<2048> History;		// call history
	void	Reset ();
	// fields for non-fatal error
	bool	nonFatalError;		// when false, app can try to recover from error (and swError will be true)
	//?? add virtuals HandleFatalError()/HandleNonfatalError() or HandleError() for both cases (may call Fatal/Nonfatal handler)
};

CORE_API NORETURN void appFatalError (const char *fmt, ...);
#define appError	appFatalError
CORE_API NORETURN void appNonFatalError (const char *fmt, ...);


CORE_API void appUnwindPrefix (const char *fmt);		// not vararg (will display function name for unguardf only)
CORE_API NORETURN void appUnwindThrow (const char *fmt, ...);

// will return correct value even when log cannot be opened
CORE_API COutputDevice *appGetErrorLog ();
CORE_API void appDisplayError ();


/*-----------------------------------------------------------------------------
	Process control
-----------------------------------------------------------------------------*/

// Initialize core
CORE_API void appInit (/*?? const char *_cmdLine, COutputDevice *_log, CErrorHandler *_err */);
CORE_API void appExit ();

// System information
CORE_API const char *appPackage ();


/*-----------------------------------------------------------------------------
	Timing functions
-----------------------------------------------------------------------------*/

CORE_API double appSeconds ();
CORE_API double appMillisecondsf ();
CORE_API unsigned appMilliseconds ();
CORE_API const char *appTimestamp ();


extern CORE_API double GMSecondsPerCycle;

inline float appDeltaCyclesToMsecf (unsigned timeDelta)
{
	double v = timeDelta;
	return v * GMSecondsPerCycle;
}


/*-----------------------------------------------------------------------------
	Normal includes
-----------------------------------------------------------------------------*/

#include "DbgSymbols.h"						// MAY have some macros, so go as first as possible
#include "MemoryMgr.h"
//!! #include "Cvar.h"
#include "Commands.h"
//!! #include "FileSystem.h"
#include "ScriptParser.h"
#include "TextContainer.h"

/*-----------------------------------------------------------------------------
	Global variables
-----------------------------------------------------------------------------*/

CORE_API extern CErrorHandler	GErr;
//??CORE_API extern char GVersion[512];
CORE_API extern bool			GIsFatalError;
CORE_API extern bool			GIsRequestingExit;

/*-----------------------------------------------------------------------------
	Variables for current package
-----------------------------------------------------------------------------*/

//??
#if 0
extern "C"
{
	//!! this vars can be non-DLL_EXPORT for main package
	DLL_EXPORT extern const char	GPackage[];			// const name of current package
	DLL_EXPORT extern const char	PkgVersion[];
	extern HINSTANCE hInstance;
}
#endif

#endif
