package org.simpleble.examples.android.views

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
import kotlinx.coroutines.launch
import org.simpleble.android.Adapter
import org.simpleble.android.Peripheral





@Composable
fun ScanContent() {
    val adapter = Adapter.getAdapters()[0]
    var scanResults by remember { mutableStateOf(emptyList<Peripheral>()) }


    LaunchedEffect(Unit) {
        CoroutineScope(Dispatchers.Main).launch {
            adapter.onScanStart.collect {
                println("Scan started.")
            }
        }

        CoroutineScope(Dispatchers.Main).launch {
            adapter.onScanStop.collect {
                println("Scan stopped.")
                scanResults = adapter.scanGetResults()
            }
        }

        CoroutineScope(Dispatchers.Main).launch {
            adapter.onScanFound.collect {
                println("Found device: ${it.identifier} [${it.address}] ${it.rssi} dBm")
            }
        }

        CoroutineScope(Dispatchers.Main).launch {
            adapter.onScanUpdated.collect {
                println("Updated device: ${it.identifier} [${it.address}] ${it.rssi} dBm")
            }
        }






//        adapter.setCallbackOnScanFound { peripheral ->
//            println("Found device: ${peripheral.identifier} [${peripheral.address}] ${peripheral.rssi} dBm")
//        }
//        adapter.setCallbackOnScanUpdated { peripheral ->
//            println("Updated device: ${peripheral.identifier} [${peripheral.address}] ${peripheral.rssi} dBm")
//        }
//        adapter.setCallbackOnScanStart {
//            println("Scan started.")
//            isScanning = true
//        }
//        adapter.setCallbackOnScanStop {
//            println("Scan stopped.")
//            isScanning = false
//            scanResults = adapter.scanGetResults().map { peripheral ->
//                PeripheralInfo(
//                    identifier = peripheral.identifier,
//                    address = peripheral.address,
//                    rssi = peripheral.rssi,
//                    isConnectable = peripheral.isConnectable,
//                    txPower = peripheral.txPower,
//                    addressType = peripheral.addressType,
//                    services = peripheral.services.map { ServiceInfo(it.uuid, it.data) },
//                    manufacturerData = peripheral.manufacturerData.mapValues { it.value.toList() }
//                )
//            }
//        }
    }

    Column(
        modifier = Modifier.fillMaxSize(),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        Button(
            onClick = {
                if (!adapter.scanIsActive) {
                    CoroutineScope(Dispatchers.Main).launch {
                        adapter.scanFor(5000)
                        scanResults = adapter.scanGetResults() // TODO: Switch to using the callback instead.
                    }
                }
            },
            modifier = Modifier.padding(16.dp)
        ) {
            Text(text = if (adapter.scanIsActive) "Scanning..." else "Start Scan")
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
                    val connectableString = if (peripheral.isConnectable) "Connectable" else "Non-Connectable"
                    Text(
                        text = "[$index] ${peripheral.identifier} [${peripheral.address}] ${peripheral.rssi} dBm $connectableString",
                        style = MaterialTheme.typography.body1,
                        modifier = Modifier.padding(8.dp)
                    )
                    Text(
                        text = "Tx Power: ${peripheral.txPower} dBm",
                        style = MaterialTheme.typography.body2,
                        modifier = Modifier.padding(start = 16.dp)
                    )
                    Text(
                        text = "Address Type: ${peripheral.addressType}",
                        style = MaterialTheme.typography.body2,
                        modifier = Modifier.padding(start = 16.dp)
                    )

//                    peripheral.services.forEach { service ->
//                        Text(
//                            text = "Service UUID: ${service.uuid}",
//                            style = MaterialTheme.typography.body2,
//                            modifier = Modifier.padding(start = 16.dp)
//                        )
//                        Text(
//                            text = "Service data: ${service.data}",
//                            style = MaterialTheme.typography.body2,
//                            modifier = Modifier.padding(start = 16.dp)
//                        )
//                    }
//
//                    peripheral.manufacturerData.forEach { (manufacturerId, data) ->
//                        Text(
//                            text = "Manufacturer ID: $manufacturerId",
//                            style = MaterialTheme.typography.body2,
//                            modifier = Modifier.padding(start = 16.dp)
//                        )
//                        Text(
//                            text = "Manufacturer data: $data",
//                            style = MaterialTheme.typography.body2,
//                            modifier = Modifier.padding(start = 16.dp)
//                        )
//                    }
                }
            }
        } else {
            Text(
                text = "No devices found.",
                style = MaterialTheme.typography.body1,
                modifier = Modifier.padding(16.dp)
            )
        }
    }
}