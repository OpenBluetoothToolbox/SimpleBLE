#include "JniHelper.h"

using namespace SimpleBLE;
using namespace SimpleBLE::JNI;



Env::Env() { _env = VM::env(); }

Class Env::find_class(const std::string& name) {
    jclass jcls = _env->FindClass(name.c_str());
    if (jcls == nullptr) {
        throw std::runtime_error("Failed to find class: " + name);
    }
    Class cls(jcls);
    _env->DeleteLocalRef(jcls);
    return cls;
}

