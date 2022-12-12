use cxx;

#[cxx::bridge]
mod ffi {

    unsafe extern "C++" {
        include!("simplersble/src/bindings/AdapterBindings.hpp");

        #[namespace = "SimpleBLE"]
        type Adapter;

        fn Adapter_bluetooth_enabled() -> bool;
        fn Adapter_get_adapters_count() -> i32;

        // THIS FUNCTION DOES NOT WORK, UNSURE WHY
        fn Adapter_get_adapters_from_index(index: i32) -> cxx::UniquePtr<Adapter>;

    }
}

pub struct Adapter {
    internal : ffi::Adapter,
}

impl Adapter {

    fn new(internal: ffi::Adapter) -> Self {
        Self { internal }
    }

    pub fn identifier() -> String {
        return "AdapterIdentifier".to_string();
    }

    pub fn bluetooth_enabled() -> bool {
        return ffi::Adapter_bluetooth_enabled();
    }

    pub fn get_adapters() -> Vec::<Adapter> {
        return vec![];
    }

}

pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
