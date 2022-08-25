#ifndef SIMPLEBLUEZ_EXPORT_H
#define SIMPLEBLUEZ_EXPORT_H

#ifdef SIMPLEBLUEZ_SHARED
#    ifdef _MSC_VER
#        ifdef SIMPLEBLUEZ_BUILD
#            define SIMPLEBLUEZ_EXPORT __declspec(dllexport)
#        else
#            define SIMPLEBLUEZ_EXPORT __declspec(dllimport)
#        endif
#    else
#        ifdef SIMPLEBLUEZ_BUILD
#            define SIMPLEBLUEZ_EXPORT __attribute__((visibility("default")))
#        else
#            define SIMPLEBLUEZ_EXPORT __attribute__((visibility("default")))
#        endif
#    endif
#else
#    define SIMPLEBLUEZ_EXPORT
#endif

#endif
