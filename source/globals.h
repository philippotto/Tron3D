#pragma once
#ifndef EXTERN
#define EXTERN extern
#endif


#include <scriptzeug/ScriptEnvironment.h>
#include <scriptzeug/Scriptable.h>

namespace troen
{
	EXTERN long double g_gameTime;
	EXTERN long double g_gameLoopTime;
	EXTERN scriptzeug::ScriptEnvironment* g_scripting;
}