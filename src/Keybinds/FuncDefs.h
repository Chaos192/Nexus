#ifndef KEYBINDS_FUNCDEFS_H
#define KEYBINDS_FUNCDEFS_H

#include "Keybind.h"

typedef void (*KEYBINDS_PROCESS)(const char* aIdentifier);
typedef void (*KEYBINDS_REGISTERWITHSTRING)(const char* aIdentifier, KEYBINDS_PROCESS aKeybindHandler, const char* aKeybind);
typedef void (*KEYBINDS_REGISTERWITHSTRUCT)(const char* aIdentifier, KEYBINDS_PROCESS aKeybindHandler, Keybind aKeybind);
typedef void (*KEYBINDS_DEREGISTER)(const char* aIdentifier);

#endif