use simplersble;

fn main() {

    println!("Bluetooth enabled: {}", simplersble::Adapter::bluetooth_enabled());

    let adapters = simplersble::Adapter::get_adapters();
    println!("Adapters length is {}", adapters.len());

    for adapter in adapters.iter() {
        println!("Adapter identifier is {}", adapter.identifier());
        println!("Adapter address is {}", adapter.address());

        adapter.scan_for(3000);

        println!("Scan complete");
    }




}
