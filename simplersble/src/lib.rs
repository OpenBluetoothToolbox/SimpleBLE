use std::mem;


#[cxx::bridge]
mod ffi {

    #[namespace = "SimpleBLE"]
    struct RustyWrapper {
        internal: UniquePtr::<RustyAdapter>
    }

    unsafe extern "C++" {
        include!("simplersble/src/bindings/AdapterBindings.hpp");

        #[namespace = "SimpleBLE"]
        type RustyAdapter;

        fn RustyAdapter_bluetooth_enabled() -> bool;
        fn RustyAdapter_get_adapters() -> Vec::<RustyWrapper>;

        fn identifier(&self) -> String;


    }

}

pub struct Adapter {
    internal : cxx::UniquePtr::<ffi::RustyAdapter>,
}

impl Adapter {

    fn new(internal: cxx::UniquePtr::<ffi::RustyAdapter>) -> Self {
        Self { internal }
    }

    pub fn identifier(&self) -> String {
        return self.internal.identifier();
    }

    pub fn bluetooth_enabled() -> bool {
        return ffi::RustyAdapter_bluetooth_enabled();
    }

    pub fn get_adapters() -> Vec::<Adapter> {
        let mut adapters = Vec::<Adapter>::new();

        for adapter_wrapper in ffi::RustyAdapter_get_adapters().iter_mut() {
            let mut carrier = cxx::UniquePtr::<ffi::RustyAdapter>::null();

            // Swap the wrapper object to extract the internal pointer that we desire.
            mem::swap(&mut carrier, &mut adapter_wrapper.internal);

            adapters.push(Adapter::new(carrier));
        }

        return adapters;
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
