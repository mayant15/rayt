#pragma once

#ifdef _WIN32
#ifndef RAYT_EXPORT
#ifdef rayt_EXPORTS
#define RAYT_EXPORT __declspec(dllexport)
#else
#define RAYT_EXPORT __declspec(dllimport)
#endif
#endif
#else
#define RAYT_EXPORT
#endif
