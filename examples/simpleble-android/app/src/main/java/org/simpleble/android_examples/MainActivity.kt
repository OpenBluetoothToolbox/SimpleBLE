package org.simpleble.android_examples

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.runtime.Composable


class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            MaterialTheme {
                Surface {
                    Greeting(stringFromJNI())
                }
            }
        }
    }

    @Composable
    fun Greeting(text: String) {
        Text(text = text)
    }

    external fun stringFromJNI(): String

    companion object {
        init {
            System.loadLibrary("android_examples")
        }
    }
}