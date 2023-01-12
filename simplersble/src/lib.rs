use std::mem;
use std::pin::Pin;
use std::fmt;

#[cxx::bridge]
mod ffi {

    #[namespace = "SimpleBLE"]
    struct RustyAdapterWrapper {
        internal: UniquePtr<RustyAdapter>,
    }

    #[namespace = "SimpleBLE"]
    struct RustyPeripheralWrapper {
        internal: UniquePtr<RustyPeripheral>,
    }

    #[namespace = "SimpleBLE"]
    struct RustyServiceWrapper {
        internal: UniquePtr<RustyService>,
    }

    #[namespace = "SimpleBLE"]
    struct RustyCharacteristicWrapper {
        internal: UniquePtr<RustyCharacteristic>,
    }

    #[namespace = "SimpleBLE"]
    struct RustyDescriptorWrapper {
        internal: UniquePtr<RustyDescriptor>,
    }

    #[namespace = "SimpleBLE"]
    #[repr(i32)]
    enum BluetoothAddressType {
        PUBLIC,
        RANDOM,
        UNSPECIFIED,
    }

    #[namespace = "SimpleRsBLE"]
    extern "Rust" {
        type Adapter;

        fn on_callback_scan_start(self: &mut Adapter);
        fn on_callback_scan_stop(self: &mut Adapter);
        fn on_callback_scan_updated(self: &mut Adapter, peripheral: &mut RustyPeripheralWrapper);
        fn on_callback_scan_found(self: &mut Adapter, peripheral: &mut RustyPeripheralWrapper);
     
        type Peripheral;
    }

    unsafe extern "C++" {
        include!("src/bindings/AdapterBindings.hpp");

        #[namespace = "SimpleBLE"]
        type BluetoothAddressType;

        #[namespace = "SimpleBLE"]
        type RustyAdapter;

        fn RustyAdapter_bluetooth_enabled() -> bool;
        fn RustyAdapter_get_adapters() -> Vec<RustyAdapterWrapper>;

        fn link(self: &RustyAdapter, target: Pin<&mut Adapter>);
        fn unlink(self: &RustyAdapter);

        fn identifier(self: &RustyAdapter) -> String;
        fn address(self: &RustyAdapter) -> String;

        fn scan_start(self: &RustyAdapter);
        fn scan_stop(self: &RustyAdapter);
        fn scan_for(self: &RustyAdapter, timeout_ms: i32);
        fn scan_is_active(self: &RustyAdapter) -> bool;
        fn scan_get_results(self: &RustyAdapter) -> Vec<RustyPeripheralWrapper>;

        fn get_paired_peripherals(self: &RustyAdapter) -> Vec<RustyPeripheralWrapper>;

        #[namespace = "SimpleBLE"]
        type RustyPeripheral;

        fn link(self: &RustyPeripheral, target: Pin<&mut Peripheral>);
        fn unlink(self: &RustyPeripheral);

        fn identifier(self: &RustyPeripheral) -> String;
        fn address(self: &RustyPeripheral) -> String;
        fn address_type(self: &RustyPeripheral) -> BluetoothAddressType;
        fn rssi(self: &RustyPeripheral) -> i16;

        // Implement the C++ functions below in Rust
        fn tx_power(self: &RustyPeripheral) -> i16;
        fn mtu(self: &RustyPeripheral) -> u16;

        fn connect(self: &RustyPeripheral);
        fn disconnect(self: &RustyPeripheral);
        fn is_connected(self: &RustyPeripheral) -> bool;
        fn is_connectable(self: &RustyPeripheral) -> bool;
        fn is_paired(self: &RustyPeripheral) -> bool;
        fn unpair(self: &RustyPeripheral);

        fn services(self: &RustyPeripheral) -> Vec<RustyServiceWrapper>;

        #[namespace = "SimpleBLE"]
        type RustyService;

        fn uuid(self: &RustyService) -> String;
        fn characteristics(self: &RustyService) -> Vec<RustyCharacteristicWrapper>;

        #[namespace = "SimpleBLE"]
        type RustyCharacteristic;

        fn uuid(self: &RustyCharacteristic) -> String;
        fn descriptors(self: &RustyCharacteristic) -> Vec<RustyDescriptorWrapper>;

        fn can_read(self: &RustyCharacteristic) -> bool;
        fn can_write_request(self: &RustyCharacteristic) -> bool;
        fn can_write_command(self: &RustyCharacteristic) -> bool;
        fn can_notify(self: &RustyCharacteristic) -> bool;
        fn can_indicate(self: &RustyCharacteristic) -> bool;

        #[namespace = "SimpleBLE"]
        type RustyDescriptor;

        fn uuid(self: &RustyDescriptor) -> String;
    }
}

#[derive(Debug)]
pub enum BluetoothAddressType {
    Public,
    Random,
    Unspecified,
}

#[derive(Debug)]
pub enum CharacteristicCapability {
    Read,
    WriteRequest,
    WriteCommand,
    Notify,
    Indicate,
}

pub struct Adapter {
    internal: cxx::UniquePtr<ffi::RustyAdapter>,
    on_scan_start: Box<dyn Fn() + Send + Sync + 'static>,
    on_scan_stop: Box<dyn Fn() + Send + Sync + 'static>,
    on_scan_found: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>,
    on_scan_updated: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>,
}

pub struct Peripheral {
    internal: cxx::UniquePtr<ffi::RustyPeripheral>,
}

pub struct Service {
    internal: cxx::UniquePtr<ffi::RustyService>,
}

pub struct Characteristic {
    internal: cxx::UniquePtr<ffi::RustyCharacteristic>,
}

pub struct Descriptor {
    internal: cxx::UniquePtr<ffi::RustyDescriptor>,
}

impl Adapter {
    pub fn bluetooth_enabled() -> bool {
        return ffi::RustyAdapter_bluetooth_enabled();
    }

    pub fn get_adapters() -> Vec<Pin<Box<Adapter>>> {
        let mut adapters = Vec::<Pin<Box<Adapter>>>::new();

        for adapter_wrapper in ffi::RustyAdapter_get_adapters().iter_mut() {
            adapters.push(Adapter::new(adapter_wrapper));
        }

        return adapters;
    }

    fn new(wrapper: &mut ffi::RustyAdapterWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyAdapter>::null(),
            on_scan_start: Box::new(||{}),
            on_scan_stop: Box::new(||{}),
            on_scan_found: Box::new(|_|{}),
            on_scan_updated: Box::new(|_|{}),
        };

        // Pin the object to guarantee that its location in memory is
        // fixed throughout the lifetime of the application
        let mut this_boxed = Box::pin(this);

        // Link `this` to the RustyAdapter
        wrapper.internal.link(this_boxed.as_mut());

        // Copy the RustyAdapter pointer into `this`
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn identifier(&self) -> String {
        return self.internal.identifier();
    }

    pub fn address(&self) -> String {
        return self.internal.address();
    }

    pub fn scan_start(&self) {
        self.internal.scan_start();
    }

    pub fn scan_stop(&self) {
        self.internal.scan_stop();
    }

    pub fn scan_for(&self, timeout_ms: i32) {
        self.internal.scan_for(timeout_ms);
    }

    pub fn scan_is_active(&self) {
        self.internal.scan_is_active();
    }

    pub fn scan_get_results(&self) -> Vec<Pin<Box<Peripheral>>> {
        let mut peripherals = Vec::<Pin<Box<Peripheral>>>::new();

        for peripheral_wrapper in self.internal.scan_get_results().iter_mut() {
            peripherals.push(Peripheral::new(peripheral_wrapper));
        }

        return peripherals;
    }

    pub fn get_paired_peripherals(&self) -> Vec<Pin<Box<Peripheral>>> {
        let mut peripherals = Vec::<Pin<Box<Peripheral>>>::new();

        for peripheral_wrapper in self.internal.get_paired_peripherals().iter_mut() {
            peripherals.push(Peripheral::new(peripheral_wrapper));
        }

        return peripherals;
    }

    pub fn set_callback_on_scan_start(&mut self, cb: Box<dyn Fn() + Send + Sync + 'static>) {
        self.on_scan_start = cb;
    }

    pub fn set_callback_on_scan_stop(&mut self, cb: Box<dyn Fn() + Send + Sync + 'static>) {
        self.on_scan_stop = cb;
    }

    pub fn set_callback_on_scan_updated(&mut self, cb: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>) {
        self.on_scan_updated = cb;
    }

    pub fn set_callback_on_scan_found(&mut self, cb: Box<dyn Fn(Pin<Box<Peripheral>>) + Send + Sync + 'static>) {
        self.on_scan_found = cb;
    }

    fn on_callback_scan_start(&self) {
        (self.on_scan_start)();
    }

    fn on_callback_scan_stop(&self) {
        (self.on_scan_stop)();
    }

    fn on_callback_scan_updated(&self, peripheral: &mut ffi::RustyPeripheralWrapper) {
        (self.on_scan_updated)(Peripheral::new(peripheral));
    }

    fn on_callback_scan_found(&self, peripheral: &mut ffi::RustyPeripheralWrapper) {
        (self.on_scan_found)(Peripheral::new(peripheral));
    }
}

impl Peripheral {
    fn new(wrapper: &mut ffi::RustyPeripheralWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyPeripheral>::null(),
        };

        // Pin the object to guarantee that its location in memory is
        // fixed throughout the lifetime of the application
        let mut this_boxed = Box::pin(this);

        // Link `this` to the RustyPeripheral
        wrapper.internal.link(this_boxed.as_mut());

        // Copy the RustyPeripheral pointer into `this`
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn identifier(&self) -> String {
        return self.internal.identifier();
    }

    pub fn address(&self) -> String {
        return self.internal.address();
    }

    pub fn address_type(&self) -> BluetoothAddressType {
        return match self.internal.address_type() {
            ffi::BluetoothAddressType::PUBLIC => BluetoothAddressType::Public,
            ffi::BluetoothAddressType::RANDOM => BluetoothAddressType::Random,
            ffi::BluetoothAddressType::UNSPECIFIED => BluetoothAddressType::Unspecified,
            _ => BluetoothAddressType::Unspecified,
        };
    }

    pub fn rssi(&self) -> i16 {
        return self.internal.rssi();
    }

    pub fn tx_power(&self) -> i16 {
        return self.internal.tx_power();
    }

    pub fn mtu(&self) -> u16 {
        return self.internal.mtu();
    }

    pub fn connect(&self) {
        self.internal.connect();
    }

    pub fn disconnect(&self) {
        self.internal.disconnect();
    }

    pub fn is_connected(&self) -> bool {
        return self.internal.is_connected();
    }

    pub fn is_connectable(&self) -> bool {
        return self.internal.is_connectable();
    }

    pub fn is_paired(&self) -> bool {
        return self.internal.is_paired();
    }

    pub fn unpair(&self) {
        self.internal.unpair();
    }

    pub fn services(&self) -> Vec<Pin<Box<Service>>> {
        let mut services = Vec::<Pin<Box<Service>>>::new();

        for service_wrapper in self.internal.services().iter_mut() {
            services.push(Service::new(service_wrapper));
        }

        return services;
    }
}

impl Service {
    fn new(wrapper: &mut ffi::RustyServiceWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyService>::null(),
        };

        // Pin the object to guarantee that its location in memory is
        // fixed throughout the lifetime of the application
        let mut this_boxed = Box::pin(this);

        // Copy the RustyService pointer into `this`
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn uuid(&self) -> String {
        return self.internal.uuid();
    }

    pub fn characteristics(&self) -> Vec<Pin<Box<Characteristic>>> {
        let mut characteristics = Vec::<Pin<Box<Characteristic>>>::new();

        for characteristic_wrapper in self.internal.characteristics().iter_mut() {
            characteristics.push(Characteristic::new(characteristic_wrapper));
        }

        return characteristics;
    }
}

impl Characteristic {
    fn new(wrapper: &mut ffi::RustyCharacteristicWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyCharacteristic>::null(),
        };

        // Pin the object to guarantee that its location in memory is
        // fixed throughout the lifetime of the application
        let mut this_boxed = Box::pin(this);

        // Copy the RustyCharacteristic pointer into `this`
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn uuid(&self) -> String {
        return self.internal.uuid();
    }

    pub fn descriptors(&self) -> Vec<Pin<Box<Descriptor>>> {
        let mut descriptors = Vec::<Pin<Box<Descriptor>>>::new();

        for descriptor_wrapper in self.internal.descriptors().iter_mut() {
            descriptors.push(Descriptor::new(descriptor_wrapper));
        }

        return descriptors;
    }

    pub fn capabilities(&self) -> Vec::<CharacteristicCapability> {
        let mut capabilities = Vec::<CharacteristicCapability>::new();

        if self.internal.can_read() {
            capabilities.push(CharacteristicCapability::Read);
        }

        if self.internal.can_write_request() {
            capabilities.push(CharacteristicCapability::WriteRequest);
        }

        if self.internal.can_write_command() {
            capabilities.push(CharacteristicCapability::WriteCommand);
        }

        if self.internal.can_notify() {
            capabilities.push(CharacteristicCapability::Notify);
        }

        if self.internal.can_indicate() {
            capabilities.push(CharacteristicCapability::Indicate);
        }

        return capabilities;
    }

    pub fn can_read(&self) -> bool {
        return self.internal.can_read();
    }

    pub fn can_write_request(&self) -> bool {
        return self.internal.can_write_request();
    }

    pub fn can_write_command(&self) -> bool {
        return self.internal.can_write_command();
    }

    pub fn can_notify(&self) -> bool {
        return self.internal.can_notify();
    }

    pub fn can_indicate(&self) -> bool {
        return self.internal.can_indicate();
    }


}

impl Descriptor {
    fn new(wrapper: &mut ffi::RustyDescriptorWrapper) -> Pin<Box<Self>> {
        let this = Self {
            internal: cxx::UniquePtr::<ffi::RustyDescriptor>::null(),
        };

        // Pin the object to guarantee that its location in memory is
        // fixed throughout the lifetime of the application
        let mut this_boxed = Box::pin(this);

        // Copy the RustyDescriptor pointer into `this`
        mem::swap(&mut this_boxed.internal, &mut wrapper.internal);

        return this_boxed;
    }

    pub fn uuid(&self) -> String {
        return self.internal.uuid();
    }
}



unsafe impl Sync for Adapter {}

unsafe impl Sync for Peripheral {}

unsafe impl Sync for Service {}

unsafe impl Sync for Characteristic {}

unsafe impl Sync for Descriptor {}

unsafe impl Send for Adapter {}

unsafe impl Send for Peripheral {}

unsafe impl Send for Service {}

unsafe impl Send for Characteristic {}

unsafe impl Send for Descriptor {}

impl fmt::Display for BluetoothAddressType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            BluetoothAddressType::Public => write!(f, "Public"),
            BluetoothAddressType::Random => write!(f, "Random"),
            BluetoothAddressType::Unspecified => write!(f, "Unspecified"),
        }
    }
}

impl fmt::Display for CharacteristicCapability {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            CharacteristicCapability::Read => write!(f, "Read"),
            CharacteristicCapability::WriteRequest => write!(f, "WriteRequest"),
            CharacteristicCapability::WriteCommand => write!(f, "WriteCommand"),
            CharacteristicCapability::Notify => write!(f, "Notify"),
            CharacteristicCapability::Indicate => write!(f, "Indicate"),
        }
    }
}

impl Drop for Adapter {
    fn drop(&mut self) {
        self.internal.unlink();
    }
}

impl Drop for Peripheral {
    fn drop(&mut self) {
        self.internal.unlink();
    }
}
