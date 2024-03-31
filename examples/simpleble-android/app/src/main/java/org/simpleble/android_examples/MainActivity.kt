package org.simpleble.android_examples

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.material.BottomNavigation
import androidx.compose.material.BottomNavigationItem
import androidx.compose.material.Icon
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Scaffold
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.AccountBox
import androidx.compose.material.icons.filled.Info
import androidx.compose.material.icons.filled.Person
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier


class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            MaterialTheme {
                Surface {
                    ExampleView()
                }
            }
        }
    }
}

@Composable
fun ExampleView() {
    var selectedTab by remember { mutableIntStateOf(0) }

    Scaffold(
        bottomBar = {
            BottomNavigation {
                BottomNavigationItem(
                    label = { Text("List Adapters") },
                    icon = { Icon(Icons.Default.Info, contentDescription = null) },
                    selected = selectedTab == 0,
                    onClick = { selectedTab = 0 }
                )
                BottomNavigationItem(
                    label = { Text("Scan") },
                    icon = { Icon(Icons.Default.AccountBox, contentDescription = null) },
                    selected = selectedTab == 1,
                    onClick = { selectedTab = 1 }
                )
                BottomNavigationItem(
                    label = { Text("Connect") },
                    icon = { Icon(Icons.Default.AccountBox, contentDescription = null) },
                    selected = selectedTab == 2,
                    onClick = { selectedTab = 2 }
                )
                BottomNavigationItem(
                    label = { Text("Read") },
                    icon = { Icon(Icons.Default.AccountBox, contentDescription = null) },
                    selected = selectedTab == 3,
                    onClick = { selectedTab = 3 }
                )
                BottomNavigationItem(
                    label = { Text("Notify") },
                    icon = { Icon(Icons.Default.AccountBox, contentDescription = null) },
                    selected = selectedTab == 4,
                    onClick = { selectedTab = 4 }
                )
            }
        }
    ) { innerPadding ->
        Column(
            modifier = Modifier.padding(innerPadding)
        ) {
            when (selectedTab) {
                0 -> ListAdaptersContent()
                1 -> ScanContent()
                2 -> ConnectContent()
                3 -> ReadContent()
                4 -> NotifyContent()
                else -> ListAdaptersContent() // Default
            }
        }
    }
}

@Composable
fun ListAdaptersContent() {
    Text("List Adapters" + SimpleAndroidBLE().stringFromJNI())
}

@Composable
fun ScanContent() {
    Text("Scan")
}

@Composable
fun ConnectContent() {
    Text("Connect")
}

@Composable
fun ReadContent() {
    Text("Read")
}

@Composable
fun NotifyContent() {
    Text("Notify")
}


