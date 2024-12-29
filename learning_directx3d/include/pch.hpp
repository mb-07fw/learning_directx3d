#pragma once

#include <sdkddkver.h> // Defines macros that correspond to various versions of windows, and will set them to highest as default.

// The following #defines disable a bunch of unused windows stuff. If you get weird errors when trying to do some windows stuff, try removing some
// (or all) of these defines. (It will increase build time though).
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMIXMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

#define STRICT

#include <functional>
#include <sstream>
#include <exception>
#include <string>
#include <queue>
#include <bitset>

#include <Windows.h>
#include <windowsx.h>