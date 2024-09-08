#include <sdbusplus/bus.hpp>
#include <iostream>
#include <string>
#include <tuple>

// 讀取回應的helper函數
template<typename Tuple, std::size_t... Is>
void read_reply(sdbusplus::message::message& reply, Tuple& data, std::index_sequence<Is...>)
{
    (reply.read(std::get<Is>(data)), ...);
}

template<typename... Args>
std::tuple<Args...> call_method(sdbusplus::bus::bus& bus, const std::string& service,
                                const std::string& object_path, const std::string& interface,
                                const std::string& method)
{
    auto method_call = bus.new_method_call(service.c_str(), object_path.c_str(), interface.c_str(), method.c_str());

    // 呼叫方法並獲取回應
    auto reply = bus.call(method_call);

    std::tuple<Args...> result;
    read_reply(reply, result, std::index_sequence_for<Args...>{});
    return result;
}

int main()
{
    // 連接到系統總線
    auto bus = sdbusplus::bus::new_default();

    // 設定目標服務、物件路徑和介面
    std::string service = "org.freedesktop.DBus";       // 替換為目標服務名稱
    std::string object_path = "/org/freedesktop/DBus";  // 替換為目標物件路徑
    std::string interface = "org.freedesktop.DBus";     // 替換為目標介面名稱
    std::string method = "ListNames";                   // 替換為目標方法名稱

    try
    {
        // 呼叫方法並獲取多類型數據
        auto result = call_method<std::vector<std::string>>(bus, service, object_path, interface, method);

        // 輸出結果
        const auto& names = std::get<0>(result);
        std::cout << "Method call result:\n";
        for (const auto& name : names)
        {
            std::cout << name << "\n";
        }
    }
    catch (const sdbusplus::exception::SdBusError& e)
    {
        std::cerr << "Error calling method: " << e.what() << std::endl;
    }

    return 0;
}

