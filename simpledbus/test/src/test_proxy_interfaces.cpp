#include <gtest/gtest.h>

#include <simpledbus/advanced/RemoteProxy.h>

using namespace SimpleDBus;

static size_t count_interfaces(RemoteProxy& proxy) {
    size_t count = 0;
    for (auto& [iface_name, interface] : proxy.interfaces()) {
        if (interface->is_loaded()) {
            count++;
        }
    }
    return count;
}

TEST(RemoteProxyInterfaces, LoadInterfaces) {
    Holder managed_interfaces = Holder::create_dict();
    managed_interfaces.dict_append(Holder::STRING, "i.1", Holder());

    RemoteProxy h = RemoteProxy(nullptr, "", "/");
    EXPECT_FALSE(h.interfaces_loaded());

    // TODO: Check that all properties of the interface are correctly loaded.

    h.interfaces_load(managed_interfaces);

    EXPECT_TRUE(h.interfaces_loaded());
    EXPECT_EQ(1, h.interfaces().size());
    EXPECT_EQ(1, h.interfaces().count("i.1"));
}

TEST(RemoteProxyInterfaces, UnloadInterfaces) {
    Holder managed_interfaces = Holder::create_dict();
    managed_interfaces.dict_append(Holder::STRING, "i.1", Holder());
    managed_interfaces.dict_append(Holder::STRING, "i.2", Holder());
    managed_interfaces.dict_append(Holder::STRING, "i.3", Holder());

    RemoteProxy h = RemoteProxy(nullptr, "", "/");
    h.interfaces_load(managed_interfaces);
    EXPECT_EQ(3, count_interfaces(h));

    Holder removed_interfaces = Holder::create_array();
    removed_interfaces.array_append(Holder::create_string("i.3"));
    h.interfaces_unload(removed_interfaces);

    EXPECT_EQ(2, count_interfaces(h));
    EXPECT_TRUE(h.interfaces_loaded());
    EXPECT_FALSE(h.interfaces().at("i.3")->is_loaded());

    removed_interfaces = Holder::create_array();
    removed_interfaces.array_append(Holder::create_string("i.2"));
    h.interfaces_unload(removed_interfaces);

    EXPECT_EQ(1, count_interfaces(h));
    EXPECT_TRUE(h.interfaces_loaded());
    EXPECT_FALSE(h.interfaces().at("i.2")->is_loaded());

    removed_interfaces = Holder::create_array();
    removed_interfaces.array_append(Holder::create_string("i.1"));
    h.interfaces_unload(removed_interfaces);

    EXPECT_EQ(0, count_interfaces(h));
    EXPECT_FALSE(h.interfaces_loaded());
    EXPECT_FALSE(h.interfaces().at("i.1")->is_loaded());
}