#ifndef SIMPLEDBUS_EXPORT_H
#define SIMPLEDBUS_EXPORT_H

#ifdef SIMPLEDBUS_SHARED
#ifdef _MSC_VER
#ifdef SIMPLEDBUS_BUILD
#define SIMPLEDBUS_EXPORT __declspec(dllexport)
#else
#define SIMPLEDBUS_EXPORT __declspec(dllimport)
#endif
#else
#ifdef SIMPLEDBUS_BUILD
#define SIMPLEDBUS_EXPORT __attribute__((visibility("default")))
#else
#define SIMPLEDBUS_EXPORT __attribute__((visibility("default")))
#endif
#endif
#else
#define SIMPLEDBUS_EXPORT
#endif

#endif
