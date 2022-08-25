#ifndef SIMPLEBLE_C_EXPORT_H
#define SIMPLEBLE_C_EXPORT_H

#ifdef SIMPLEBLE_C_SHARED
#    ifdef _MSC_VER
#        ifdef SIMPLEBLE_C_BUILD
#            define SIMPLEBLE_C_EXPORT __declspec(dllexport)
#        else
#            define SIMPLEBLE_C_EXPORT __declspec(dllimport)
#        endif
#    else
#        ifdef SIMPLEBLE_C_BUILD
#            define SIMPLEBLE_C_EXPORT __attribute__((visibility("default")))
#        else
#            define SIMPLEBLE_C_EXPORT __attribute__((visibility("default")))
#        endif
#    endif
#else
#    define SIMPLEBLE_C_EXPORT
#endif

#endif
