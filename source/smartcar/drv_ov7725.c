#include "drv_ov7725.h"
void funcvoid(bool x) {
    __NOP();
}


ov7725_resource_t ov7725Resource = {          //摄像头初始化结构体
    .sccbI2C = NULL,//待填
    .pullResetPin = funcvoid,
    .pullPowerDownPin = funcvoid,
    .inputClockFreq_Hz = 24000000,                    //像素时钟 修改这里可以修改帧率 具体看static const ov7725_clock_config_t ov7725ClockConfigs[]
};
camera_config_t cameraConfig = {
    .pixelFormat = kVIDEO_PixelFormatRGB565,//kVIDEO_PixelFormatRGB565,//kVIDEO_PixelFormatYUYV,
    .bytesPerPixel = 2,
    .resolution = 0,       //分辨率//待填
    .frameBufferLinePitch_Bytes =0,                 //行间隔//待填
    .interface = kCAMERA_InterfaceCCIR656,                 //摄像机接口类型
    .controlFlags = (kCAMERA_VsyncActiveLow | kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge),
    .framePerSec = 75,                              //修改fps 帧率太大需要提高像素时钟到48000000
};

camera_device_handle_t cameraDevice = { 
    .resource = &ov7725Resource,
    .ops = &ov7725_ops,
};

//以上和单片机的具体实现无关
csi_config_t ov7725_csi_config = {//底层接口
    .width = 0,//待填
    .height = 0,//待填
    .polarityFlags = kCSI_HsyncActiveHigh | kCSI_DataLatchOnRisingEdge | kCSI_VsyncActiveLow,
    .bytesPerPixel = 2,
    .linePitch_Bytes = 0,//待填
    .workMode = kCSI_CCIR656ProgressiveMode,
    .dataBus = kCSI_DataBus8Bit,
    .useExtVsync = true,
};

status_t OV7725_Init2(ov7725_frame_size_t size, I2CS_Type* base)
{
    status_t status;
    int height = CAMERA_FRAME_HEIGHT(size);
    int width = CAMERA_FRAME_WIDTH(size);
    ov7725_csi_config.height = height;
    ov7725_csi_config.width = width;
    ov7725_csi_config.bytesPerPixel = width * ov7725_csi_config.bytesPerPixel;
    status = CAMERA_ReceiverInit(&ov7725_csi_config);
    if (status != kStatus_Success) { return status; }
    //初始化完CAMERA_Recevier之后卡死正式初始化摄像头
    ov7725Resource.sccbI2C = base;
    cameraConfig.resolution = FSL_VIDEO_RESOLUTION(width,height);
    cameraConfig.frameBufferLinePitch_Bytes = width*2;
    status = CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);                  //初始化相机配置
    if (status != kStatus_Success) { return status; }
    status = CAMERA_DEVICE_Start(&cameraDevice);                                //启动相机
    return status;
}
