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
    }
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

#[derive(Debug)]
pub enum BluetoothAddressType {
    Public,
    Random,
    Unspecified,
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

}

impl fmt::Display for BluetoothAddressType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            BluetoothAddressType::Public => write!(f, "Public"),
            BluetoothAddressType::Random => write!(f, "Random"),
            BluetoothAddressType::Unspecified => write!(f, "Unspecified"),
        }
    }
}

unsafe impl Sync for Adapter {}

unsafe impl Sync for Peripheral {}

unsafe impl Send for Adapter {}

unsafe impl Send for Peripheral {}

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
