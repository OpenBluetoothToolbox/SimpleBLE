//
// Created by kevin on 01-10-2022.
//

#ifndef ANDROID_ANDROID_UTILS_H
#define ANDROID_ANDROID_UTILS_H

#include <string>
#include <jni.h>

void log_error(const std::string& msg);
void log_info(const std::string& msg);
void log_debug(const std::string& msg);

jstring to_jstring(JNIEnv* env, const std::string& str);

#endif //ANDROID_ANDROID_UTILS_H
