#include "gtest/gtest.h"
extern "C" {
#include "intf.h"
}

namespace
{

TEST(IntfSelect, FlagOk)
{
    intf_t cam;
    ASSERT_EQ(intf_select(&cam, INTF_CAM_ONE), INTF_OK);
}

TEST(IntfSelect, FlagErr)
{
    intf_t cam;
    // CAM_HW_THREE is added to cam_hw_t, but it has not been mapped in intf_select
    ASSERT_EQ(intf_select(&cam, INTF_CAM_THREE), INTF_ERR);
}

/* This test is the reason for using an interface. Hardware is being switched here
 * by using the intf_select() function.
*/
TEST(IntfSelect, SwitchHW)
{
    intf_t cam;
    intf_state_t state;

    ASSERT_EQ(intf_select(&cam, INTF_CAM_ONE), INTF_OK);
    state = cam.init();
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_ONE_INIT);

    ASSERT_EQ(intf_select(&cam, INTF_CAM_TWO), INTF_OK);
    state = cam.init();
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_TWO_INIT);
}


class IntfFncs : public ::testing::Test
{
    public:
        intf_t       cam;
        intf_state_t state;

    private:
        void SetUp() override
        {
            ASSERT_EQ(intf_select(&cam, INTF_CAM_ONE), INTF_OK);
        }
};

TEST_F(IntfFncs, Init)
{
    state = cam.init();
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_ONE_INIT);
}

TEST_F(IntfFncs, Capture)
{
    size_t captures = 5;
    state = cam.capture(captures);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_ONE_CAPTURE);
    EXPECT_EQ(state.cam_state.extra, captures);
}

TEST_F(IntfFncs, Start)
{
    state = cam.start();
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_ONE_START);
}

TEST_F(IntfFncs, Stop)
{
    state = cam.stop();
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_ONE_STOP);
}

} // namespace
