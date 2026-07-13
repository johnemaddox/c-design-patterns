#include "gtest/gtest.h"
extern "C" {
#include "state_machine.h"
#include "traffic_light.h"
}

namespace
{

class StateMachine : public ::testing::Test
{
    public:
        sm_handle_t smh;
        sm_states_t state;

    private:
        void SetUp() override
        {
            /* Initialization is done by defining the states in
             * sm_handle_t. This can also be done using dot as
             * the struct initializer, depending on the standard.
             * 
             * Example using dot initialization.
             * This does not work in C++17, but does work in C11.
             * 
             * sm_handle_t smh = {
             *     .cur_state = SM_GREEN,
             *     .states = {
             *         [SM_YELLOW]{.event = &yellow, .next_state = SM_RED    },
             *         [SM_GREEN] {.event = &green,  .next_state = SM_YELLOW },
             *         [SM_RED]   {.event = &red,    .next_state = SM_GREEN  }
             *     }
             * };
            */
            smh.cur_state = SM_GREEN;
            smh.states[SM_YELLOW] = { &yellow, SM_RED    };
            smh.states[SM_GREEN]  = { &green,  SM_YELLOW };
            smh.states[SM_RED]    = { &red,    SM_GREEN  };
        }
};

/* Cycles through the states, each call to sm_run() executes
 * the current state and queues the next state.
*/
TEST_F(StateMachine, CycleAllStates)
{
    int expect = 0;
    for (int i=0; i<SM_STATES_LEN*2; i++)
    {
        state = sm_run(&smh);
        EXPECT_EQ(expect, state);

        expect = (expect == SM_STATES_LEN-1) ? 0 : expect+1;
    }
}

} // namespace
