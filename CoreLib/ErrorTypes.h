#ifndef __ERROR_TYPES_H__
#define __ERROR_TYPES_H__

enum TRISTATE
{
	TRITRUE	= 1,
	TRIFALSE = 0,
	TRIERROR = -1,

	FORCE_DWORD = 0xffffffff
};

extern TRISTATE	gTriState;

#define InvertTrueAndFalse(p)			if (p == TRITRUE) { p = TRIFALSE; } else if (p == TRIFALSE) { p = TRITRUE; } 
#define ReturnOnErrorBreakOnFalse(p)	gTriState = p;  if (gTriState == TRIERROR) { return TRIERROR; } else if (gTriState == TRIFALSE) { break; }
#define ReturnOnErrorBreakOnTrue(p)		gTriState = p;  if (gTriState == TRIERROR) { return TRIERROR; } else if (gTriState == TRITRUE) { break; }
#define ReturnOnErrorAndFalse(p)		gTriState = p;  if ((gTriState == TRIERROR) || (gTriState == TRIFALSE)) { return gTriState; }
#define ReturnOnErrorAndTrue(p)			gTriState = p;  if ((gTriState == TRIERROR) || (gTriState == TRITRUE)) { return gTriState; }
#define ReturnOnError(p)				if (p == TRIERROR) { return TRIERROR; }
#define ReturnOnFalse(p)				if (p == TRIFALSE) { return TRIFALSE; }
#define ReturnOnTrue(p)					if (p == TRITRUE)  { return TRITRUE; }
#define ReturnErrorOnFalse(p)			if (p == TRIFALSE) { return TRIERROR; }
#define ReturnFalseOnError(p)			if (p == TRIERROR) { return TRIFALSE; }
#define ReturnFalseOnErrorAndFalse(p)	gTriState = p;  if ((gTriState == TRIERROR) || (gTriState == TRIFALSE)) { return TRIFALSE; }
#define ReturnErrorOnErrorAndFalse(p)	gTriState = p;  if ((gTriState == TRIERROR) || (gTriState == TRIFALSE)) { return TRIERROR; }
#define BreakOnErrorAndFalse(p)			if (p != TRITRUE) { break; }
#define BreakOnError(p)					if (p == TRIERROR) { break; }
#define BreakOnFalse(p)					if (p == TRIFALSE) { break; }
#define BreakOnTrue(p)					if (p == TRITRUE) { break; }
#define ContinueOnTrue(p)				if (p == TRITRUE) { continue; }

//Not specifically TRISTATE related but errors none the less.
#define ReturnZeroOnZero(p)				if (p == 0)  { return 0; }
#define ReturnMinusOneOnMinusOne(p)		if (p == -1)  { return -1; }

#endif

