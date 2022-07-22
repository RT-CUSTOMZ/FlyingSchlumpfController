#pragma once

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 20

#include <vector>
#include <string_view>
#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
// front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>

#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

#include "config.h"

#include "actions.h"
#include "guards.h"

#include "MotorControl.h"

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// // for And_ operator
// using namespace msm::front::euml;

namespace // Concrete FSM implementation
{
    // events
    struct Trigger
    {
        std::string_view mac;
        std::string_view data;
    };
    struct Timeout
    {
    };
    struct ButtonTrigger
    {
    };


    // front-end: define the FSM structure
    struct player_ : public msm::front::state_machine_def<player_>
    {
        typedef int no_exception_thrown;
        typedef int no_message_queue;

        bool PeeringMode = true;

        StaticTimer_t pxTimerBuffer;
        TimerHandle_t xTimer;
        static void vTimerCallback(TimerHandle_t pxTimer)
        {
            // pxTimer
            auto self = reinterpret_cast<player_ *> (pvTimerGetTimerID(pxTimer));
            msm::back::state_machine<player_> &fsm = static_cast<msm::back::state_machine<player_> &>(*self);
            fsm.process_event(Timeout());
        }

        player_(const player_&) = delete;

        player_()
        {
            xTimer = xTimerCreateStatic("Timer",                 // Just a text name, not used by the kernel.
                                  (1000),                  // The timer period in ticks.
                                  pdFALSE,                 // The timers will auto-reload themselves when they expire.
                                  this,                    // Assign each timer a unique id equal to its array index.
                                  this->vTimerCallback, // Each timer calls the same callback when it expires.
                                  &this->pxTimerBuffer
            );
        }


        // The list of FSM states
        struct Init : public msm::front::state<>
        {
        };
        struct Idle : public msm::front::state<>
        {
        };
        struct Triggered : public msm::front::state<>
        {
            template <class Event, class FSM>
            void on_entry(Event const &, FSM &fsm)
            {
                std::cout << "Triggered" << std::endl;
                std::cout << "led on" << std::endl;
                digitalWrite(LedPinRed, LOW);
                MotorFreeWheeling();
                xTimerChangePeriod(fsm.xTimer, 10, 0);
                xTimerStart(fsm.xTimer, 0);
            }
            template <class Event, class FSM>
            void on_exit(Event const &, FSM &fsm)
            {
                xTimerStop(fsm.xTimer, 0);
            }
        };
        struct AckNewPeering : public msm::front::state<>
        {
            template <class Event, class FSM>
            void on_entry(Event const &, FSM &fsm)
            {
                std::cout << "AckNewPeering" << std::endl;
                digitalWrite(LedPinRed, LOW);
                xTimerChangePeriod(fsm.xTimer, 100, 0);
                xTimerStart(fsm.xTimer, 0);
            }
            template <class Event, class FSM>
            void on_exit(Event const &, FSM &fsm)
            {
                digitalWrite(LedPinRed, HIGH);
                xTimerStop(fsm.xTimer, 0);
            }
        };
        struct DirectionSelected : public msm::front::state<>
        {
            template <class Event, class FSM>
            void on_entry(Event const &, FSM &fsm)
            {
                std::cout << "DirectionSelected" << std::endl;
                MotorRightRotation();
                xTimerChangePeriod(fsm.xTimer, 10, 0);
                xTimerStart(fsm.xTimer, 0);
            }
            template <class Event, class FSM>
            void on_exit(Event const &, FSM &fsm)
            {
                xTimerStop(fsm.xTimer, 0);
            }
        };
        struct RampUp : public msm::front::state<>
        {
            template <class Event, class FSM>
            void on_entry(Event const &, FSM &fsm)
            {
                std::cout << "RampUp" << std::endl;
                RampUpMotor();
                xTimerChangePeriod(fsm.xTimer, 100, 0);
                xTimerStart(fsm.xTimer, 0);
            }
            template <class Event, class FSM>
            void on_exit(Event const &, FSM &fsm)
            {
                xTimerStop(fsm.xTimer, 0);
            }
        };
        struct Drive : public msm::front::state<>
        {
            template <class Event, class FSM>
            void on_entry(Event const &, FSM &fsm)
            {
                std::cout << "Drive" << std::endl;
                xTimerChangePeriod(fsm.xTimer, 1, 0);
                xTimerStart(fsm.xTimer, 0);
            }
            template <class Event, class FSM>
            void on_exit(Event const &, FSM &fsm)
            {
                xTimerStop(fsm.xTimer, 0);
            }
        };
        struct RampDown : public msm::front::state<>
        {
            template <class Event, class FSM>
            void on_entry(Event const &, FSM &fsm)
            {
                std::cout << "RampDown" << std::endl;
                RampDownMotor();
                xTimerChangePeriod(fsm.xTimer, 1, 0);
                xTimerStart(fsm.xTimer, 0);
            }
            template <class Event, class FSM>
            void on_exit(Event const &, FSM &fsm)
            {
                std::cout << "led off" << std::endl;
                digitalWrite(LedPinRed, HIGH);
                xTimerStop(fsm.xTimer, 0);
            }
        };

        // actions


        // guard conditions


        // the initial state of the player SM. Must be defined
        typedef Init initial_state;


        // Transition table for player
        struct transition_table : mpl::vector<
        // clang-format off
            //    Start     Event         Next      Action                     Guard
            //  +---------+-------------+---------+---------------------------+----------------------+
            msm::front::Row<Init,               none,    Idle>,
            msm::front::Row<Idle,               Trigger, Triggered,     none,                   IsPeerExist>,
            msm::front::Row<Idle,               ButtonTrigger, Triggered>,
            msm::front::Row<Idle,               Trigger, AckNewPeering, addPeer,                IsInPeeringModeAndPeerNotExist>,
            msm::front::Row<AckNewPeering,      Timeout, Idle,          msm::front::none,       msm::front::none>,
            msm::front::Row<Triggered,          Timeout, DirectionSelected>,
            msm::front::Row<DirectionSelected,  Timeout, RampUp>,
            msm::front::Row<RampUp,             Timeout, Drive>,
            msm::front::Row<Drive,              Timeout, RampDown>,
            msm::front::Row<RampDown,           Timeout, Idle>
        // clang-format on
                                      >
        {
        };

        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << std::endl;
        }
    };
    // Pick a back-end
    typedef msm::back::state_machine<player_> MainFSM;
}
