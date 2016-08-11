/** Copyright 2016 MarFil Studios. All rights reserved. **/

#ifndef _EXPOSED_FUNCTIONS_H
#define _EXPOSED_FUNCTIONS_H
#include <lua-5.3.2/src/lua.h>

void ExposeFunctionsToScript();
int GetExposedFunctionsForType(type_info &type);

#endif //  ifndef _EXPOSED_FUNCTIONS_H