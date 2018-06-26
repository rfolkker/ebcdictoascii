#ifndef __POINTER_FUNCTIONS_H__
#define __POINTER_FUNCTIONS_H__


#define SafeFree(p) if (p) { free(p); }
#define SafeAssign(p, q) if (p) { *p = q; }
#define SafeKill(p) if (p) { p->Kill(); free(p); }


#endif
