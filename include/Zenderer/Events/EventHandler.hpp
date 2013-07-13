/**
 * @file
 *  Zenderer/Events/EventHandler.hpp - Defines an event-handling singleton
 *  object that stores and processes system events from a stack.
 *
 * @author      George Kudrayvtsev (halcyon)
 * @version     2.0
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").         \n
 *  You may not use this file except in compliance with the License.        \n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0                              \n
 *  Unless required by applicable law or agreed to in writing, software     \n
 *  distributed under the License is distributed on an "AS IS" BASIS,       \n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and     \n
 *  limitations under the License.
 *
 * @addtogroup Events
 * @{
 **/

#ifndef ZENDERER__EVENTS__EVENT_HANDLER_HPP
#define ZENDERER__EVENTS__EVENT_HANDLER_HPP

#include <stack>

#include "Zenderer/CoreGraphics/OpenGL.hpp"

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Event.hpp"

/// Forward declaration for callback functions.
struct GLFWwindow;

namespace zen
{
namespace evt
{
    /// @todo   Window events / callback.
    /// @todo   Document API.
    class ZEN_API CEventHandler
    {
    public:
        ~CEventHandler();

        /// Registers all pending events into the interal stack.
        static bool PollEvents();

        /**
         * Removes the latest event from the stack and stores it in the parameter.
         *  This will pop an event from the internal event stack and store
         *  it in the parameter that you passed, tossing out any existing event
         *  data that may have been stored in it.
         *
         * @param   Evt     Event structure to store latest even in
         *
         * @return  `true`  if an event was loaded,
         *          `false` if there are none remaining.
         **/
        bool PopEvent(event_t& Evt);;
        
        /// Retrieves the singleton instance of the event handler.
        static CEventHandler& GetInstance();
        
        static void CharacterCallback(GLFWwindow*, unsigned int c);
        static void KeyboardCallback(GLFWwindow*, int key, int scancode,
                                     int action, int mods);
        static void MouseMotionCallback(GLFWwindow*, double x, double y);
        static void MouseCallback(GLFWwindow*, int button, int action, int mods);
        
    private:
        CEventHandler() {}
        CEventHandler(const CEventHandler&);
        CEventHandler& operator=(const CEventHandler&);
       
        static std::stack<event_t> s_evtList;
        static event_t s_Active;
    };
}
}

#endif // ZENDERER__EVENTS__EVENT_HANDLER_HPP

/** @} **/
