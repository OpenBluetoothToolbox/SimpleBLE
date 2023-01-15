use simplersble;

fn main() {

    println!(
        "Bluetooth enabled: {}",
        simplersble::Adapter::bluetooth_enabled()
    );

    let mut adapters = simplersble::Adapter::get_adapters();

    // If the adapter list is empty, print a message and exit
    if adapters.is_empty() {
        println!("No adapters found.");
        return;
    }

    for adapter in adapters.iter_mut() {
        println!("Adapter: {} [{}]", adapter.identifier(), adapter.address());
    }
}
