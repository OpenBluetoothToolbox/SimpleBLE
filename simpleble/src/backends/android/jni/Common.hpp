#pragma once

#include <jni.h>
#include <string>

#include "GlobalRef.hpp"
#include "VM.hpp"

namespace SimpleBLE {
namespace JNI {

// Forward declarations
class Class;

class Object : public GlobalRef<jobject> {
  public:
    Object() = default;

    Object(jobject obj, jclass cls) : _obj(obj), _cls(cls) {}

    template <typename... Args>
    GlobalRef<jobject> call_object_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetMethodID(_cls.get(), name, signature);
        jobject result = env->CallObjectMethod(_obj.get(), method, std::forward<Args>(args)...);

        return GlobalRef<jobject>(result);
    }

    template <typename... Args>
    bool call_boolean_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetMethodID(_cls.get(), name, signature);
        jboolean result = env->CallBooleanMethod(_obj.get(), method, std::forward<Args>(args)...);

        return result;
    }

    template <typename... Args>
    int call_int_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetMethodID(_cls.get(), name, signature);
        jint result = env->CallIntMethod(_obj.get(), method, std::forward<Args>(args)...);

        return result;
    }

    template <typename... Args>
    std::string call_string_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetMethodID(_cls.get(), name, signature);
        jstring jstr = (jstring)env->CallObjectMethod(_obj.get(), method, std::forward<Args>(args)...);

        const char* c_str = env->GetStringUTFChars(jstr, nullptr);
        std::string result(c_str);
        env->ReleaseStringUTFChars(jstr, c_str);
        return result;
    }

  private:
    GlobalRef<jclass> _cls;
    GlobalRef<jobject> _obj;
};

class Class : public GlobalRef<jclass> {
  public:
    Class() = default;
    Class(jclass cls) : _cls(cls) {}

    template <typename... Args>
    Object call_static_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetStaticMethodID(_cls.get(), name, signature);
        jobject obj = env->CallStaticObjectMethod(_cls.get(), method, std::forward<Args>(args)...);

        return Object(obj, _cls.get());
    }

  private:
    GlobalRef<jclass> _cls;
};

    class Env {
    public:
        Env() { _env = VM::env(); }
        virtual ~Env() = default;
        Env(Env& other) = delete;             // Remove the copy constructor
        void operator=(const Env&) = delete;  // Remove the copy assignment

        Class find_class(const std::string& name) {
            jclass jcls = _env->FindClass(name.c_str());
            if (jcls == nullptr) {
                throw std::runtime_error("Failed to find class: " + name);
            }
            Class cls(jcls);
            _env->DeleteLocalRef(jcls);
            return cls;
        }

    private:
        JNIEnv* _env = nullptr;
    };

}  // namespace JNI
}  // namespace SimpleBLE
