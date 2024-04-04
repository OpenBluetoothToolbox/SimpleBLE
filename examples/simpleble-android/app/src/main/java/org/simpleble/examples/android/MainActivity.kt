package org.simpleble.examples.android;

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.BottomNavigation
import androidx.compose.material.BottomNavigationItem
import androidx.compose.material.Button
import androidx.compose.material.Icon
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Scaffold
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.AccountBox
import androidx.compose.material.icons.filled.Info
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp

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
                    label = { Text("Adapter") },
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

// ---------------------------- Composable functions ----------------------------

data class AdapterInfo(
    val identifier: String,
    val address: String
)

@Composable
fun ListAdaptersContent() {
    var simpleBleVersion by remember { mutableStateOf("") }
    var bluetoothEnabled by remember { mutableStateOf(false) }
    var adapterList by remember { mutableStateOf(emptyList<AdapterInfo>()) }

    LaunchedEffect(Unit) {
        simpleBleVersion = "4"
        bluetoothEnabled = false
        adapterList = listOf(
            AdapterInfo("Adapter 1", "00:11:22:33:44:55"),
            AdapterInfo("Adapter 2", "00:11:22:33:44:56"),
            AdapterInfo("Adapter 3", "00:11:22:33:44:57"),
        )
    }

    Column(
        modifier = Modifier.fillMaxSize(),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        Text(
            text = "Using SimpleBLE version: $simpleBleVersion",
            style = MaterialTheme.typography.h6,
            textAlign = TextAlign.Center,
            modifier = Modifier.padding(16.dp)
        )

        Text(
            text = "Bluetooth enabled: $bluetoothEnabled",
            style = MaterialTheme.typography.h6,
            textAlign = TextAlign.Center,
            modifier = Modifier.padding(16.dp)
        )
        // SimpleDroidBle().stringFromJNI()
        // Text("List Adapters" + SimpleAndroidBLE().stringFromJNI())

        if (adapterList.isEmpty()) {
            Text(
                text = "No adapter found",
                style = MaterialTheme.typography.h6,
                textAlign = TextAlign.Center,
                modifier = Modifier.padding(16.dp)
            )
        } else {
            Text(
                text = "Adapters:",
                style = MaterialTheme.typography.h6,
                textAlign = TextAlign.Center,
                modifier = Modifier.padding(16.dp)
            )

            LazyColumn(
                modifier = Modifier.fillMaxWidth(),
                contentPadding = PaddingValues(16.dp)
            ) {
                items(adapterList) { adapter ->
                    Text(
                        text = "Adapter: ${adapter.identifier} [${adapter.address}]",
                        style = MaterialTheme.typography.body1,
                        modifier = Modifier.padding(8.dp)
                    )
                }
            }
        }
    }
}

// ---------------------------- Composable functions ----------------------------

data class PeripheralInfo(
    val identifier: String,
    val address: String,
    val rssi: Int,
    val isConnectable: Boolean,
    val txPower: Int,
    val addressType: String,
    val services: List<ServiceInfo>,
    val manufacturerData: Map<Int, List<Byte>>,
    // This is for connect
    val mtu: Int = 0,
    val connect: () -> Unit = {},
    val disconnect: () -> Unit = {}
)

data class ServiceInfo(
    val uuid: String,
    val data: ByteArray,
    val characteristics: List<CharacteristicInfo> = emptyList()
)

@Composable
fun ScanContent() {
    var scanResults by remember { mutableStateOf(emptyList<PeripheralInfo>()) }
    var isScanning by remember { mutableStateOf(false) }

    //val adapter = SimpleBLE.getAdapter()

    LaunchedEffect(Unit) {
        // Populate scanResults with dummy data
        scanResults = listOf(
            PeripheralInfo(
                identifier = "Device 1",
                address = "00:11:22:33:44:55",
                rssi = -50,
                isConnectable = true,
                txPower = -10,
                addressType = "Public",
                services = listOf(
                    ServiceInfo("00001800-0000-1000-8000-00805f9b34fb", byteArrayOf(0x01, 0x02, 0x03)),
                    ServiceInfo("00001801-0000-1000-8000-00805f9b34fb", byteArrayOf(0x04, 0x05, 0x06))
                ),
                manufacturerData = mapOf(
                    0x004C to listOf(0x01, 0x02, 0x03),
                    0x0059 to listOf(0x04, 0x05, 0x06)
                )
            ),
            PeripheralInfo(
                identifier = "Device 2",
                address = "00:11:22:33:44:56",
                rssi = -60,
                isConnectable = false,
                txPower = -20,
                addressType = "Random",
                services = listOf(
                    ServiceInfo("00001802-0000-1000-8000-00805f9b34fb", byteArrayOf(0x07, 0x08, 0x09)),
                    ServiceInfo("00001803-0000-1000-8000-00805f9b34fb", byteArrayOf(0x0A, 0x0B, 0x0C))
                ),
                manufacturerData = mapOf(
                    0x004C to listOf(0x07, 0x08, 0x09),
                    0x0059 to listOf(0x0A, 0x0B, 0x0C)
                )
            ))

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
                if (!isScanning) {
                    //adapter.scanFor(5000)
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

                    peripheral.services.forEach { service ->
                        Text(
                            text = "Service UUID: ${service.uuid}",
                            style = MaterialTheme.typography.body2,
                            modifier = Modifier.padding(start = 16.dp)
                        )
                        Text(
                            text = "Service data: ${service.data}",
                            style = MaterialTheme.typography.body2,
                            modifier = Modifier.padding(start = 16.dp)
                        )
                    }

                    peripheral.manufacturerData.forEach { (manufacturerId, data) ->
                        Text(
                            text = "Manufacturer ID: $manufacturerId",
                            style = MaterialTheme.typography.body2,
                            modifier = Modifier.padding(start = 16.dp)
                        )
                        Text(
                            text = "Manufacturer data: $data",
                            style = MaterialTheme.typography.body2,
                            modifier = Modifier.padding(start = 16.dp)
                        )
                    }
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

// ---------------------------- Composable functions ----------------------------

data class CharacteristicInfo(
    val uuid: String,
    val capabilities: List<String> = emptyList(),
    val descriptors: List<DescriptorInfo> = emptyList()
)

data class DescriptorInfo(
    val uuid: String
)

@Composable
fun ConnectContent() {
    var scanResults by remember { mutableStateOf(emptyList<PeripheralInfo>()) }
    var isScanning by remember { mutableStateOf(false) }
    var selectedDevice by remember { mutableStateOf<PeripheralInfo?>(null) }
    var isConnected by remember { mutableStateOf(false) }
    var mtu by remember { mutableStateOf(0) }

    //val adapter = SimpleBLE.getAdapter()

    LaunchedEffect(Unit) {
//        adapter.setCallbackOnScanFound { peripheral ->
//            scanResults = scanResults + PeripheralInfo(
//                identifier = peripheral.identifier,
//                address = peripheral.address
//            )
//        }
//        adapter.setCallbackOnScanStart {
//            println("Scan started.")
//            isScanning = true
//        }
//        adapter.setCallbackOnScanStop {
//            println("Scan stopped.")
//            isScanning = false
//        }
    }

    Column(
        modifier = Modifier.fillMaxSize(),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        Button(
            onClick = {
                if (!isScanning) {
                    scanResults = emptyList()
                    //adapter.scanFor(5000)
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
                        peripheral.connect()
                        isConnected = true
                        mtu = peripheral.mtu
                    } else {
                        peripheral.disconnect()
                        isConnected = false
                        mtu = 0
                    }
                },
                modifier = Modifier.padding(16.dp)
            ) {
                Text(text = if (isConnected) "Disconnect" else "Connect")
            }

            if (isConnected) {
                Text(
                    text = "Successfully connected.",
                    style = MaterialTheme.typography.body1,
                    modifier = Modifier.padding(16.dp)
                )

                Text(
                    text = "MTU: $mtu",
                    style = MaterialTheme.typography.body1,
                    modifier = Modifier.padding(16.dp)
                )

                peripheral.services.forEach { service ->
                    Text(
                        text = "Service: ${service.uuid}",
                        style = MaterialTheme.typography.body1,
                        modifier = Modifier.padding(16.dp)
                    )

                    service.characteristics.forEach { characteristic ->
                        Text(
                            text = "Characteristic: ${characteristic.uuid}",
                            style = MaterialTheme.typography.body2,
                            modifier = Modifier.padding(start = 32.dp)
                        )

                        Text(
                            text = "Capabilities: ${characteristic.capabilities.joinToString(", ")}",
                            style = MaterialTheme.typography.body2,
                            modifier = Modifier.padding(start = 32.dp)
                        )

                        characteristic.descriptors.forEach { descriptor ->
                            Text(
                                text = "Descriptor: ${descriptor.uuid}",
                                style = MaterialTheme.typography.body2,
                                modifier = Modifier.padding(start = 48.dp)
                            )
                        }
                    }
                }
            }
        }
    }
}

@Composable
fun ReadContent() {
    Text("Read")
}

@Composable
fun NotifyContent() {
    Text("Notify")
}


