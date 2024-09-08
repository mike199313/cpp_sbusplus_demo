#include <iostream>
#include <string>
#include <stdio.h>
#include <cassert>
#include <sdbusplus/message.hpp>
#include <sdbusplus/bus.hpp>

int main() {
    // 連接到系統總線
    auto bus = sdbusplus::bus::new_default();

    // 設定目標服務、物件路徑和介面
    std::string service = "org.freedesktop.DBus";  // 替換為目標服務名稱
    std::string object_path = "/org/freedesktop/DBus";  // 替換為目標物件路徑
    std::string interface = "org.freedesktop.DBus";  // 替換為目標介面名稱
    std::string method = "GetId";  // 替換為目標方法名稱

    // 建立一個method call
    auto method_call = bus.new_method_call(service.c_str(), object_path.c_str(), interface.c_str(), method.c_str());

    try {
        // 呼叫方法並獲取回應
        auto reply = bus.call(method_call);

        // 解析回應（此處假設回應是一個單一的字串，根據實際情況修改）
        std::string result;
        reply.read(result);

        // 輸出結果
        std::cout << "Method call result: " << result << std::endl;
    } catch (const sdbusplus::exception::SdBusError& e) {
        std::cerr << "Error calling method: " << e.what() << std::endl;
    }

    return 0;
}

