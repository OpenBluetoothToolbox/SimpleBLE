#pragma once

#include <jni.h>
#include <string>

#include "GlobalRef.hpp"
#include "VM.hpp"

namespace SimpleBLE {
namespace JNI {

struct JObjectComparator {
    bool operator() (const jobject& lhs, const jobject& rhs) const {
        // Use JNI's IsSameObject to compare objects, considering NULL cases
        if (lhs == nullptr || rhs == nullptr) {
            return lhs < rhs;  // Handle nulls consistently by pointer comparison
        }

        JNIEnv* env = VM::env();
        return !env->IsSameObject(lhs, rhs) && lhs < rhs;
    }
};

// Forward declarations
class Class;

class Object {
  public:
    Object() = default;

    Object(jobject obj) : _obj(obj) {
        JNIEnv* env = VM::env();
        _cls = env->GetObjectClass(obj);
    }

    Object(jobject obj, jclass cls) : _obj(obj), _cls(cls) {}

    jobject get() const { return _obj.get(); }

    explicit operator bool() const {
        return _obj.get() != nullptr;
    }

    template <typename... Args>
    Object call_object_method(jmethodID method, Args&&... args) {
        JNIEnv* env = VM::env();
        jobject result = env->CallObjectMethod(_obj.get(), method, std::forward<Args>(args)...);
        jclass resultClass = env->GetObjectClass(result);
        return Object(result, resultClass);
    }

    template <typename... Args>
    void call_void_method(jmethodID method, Args&&... args) {
        JNIEnv* env = VM::env();
        env->CallVoidMethod(_obj.get(), method, std::forward<Args>(args)...);
    }

    template <typename... Args>
    bool call_boolean_method(jmethodID method, Args&&... args) {
        JNIEnv* env = VM::env();
        jboolean result = env->CallBooleanMethod(_obj.get(), method, std::forward<Args>(args)...);
        return result;
    }

    template <typename... Args>
    int call_int_method(jmethodID method, Args&&... args) {
        JNIEnv* env = VM::env();
        jint result = env->CallIntMethod(_obj.get(), method, std::forward<Args>(args)...);
        return result;
    }

    template <typename... Args>
    std::string call_string_method(jmethodID method, Args&&... args) {
        JNIEnv* env = VM::env();
        jstring jstr = (jstring)env->CallObjectMethod(_obj.get(), method, std::forward<Args>(args)...);

        if (jstr == nullptr) {
            return "";
        }

        const char* c_str = env->GetStringUTFChars(jstr, nullptr);
        std::string result(c_str);
        env->ReleaseStringUTFChars(jstr, c_str);
        return result;
    }

    


    template <typename... Args>
    Object call_object_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetMethodID(_cls.get(), name, signature);
        jobject result = env->CallObjectMethod(_obj.get(), method, std::forward<Args>(args)...);

        jclass resultClass = env->GetObjectClass(result);

        return Object(result, resultClass);
    }

    template <typename... Args>
    void call_void_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetMethodID(_cls.get(), name, signature);
        env->CallVoidMethod(_obj.get(), method, std::forward<Args>(args)...);
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

        if (jstr == nullptr) {
            return "";
        }

        const char* c_str = env->GetStringUTFChars(jstr, nullptr);
        std::string result(c_str);
        env->ReleaseStringUTFChars(jstr, c_str);
        return result;
    }

  private:
    GlobalRef<jobject> _obj;
    GlobalRef<jclass> _cls;
};

class Class {
  public:
    Class() = default;
    Class(jclass cls) : _cls(cls) {}

    jclass get() { return _cls.get(); }

    template <typename... Args>
    Object call_static_method(const char* name, const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetStaticMethodID(_cls.get(), name, signature);
        jobject obj = env->CallStaticObjectMethod(_cls.get(), method, std::forward<Args>(args)...);

        return Object(obj, _cls.get());
    }

    template <typename... Args>
    Object call_constructor(const char* signature, Args&&... args) {
        JNIEnv* env = VM::env();

        jmethodID method = env->GetMethodID(_cls.get(), "<init>", signature);
        jobject obj = env->NewObject(_cls.get(), method, std::forward<Args>(args)...);

        return Object(obj, _cls.get());
    }

    void register_natives(const JNINativeMethod* methods, int nMethods) {
        JNIEnv* env = VM::env();
        env->RegisterNatives(_cls.get(), methods, nMethods);
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

        JNIEnv* operator->() { return _env; }

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
