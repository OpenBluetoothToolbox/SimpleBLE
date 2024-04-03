package org.simpleble.android_examples

class SimpleAndroidBLE {

    external fun stringFromJNI(): String

    companion object {
        init {
            System.loadLibrary("android_examples")
        }
    }

}