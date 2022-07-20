#pragma once

#include <cstdint>

namespace
{
    struct addPeer
    {
        template <class EVT, class FSM, class SourceState, class TargetState>
        void operator()(EVT const &evt, FSM &, SourceState &, TargetState &)
        {
            std::cout << "addPeer" << std::endl;
            esp_now_peer_info_t peerInfo = {};
            ;
            // Register peer
            memcpy(peerInfo.peer_addr, evt.mac.data(), evt.mac.size());
            peerInfo.channel = 0;
            peerInfo.encrypt = false;

            // Add peer
            if (esp_now_add_peer(&peerInfo) != ESP_OK)
            {
                Serial.println("Failed to add peer");
                return;
            }
        }
    };
    
    struct PrintIncomingPackage
    {
        struct struct_message
        {
            uint32_t type;
            uint32_t value;
        };

        template <class EVT, class FSM, class SourceState, class TargetState>
        void operator()(EVT const &evt, FSM &, SourceState &, TargetState &)
        {
            auto data{evt.data};
            auto recMac{evt.mac.data()};
            struct_message myData;
            memcpy(&myData, data.data(), sizeof(myData));

            Serial.print("Bytes received: ");
            Serial.println(data.size());
            Serial.print("From: ");
            char macStr[18] = {0};
            sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", recMac[0], recMac[1], recMac[2], recMac[3], recMac[4], recMac[5]);
            Serial.println(macStr);
            Serial.print("Type: ");
            Serial.println(myData.type);
            Serial.println();
        }
    };
}
