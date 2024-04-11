#include "android_utils.h"

#include <android/log.h>
#include <fmt/core.h>

void log_error(const std::string& msg) {
    __android_log_write(ANDROID_LOG_ERROR, "SimpleBLE", msg.c_str());
}
void log_info(const std::string& msg) {
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg.c_str());
}
void log_debug(const std::string& msg) {
    __android_log_write(ANDROID_LOG_DEBUG, "SimpleBLE", msg.c_str());
}

jstring to_jstring(JNIEnv* env, const std::string& str){
    return env->NewStringUTF(str.c_str());
}

std::string from_jstring(JNIEnv* env, jstring str){
    const char* c_str = env->GetStringUTFChars(str, nullptr);
    std::string result(c_str);
    env->ReleaseStringUTFChars(str, c_str);
    return result;
}

jbyteArray to_jbyteArray(JNIEnv* env, const std::string& data) {
    jbyteArray result = env->NewByteArray(data.size());
    env->SetByteArrayRegion(result, 0, data.size(), reinterpret_cast<const jbyte*>(data.data()));
    return result;
}

void throw_exception(JNIEnv* env, const std::string& msg) {
    log_error(fmt::format("Throwing exception: {}", msg));

    jclass Exception = env->FindClass("java/lang/Exception");
    env->ThrowNew(Exception, msg.c_str());
}