#include "gtest/gtest.h"
extern "C" {
#include "camera_intf.h"
#include "camera_hw.h"
#include "camera_sim.h"
}

namespace
{

TEST(IntfCreate, NullPtr)
{
    ASSERT_EQ(camera_hw_create(nullptr), INTF_ERR);
    ASSERT_EQ(camera_sim_create(nullptr), INTF_ERR);
}

TEST(IntfCreate, Hardware)
{
    intf_t cam;
    ASSERT_EQ(camera_hw_create(&cam), INTF_OK);
    EXPECT_NE(cam.ops, nullptr);
    EXPECT_NE(cam.context, nullptr);
}

TEST(IntfCreate, Simulator)
{
    intf_t cam;
    ASSERT_EQ(camera_sim_create(&cam), INTF_OK);
    EXPECT_NE(cam.ops, nullptr);
    EXPECT_NE(cam.context, nullptr);
}

TEST(IntfSafeWrappers, NullPtr)
{
    // Test wrapper null robustness
    EXPECT_EQ(camera_init(nullptr).intf_flag, INTF_ERR);
    EXPECT_EQ(camera_capture(nullptr, 5).intf_flag, INTF_ERR);
    EXPECT_EQ(camera_start(nullptr).intf_flag, INTF_ERR);
    EXPECT_EQ(camera_stop(nullptr).intf_flag, INTF_ERR);

    // Test missing ops
    intf_t empty_cam = { .ops = nullptr, .context = nullptr };
    EXPECT_EQ(camera_init(&empty_cam).intf_flag, INTF_ERR);
}

TEST(IntfSelect, SwitchHW)
{
    intf_t cam;
    camera_state_t state;

    // Test HW Cam
    ASSERT_EQ(camera_hw_create(&cam), INTF_OK);
    state = camera_init(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_INIT);

    // Test SIM Cam
    ASSERT_EQ(camera_sim_create(&cam), INTF_OK);
    state = camera_init(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_INIT);
}

class IntfFncs : public ::testing::Test
{
    public:
        intf_t         cam;
        camera_state_t state;

    private:
        void SetUp() override
        {
            ASSERT_EQ(camera_hw_create(&cam), INTF_OK);
        }
};

TEST_F(IntfFncs, Init)
{
    state = camera_init(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_INIT);
}

TEST_F(IntfFncs, Capture)
{
    const uint32_t captures = 5;
    state = camera_capture(&cam, captures);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_CAPTURE);
    EXPECT_EQ(state.cam_state.extra, captures);
}

TEST_F(IntfFncs, Start)
{
    state = camera_start(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_START);
}

TEST_F(IntfFncs, Stop)
{
    state = camera_stop(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_STOP);
}

} // namespace
