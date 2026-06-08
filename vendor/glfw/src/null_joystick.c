// vendor/glfw/src/null_joystick.c
//========================================================================
// GLFW 3.4 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2016-2017 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

// Trimmed-down vendored copy. Comments stripped to slim the tree, 2026-06-08.
// Upstream pin and license unchanged; see THIRD_PARTY_NOTICES.md and vendor/versions.md.
#include "internal.h"



GLFWbool _glfwInitJoysticksNull(void)
{
    return GLFW_TRUE;
}

void _glfwTerminateJoysticksNull(void)
{
}

GLFWbool _glfwPollJoystickNull(_GLFWjoystick* js, int mode)
{
    return GLFW_FALSE;
}

const char* _glfwGetMappingNameNull(void)
{
    return "";
}

void _glfwUpdateGamepadGUIDNull(char* guid)
{
}

