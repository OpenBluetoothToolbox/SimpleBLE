#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** The maximum length of a UUID. */
#define SIMPLEBLE_UUID_STR_LEN 37  // 36 characters + null terminator
/** The maximum number of characteristics a service may include. */
#define SIMPLEBLE_CHARACTERISTIC_MAX_COUNT 16
/** The maximum number of descriptors a characteristic may have. */
#define SIMPLEBLE_DESCRIPTOR_MAX_COUNT 16
/** The maximum number of included services a service may have. */
#define SIMPLEBLE_INCLUDED_SERVICES_MAX_COUNT 7

/**
 * @brief Return values for SimpleBLE-C functions.
 * @todo Add proper error codes.
 */
typedef enum {
    SIMPLEBLE_SUCCESS = 0,
    SIMPLEBLE_FAILURE = 1,
} simpleble_err_t;

/**
 * @brief A type for representing a Bluetooth UUID.
 * @note This is a struct to allow easy extending in the future.
 */
typedef struct {
    /** UUID in string form. */
    char value[SIMPLEBLE_UUID_STR_LEN];
} simpleble_uuid_t;

/**
 * @brief A Bluetooth LE descriptor.
 * @sa Bluetooth Core Specification version 5.3 (Vol 3, Part G, Section 3.3)
 */
typedef struct {
    /** Descriptor UUID. */
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
    /** Indicates if the characteristic supports notifications. */
    bool can_notify;
    /** Indicates if the characteristic supports indications. */
    bool can_indicate;
    /** Indicates if the characteristic supports authenticated signed writes. */
    bool can_write_authenticated;
    /** Indicates if the characteristic supports broadcasting. */
    bool can_broadcast;
    /** Indicates if the characteristic has extended properties. */
    bool has_extended_properties;
    /** The actual number of descriptors the characteristic has.*/
    size_t descriptor_count;
    /** Every descriptor owned by the characteristic. */
    simpleble_descriptor_t descriptors[SIMPLEBLE_DESCRIPTOR_MAX_COUNT];
} simpleble_characteristic_t;

/**
 * @brief A Bluetooth LE Service.
 * @sa Bluetooth Core Specification version 5.3 (Vol 3, Part G, Section 3.1)
 */
typedef struct {
    /** Service UUID. */
    simpleble_uuid_t uuid;
    /** Actual service data size. */
    size_t data_length;
    /** Service data. */
    uint8_t data[27];
    /** Actual number of characteristics. */
    size_t characteristic_count;
    /** Array of characteristics. */
    simpleble_characteristic_t characteristics[SIMPLEBLE_CHARACTERISTIC_MAX_COUNT];
    /** Number of included services. */
    size_t included_services_count;
    /** Array of included services (as UUIDs). */
    simpleble_uuid_t included_services[SIMPLEBLE_INCLUDED_SERVICES_MAX_COUNT];
} simpleble_service_t;

/**
 * @brief Manufacturer-specific data.
 * @sa Bluetooth Core Specification version 5.3 (Vol 3, Part C, Section 11).
 */
typedef struct {
    /** Manufacturer ID. */
    uint16_t manufacturer_id;
    /** Actual data size. */
    size_t data_length;
    /** Data. */
    uint8_t data[27];
    // Note: The maximum length of a BLE advertisement is 31 bytes.
    // The first byte will be the length of the field,
    // the second byte will be the type of the field (0xFF for manufacturer data),
    // the next two bytes will be the manufacturer ID,
    // and the remaining 27 bytes are the manufacturer data.
} simpleble_manufacturer_data_t;

/** Handle to a SimpleBLE adapter. */
typedef void* simpleble_adapter_t;

/** Handle to a SimpleBLE peripheral. */
typedef void* simpleble_peripheral_t;

/** Host operating system. */
typedef enum {
    /** Windows. */
    SIMPLEBLE_OS_WINDOWS = 0,
    /** macOS. */
    SIMPLEBLE_OS_MACOS = 1,
    /** Linux. */
    SIMPLEBLE_OS_LINUX = 2,
} simpleble_os_t;

/** Address types. */
typedef enum {
    /** Public. */
    SIMPLEBLE_ADDRESS_TYPE_PUBLIC = 0,
    /** Random. */
    SIMPLEBLE_ADDRESS_TYPE_RANDOM = 1,
    /** Unknown. */
    SIMPLEBLE_ADDRESS_TYPE_UNSPECIFIED = 2,
} simpleble_address_type_t;
