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

TEST(RemoteProxyChildren, AppendChild) {
    RemoteProxy p = RemoteProxy(nullptr, "", "/a/b");

    p.path_add("/a/b/c", Holder());
    EXPECT_EQ(1, p.children().size());
    EXPECT_EQ(1, p.children().count("/a/b/c"));
    EXPECT_EQ("/a/b/c", p.children().at("/a/b/c")->path());
}

TEST(RemoteProxyChildren, AppendRepeatedChild) {
    RemoteProxy p = RemoteProxy(nullptr, "", "/a/b");
    p.path_add("/a/b/c", Holder());

    // Attempting to add a child with the same name should be a no-op
    p.path_add("/a/b/c", Holder());
    EXPECT_EQ(1, p.children().size());
}

TEST(RemoteProxyChildren, AppendExtendedChild) {
    RemoteProxy p = RemoteProxy(nullptr, "", "/");
    p.path_add("/a/b/c/d", Holder());

    ASSERT_EQ(1, p.children().size());
    ASSERT_EQ(1, p.children().count("/a"));

    std::shared_ptr<RemoteProxy> p_a = std::dynamic_pointer_cast<RemoteProxy>(p.children().at("/a"));
    ASSERT_EQ(1, p_a->children().size());
    ASSERT_EQ(1, p_a->children().count("/a/b"));

    std::shared_ptr<RemoteProxy> p_a_b = std::dynamic_pointer_cast<RemoteProxy>(p_a->children().at("/a/b"));
    ASSERT_EQ(1, p_a_b->children().size());
    ASSERT_EQ(1, p_a_b->children().count("/a/b/c"));

    std::shared_ptr<RemoteProxy> p_a_b_c = std::dynamic_pointer_cast<RemoteProxy>(p_a_b->children().at("/a/b/c"));
    ASSERT_EQ(1, p_a_b_c->children().size());
    ASSERT_EQ(1, p_a_b_c->children().count("/a/b/c/d"));
}

TEST(RemoteProxyChildren, RemoveSelf) {
    RemoteProxy p = RemoteProxy(nullptr, "", "/");

    // Should notify that the proxy can be safely deleted, as nothing worth keeping is left
    ASSERT_TRUE(p.path_remove("/", Holder::create_array()));

    // Add a child path without any interfaces
    p.path_add("/a", Holder());

    // Attempt to remove the path while holding a local copy of the child, should be a no-op
    {
        std::shared_ptr<RemoteProxy> p_a = std::dynamic_pointer_cast<RemoteProxy>(p.children().at("/a"));
        // As there is another copy of the child, the proxy should not be deleted
        ASSERT_FALSE(p.path_remove("/", Holder::create_array()));
        ASSERT_EQ(1, p.children().size());
    }

    // Should notify that the proxy can be safely deleted, as nothing worth keeping is left
    ASSERT_TRUE(p.path_remove("/", Holder::create_array()));
    ASSERT_EQ(0, p.children().size());
}

TEST(RemoteProxyChildren, RemoveChildNoInterfaces) {
    RemoteProxy p = RemoteProxy(nullptr, "", "/");
    p.path_add("/a", Holder());

    // Attempt to remove the path while holding a local copy of the child, should be a no-op
    {
        std::shared_ptr<RemoteProxy> p_a = std::dynamic_pointer_cast<RemoteProxy>(p.children().at("/a"));
        // As there is another copy of the child, the proxy should not be deleted
        ASSERT_FALSE(p.path_remove("/a", Holder::create_array()));
        ASSERT_EQ(1, p.children().size());
    }

    // As only the child was removed, the main proxy should not be deleted.
    ASSERT_FALSE(p.path_remove("/a", Holder::create_array()));
    ASSERT_EQ(0, p.children().size());
}

TEST(RemoteProxyChildren, RemoveChildWithInterfaces) {
    RemoteProxy p = RemoteProxy(nullptr, "", "/");

    Holder managed_interfaces = Holder::create_dict();
    managed_interfaces.dict_append(Holder::STRING, "i.1", Holder());
    managed_interfaces.dict_append(Holder::STRING, "i.2", Holder());
    p.path_add("/a", managed_interfaces);

    Holder removed_interfaces = Holder::create_array();
    removed_interfaces.array_append(Holder::create_string("i.2"));
    p.path_remove("/a", removed_interfaces);

    // Because /a has one interface still, it should still be in the children map.
    ASSERT_EQ(1, p.children().size());
    {
        std::shared_ptr<RemoteProxy> p_a = std::dynamic_pointer_cast<RemoteProxy>(p.children().at("/a"));
        ASSERT_EQ(1, count_interfaces(*p_a));
        ASSERT_EQ(1, p_a->interfaces().count("i.1"));
    }

    // Remove the second interface
    removed_interfaces = Holder::create_array();
    removed_interfaces.array_append(Holder::create_string("i.1"));
    p.path_remove("/a", removed_interfaces);
    ASSERT_EQ(0, p.children().size());
}
