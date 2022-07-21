#pragma once

#include <cstdint>

namespace
{
    struct IsPeerExist
    {
        template <class EVT, class FSM, class SourceState, class TargetState>
        bool operator()(EVT const &evt, FSM &fsm, SourceState &src, TargetState &tgt)
        {
            return esp_now_is_peer_exist(reinterpret_cast<const uint8_t *>(evt.mac.data()));
        }
    };
    
    struct IsInPeeringModeAndPeerNotExist
    {
        template <class EVT, class FSM, class SourceState, class TargetState>
        bool operator()(EVT const &evt, FSM &fsm, SourceState &, TargetState &)
        {
            return fsm.PeeringMode && !esp_now_is_peer_exist(reinterpret_cast<const uint8_t *>(evt.mac.data()));
        }
    };
}