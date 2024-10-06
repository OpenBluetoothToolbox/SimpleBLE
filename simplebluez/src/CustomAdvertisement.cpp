#include <simplebluez/CustomAdvertisement.h>
#include <simpledbus/interfaces/ObjectManager.h>

using namespace SimpleBluez;

CustomAdvertisement::CustomAdvertisement(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {

    _interfaces.emplace(std::make_pair("org.bluez.LEAdvertisement1", std::make_shared<SimpleBluez::LEAdvertisement1>(conn, this)));

}

std::shared_ptr<SimpleBluez::LEAdvertisement1> CustomAdvertisement::le_advertisement1() {
    return std::dynamic_pointer_cast<SimpleBluez::LEAdvertisement1>(interface_get("org.bluez.LEAdvertisement1"));
}
