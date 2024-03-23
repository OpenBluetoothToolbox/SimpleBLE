#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SIMPLEBLE_UUID_STR_LEN 37  // 36 characters + null terminator
#define SIMPLEBLE_CHARACTERISTIC_MAX_COUNT 16
#define SIMPLEBLE_DESCRIPTOR_MAX_COUNT 16

// TODO: Add proper error codes.
typedef enum {
    SIMPLEBLE_SUCCESS = 0,
    SIMPLEBLE_FAILURE = 1,
} simpleble_err_t;

typedef struct {
    char value[SIMPLEBLE_UUID_STR_LEN];
} simpleble_uuid_t;

typedef struct {
    simpleble_uuid_t uuid;
} simpleble_descriptor_t;

/**
 * @brief A Bluetooth LE characteristic.
 * @sa Bluetooth Core Specification version 5.3 (Vol 3, Part G, Section 3.3)
 */
typedef struct {
    /** Characteristic UUID. */
    simpleble_uuid_t uuid;
    /** Indicates if the characteristic supports reading. */
    bool can_read;
    /** Indicates if the characteristic supports write operations with a response. */
    bool can_write_request;
    /** Indicates if the characteristic supports write operations with a response. */
    bool can_write_command;
    /** Indicates if the characteristic supports authenticated signed writes. */
    bool can_write_authenticated;
    /** Indicates if the characteristic supports notifications. */
    bool can_notify;
    /** Indicates if the characteristic supports indications. */
    bool can_indicate;
    /** Indicates if the characteristic supports broadcasting. */
    bool can_broadcast;
    /** Indicates if the characteristic has extended properties. */
    bool has_extended_properties;
    /** The actual number of descriptors the characteristic has.*/
    size_t descriptor_count;
    /** Every descriptor owned by the characteristic. */
    simpleble_descriptor_t descriptors[SIMPLEBLE_DESCRIPTOR_MAX_COUNT];
} simpleble_characteristic_t;

typedef struct {
    simpleble_uuid_t uuid;
    size_t data_length;
    uint8_t data[27];
    // Note: The maximum length of a BLE advertisement is 31 bytes.
    // The first byte will be the length of the field,
    // the second byte will be the type of the field,
    // the next two bytes will be the service UUID,
    // and the remaining 27 bytes are the manufacturer data.
    size_t characteristic_count;
    simpleble_characteristic_t characteristics[SIMPLEBLE_CHARACTERISTIC_MAX_COUNT];
} simpleble_service_t;

typedef struct {
    uint16_t manufacturer_id;
    size_t data_length;
    uint8_t data[27];
    // Note: The maximum length of a BLE advertisement is 31 bytes.
    // The first byte will be the length of the field,
    // the second byte will be the type of the field (0xFF for manufacturer data),
    // the next two bytes will be the manufacturer ID,
    // and the remaining 27 bytes are the manufacturer data.
} simpleble_manufacturer_data_t;

typedef void* simpleble_adapter_t;
typedef void* simpleble_peripheral_t;

typedef enum {
    SIMPLEBLE_OS_WINDOWS = 0,
    SIMPLEBLE_OS_MACOS = 1,
    SIMPLEBLE_OS_LINUX = 2,
} simpleble_os_t;

typedef enum {
    SIMPLEBLE_ADDRESS_TYPE_PUBLIC = 0,
    SIMPLEBLE_ADDRESS_TYPE_RANDOM = 1,
    SIMPLEBLE_ADDRESS_TYPE_UNSPECIFIED = 2,
} simpleble_address_type_t;
