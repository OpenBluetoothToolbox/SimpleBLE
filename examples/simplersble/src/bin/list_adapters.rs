use simplersble;

fn main() {
    println!("List adapters!");

    println!(
        "Bluetooth enabled: {}",
        simplersble::Adapter::bluetooth_enabled()
    );

    let mut adapters = simplersble::Adapter::get_adapters();
    println!("Adapters length is {}", adapters.len());

    for adapter in adapters.iter_mut() {
        println!("Adapter identifier is {}", adapter.identifier());
        println!("Adapter address is {}", adapter.address());
    }
}
