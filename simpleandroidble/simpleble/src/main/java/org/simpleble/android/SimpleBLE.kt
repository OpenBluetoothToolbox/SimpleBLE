package org.simpleble.android

class SimpleBLE {

    /**
     * A native method that is implemented by the 'android' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'android' library on application startup.
        init {
            System.loadLibrary("simpleble-jni")
        }
    }
}