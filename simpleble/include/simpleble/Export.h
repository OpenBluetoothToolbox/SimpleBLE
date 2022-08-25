#ifndef SIMPLEBLE_EXPORT_H
#define SIMPLEBLE_EXPORT_H

#ifdef SIMPLEBLE_SHARED
#ifdef _MSC_VER
#ifdef SIMPLEBLE_BUILD
#define SIMPLEBLE_EXPORT __declspec(dllexport)
#else
#define SIMPLEBLE_EXPORT __declspec(dllimport)
#endif
#else
#ifdef SIMPLEBLE_BUILD
#define SIMPLEBLE_EXPORT __attribute__((visibility("default")))
#else
#define SIMPLEBLE_EXPORT __attribute__((visibility("default")))
#endif
#endif
#else
#define SIMPLEBLE_EXPORT
#endif

#endif
