// vendor/glfw/src/win32_joystick.h
//========================================================================
// GLFW 3.4 Win32 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2006-2017 Camilla Löwy <elmindreda@glfw.org>
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
#define GLFW_WIN32_JOYSTICK_STATE         _GLFWjoystickWin32 win32;
#define GLFW_WIN32_LIBRARY_JOYSTICK_STATE

typedef struct _GLFWjoyobjectWin32
{
    int                     offset;
    int                     type;
} _GLFWjoyobjectWin32;

typedef struct _GLFWjoystickWin32
{
    _GLFWjoyobjectWin32*    objects;
    int                     objectCount;
    IDirectInputDevice8W*   device;
    DWORD                   index;
    GUID                    guid;
} _GLFWjoystickWin32;

void _glfwDetectJoystickConnectionWin32(void);
void _glfwDetectJoystickDisconnectionWin32(void);

