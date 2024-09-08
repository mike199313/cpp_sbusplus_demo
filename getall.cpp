#include <sdbusplus/bus.hpp>
#include <iostream>
#include <map>
#include <variant>
#include <vector>

// Type alias for the variant type used in D-Bus responses
using DBusVariant = std::variant<std::vector<std::string>>;

// Function to parse and print the properties
void parseDBusProperties(const std::map<std::string, DBusVariant>& properties) {
    for (const auto& [key, value] : properties) {
        std::cout << "Key: " << key << "\n";
        if (auto vec = std::get_if<std::vector<std::string>>(&value)) {
            std::cout << "Value: [";
            for (const auto& str : *vec) {
                std::cout << str << " ";
            }
            std::cout << "]\n";
        } else {
            std::cout << "Unhandled variant type\n";
        }
    }
}

int main() {
    try {
        // Create a connection to the system bus
        auto bus = sdbusplus::bus::new_default();

        // Create a new method call for GetAll properties
        auto method = bus.new_method_call("org.freedesktop.DBus",
                                          "/org/freedesktop/DBus",
                                          "org.freedesktop.DBus.Properties",
                                          "GetAll");

        // Append the interface name to the method call
        method.append("org.freedesktop.DBus");

        // Call the method and get the reply
        auto reply = bus.call(method);

        // Parse the reply
        std::map<std::string, DBusVariant> properties;
        reply.read(properties);

        // Parse and print the D-Bus properties
        parseDBusProperties(properties);
    } catch (const sdbusplus::exception::SdBusError& e) {
        std::cerr << "D-Bus error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}

