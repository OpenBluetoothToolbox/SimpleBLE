package org.simpleble.android

import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

class Peripheral private constructor() {

    private val _onConnected = MutableSharedFlow<Unit>()
    private val _onDisconnected = MutableSharedFlow<Unit>()

    fun identifier(): String {
        return ""
    }

    fun address(): BluetoothAddress {
        return BluetoothAddress()
    }

    fun address_type(): BluetoothAddressType {
        return BluetoothAddressType()
    }

    fun rssi(): Int {
        return 0
    }

    fun tx_power(): Int {
        return 0
    }

    fun mtu(): Int {
        return 0
    }

    fun connect() {
    }

    fun disconnect() {
    }

    fun is_connected(): Boolean {
        return false
    }

    fun is_connectable(): Boolean {
        return false
    }

    fun is_paired(): Boolean {
        return false
    }

    fun unpair() {
    }

    fun services(): List<Service> {
        return emptyList()
    }

    fun manufacturer_data(): Map<Int, ByteArray> {
        return emptyMap()
    }

    fun read(service: BluetoothUUID, characteristic: BluetoothUUID): ByteArray {
        return ByteArray(0)
    }

    fun write_request(service: BluetoothUUID, characteristic: BluetoothUUID, data: ByteArray) {
    }

    fun write_command(service: BluetoothUUID, characteristic: BluetoothUUID, data: ByteArray) {
    }

    fun notify(
        service: BluetoothUUID,
        characteristic: BluetoothUUID
    ): StateFlow<ByteArray> {
        val payloadFlow = MutableStateFlow(ByteArray(0))

        // Create a callback that emits the received payload to the StateFlow
        val callback = { payload: ByteArray ->
            payloadFlow.value = payload
        }

        // Register the callback with the Bluetooth stack
        // (assuming you have a function to do that)
        //registerBluetoothCallback(service, characteristic, callback)

        return payloadFlow.asStateFlow()
    }

    fun indicate(
        service: BluetoothUUID,
        characteristic: BluetoothUUID
    ): StateFlow<ByteArray> {
        val payloadFlow = MutableStateFlow(ByteArray(0))

        // Create a callback that emits the received payload to the StateFlow
        val callback = { payload: ByteArray ->
            payloadFlow.value = payload
        }

        // Register the callback with the Bluetooth stack
        // (assuming you have a function to do that)
        //registerBluetoothCallback(service, characteristic, callback)

        return payloadFlow.asStateFlow()
    }

    fun unsubscribe(service: BluetoothUUID, characteristic: BluetoothUUID) {
    }

    fun read(
        service: BluetoothUUID,
        characteristic: BluetoothUUID,
        descriptor: BluetoothUUID
    ): ByteArray {
        return ByteArray(0)
    }

    fun write(
        service: BluetoothUUID,
        characteristic: BluetoothUUID,
        descriptor: BluetoothUUID,
        data: ByteArray
    ) {
    }

    val onConnected get() = _onConnected

    val onDisconnected get() = _onDisconnected

}