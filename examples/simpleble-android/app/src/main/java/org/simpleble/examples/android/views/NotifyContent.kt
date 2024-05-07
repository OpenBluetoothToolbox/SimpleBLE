package org.simpleble.examples.android.views

import android.util.Log
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.Button
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import org.simpleble.android.Adapter
import org.simpleble.android.BluetoothUUID
import org.simpleble.android.Peripheral
import org.simpleble.examples.android.viewmodels.BluetoothViewModel

@Composable
fun NotifyContent(bluetoothViewModel: BluetoothViewModel) {
    var scanResults by remember { mutableStateOf(emptyList<Peripheral>()) }
    var isScanning by remember { mutableStateOf(false) }
    var selectedDevice by remember { mutableStateOf<Peripheral?>(null) }
    var isConnected by remember { mutableStateOf(false) }
    var characteristics by remember { mutableStateOf(emptyList<Pair<BluetoothUUID, BluetoothUUID>>()) }
    var selectedCharacteristic by remember { mutableStateOf<Pair<BluetoothUUID, BluetoothUUID>?>(null) }
    var receivedData by remember { mutableStateOf<ByteArray?>(null) }

    LaunchedEffect(Unit) {
        CoroutineScope(Dispatchers.Main).launch {
            bluetoothViewModel.adapter.onScanActive.collect {
                Log.d("SimpleBLE", "Scan active: $it")
                isScanning = it
            }
        }

        CoroutineScope(Dispatchers.Main).launch {
            bluetoothViewModel.adapter.onScanFound.collect {
                Log.d("SimpleBLE", "Found device: ${it.identifier} [${it.address}] ${it.rssi} dBm")
                scanResults = scanResults + it
            }
        }
    }

    Column(
        modifier = Modifier.fillMaxSize(),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        Button(
            onClick = {
                if (!isScanning) {
                    CoroutineScope(Dispatchers.Main).launch {
                        scanResults = emptyList()
                        bluetoothViewModel.adapter.scanFor(5000)
                    }
                }
            },
            modifier = Modifier.padding(16.dp)
        ) {
            Text(text = if (isScanning) "Scanning..." else "Start Scan")
        }

        if (scanResults.isNotEmpty()) {
            Text(
                text = "The following devices were found:",
                style = MaterialTheme.typography.h6,
                modifier = Modifier.padding(16.dp)
            )

            LazyColumn(
                modifier = Modifier.fillMaxWidth(),
                contentPadding = PaddingValues(16.dp)
            ) {
                items(scanResults.withIndex().toList()) { (index, peripheral) ->
                    Text(
                        text = "[$index] ${peripheral.identifier} [${peripheral.address}]",
                        style = MaterialTheme.typography.body1,
                        modifier = Modifier
                            .padding(8.dp)
                            .clickable {
                                selectedDevice = peripheral
                            }
                    )
                }
            }
        } else {
            Text(
                text = "No devices found.",
                style = MaterialTheme.typography.body1,
                modifier = Modifier.padding(16.dp)
            )
        }

        selectedDevice?.let { peripheral ->
            Text(
                text = "Connecting to ${peripheral.identifier} [${peripheral.address}]",
                style = MaterialTheme.typography.body1,
                modifier = Modifier.padding(16.dp)
            )

            Button(
                onClick = {
                    if (!isConnected) {
                        CoroutineScope(Dispatchers.Main).launch {
                            peripheral.connect()
                            isConnected = true

//                            characteristics = peripheral.services.flatMap { service ->
//                                service.characteristics.map { characteristic ->
//                                    Pair(service.uuid, characteristic.uuid)
//                                }
//                            }
                        }
                    } else {
                        CoroutineScope(Dispatchers.Main).launch {
                            peripheral.disconnect()
                            isConnected = false
                            characteristics = emptyList()
                            selectedCharacteristic = null
                            receivedData = null
                        }
                    }
                },
                modifier = Modifier.padding(16.dp)
            ) {
                Text(text = if (isConnected) "Disconnect" else "Connect")
            }

            if (isConnected) {
                Text(
                    text = "Successfully connected, printing services and characteristics..",
                    style = MaterialTheme.typography.body1,
                    modifier = Modifier.padding(16.dp)
                )

                LazyColumn(
                    modifier = Modifier.fillMaxWidth(),
                    contentPadding = PaddingValues(16.dp)
                ) {
                    items(characteristics.withIndex().toList()) { (index, characteristic) ->
                        Text(
                            text = "[$index] ${characteristic.first} ${characteristic.second}",
                            style = MaterialTheme.typography.body1,
                            modifier = Modifier
                                .padding(8.dp)
                                .clickable {
                                    selectedCharacteristic = characteristic
                                }
                        )
                    }
                }

                selectedCharacteristic?.let { characteristic ->
                    Button(
                        onClick = {
                            CoroutineScope(Dispatchers.Main).launch {
                                peripheral.connect()
                                peripheral.notify(BluetoothUUID("0000180f-0000-1000-8000-00805f9b34fb"), BluetoothUUID("00002a19-0000-1000-8000-00805f9b34fb")).collect { it ->
                                    val hexString = it.joinToString(separator = " ") { "%02x".format(it) }
                                    Log.d("SimpleBLE", "Received notification: $hexString")
                                }
                            }
                            CoroutineScope(Dispatchers.Main).launch {
                                delay(5000)
                                peripheral.unsubscribe(BluetoothUUID("0000180f-0000-1000-8000-00805f9b34fb"), BluetoothUUID("00002a19-0000-1000-8000-00805f9b34fb"))
                            }
                        },
                        modifier = Modifier.padding(16.dp)
                    ) {
                        Text(text = "Subscribe to Notifications")
                    }

                    receivedData?.let { data ->
                        Text(
                            text = "Received: ${data.joinToString(separator = " ") { "%02x".format(it) }}",
                            style = MaterialTheme.typography.body1,
                            modifier = Modifier.padding(16.dp)
                        )
                    }
                }
            }
        }
    }
}