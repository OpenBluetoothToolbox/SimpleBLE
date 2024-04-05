package org.simpleble.android

class SimpleDroidBle {

    companion object {
        // Used to load the 'android' library on application startup.
        init {
            System.loadLibrary("simpleble-jni")
        }
    }
}