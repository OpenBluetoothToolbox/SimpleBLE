use simplersble;

fn main() {

    let x = simplersble::add(4, 3);

    let adapters = simplersble::Adapter::get_adapters();
    println!("Adapters length is {}", adapters.len());

    for adapter in adapters.iter() {
        println!("Adapter identifier is {}", adapter.identifier());
    }

    println!("X was {} - Z was {}", x, simplersble::Adapter::bluetooth_enabled());

}
