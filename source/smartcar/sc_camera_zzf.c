#include"sc_camera_zzf.h"
#include"FreeRTOS.h"
#include"task.h"
#include"status.h"



//摄像头命令枚举
typedef enum
{
	INIT = 0,               //摄像头初始化命令
	AUTO_EXP,               //自动曝光命令
	EXP_TIME,               //曝光时间命令
	FPS,                    //摄像头帧率命令
	SET_COL,                //图像列命令
	SET_ROW,                //图像行命令
	LR_OFFSET,              //图像左右偏移命令
	UD_OFFSET,              //图像上下偏移命令
	GAIN,                   //图像偏移命令
	CONFIG_FINISH,          //非命令位，主要用来占位计数

	SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
	GET_STATUS,             //获取摄像头配置命令
	GET_VERSION,            //固件版本号命令

	SET_ADDR = 0XFE,        //寄存器地址命令
	SET_DATA                //寄存器数据命令
}CMD;
//需要配置到摄像头的数据
int16_t MT9V032_CFG[CONFIG_FINISH][2] =
{
  {AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
  //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
  {EXP_TIME,          900}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
  {FPS,               50},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
  {SET_COL,           0}, //图像列数量        范围1-752     由初始化函数赋值
  {SET_ROW,           0}, //图像行数量        范围1-480      由初始化函数赋值
  {LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
  {UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
  {GAIN,              64},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度


  {INIT,              0}    //摄像头开始初始化
};
//从摄像头内部获取到的配置数据
int16_t GET_CFG[CONFIG_FINISH - 1][2] =
{
  {AUTO_EXP,          0},   //自动曝光设置      
  {EXP_TIME,          0},   //曝光时间          
  {FPS,               0},   //图像帧率          
  {SET_COL,           0},   //图像列数量        
  {SET_ROW,           0},   //图像行数量        
  {LR_OFFSET,         0},   //图像左右偏移量    
  {UD_OFFSET,         0},   //图像上下偏移量    
  {GAIN,              0},   //图像增益          
};

LPUART_Type* uartx;

csi_config_t zzf_csi_config = {
	.width = 0,
	.height = 0,
	.polarityFlags = kCSI_HsyncActiveHigh | kCSI_DataLatchOnRisingEdge | kCSI_VsyncActiveHigh,
	.bytesPerPixel = 1,
	.linePitch_Bytes = 0,
	.workMode = kCSI_GatedClockMode,
	.dataBus = kCSI_DataBus8Bit,
	.useExtVsync = true,
};

status_t ZZF_Init(zzf_frame_size_t frameSize, LPUART_Type* LPUARTx)
{
	uartx = LPUARTx;
	MT9V032_CFG[SET_COL - 1][1] = ZZF_FRAME_WIDTH(frameSize);
	MT9V032_CFG[SET_ROW - 1][1] = ZZF_FRAME_HEIGHT(frameSize);
	vTaskDelay(50);//等待上电初始化完成
	int16_t temp = 0;

	uint8_t RXtemp = 0;
	uint8_t  send_buffer[4];
	for (int i = 0; i < CONFIG_FINISH; i++)//开始配置摄像头并重新初始化
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = MT9V032_CFG[i][0];
		temp = MT9V032_CFG[i][1];
		send_buffer[2] = temp >> 8;
		send_buffer[3] = (uint8_t)temp;
		UART_WriteBlocking(uartx, send_buffer, 4);
		vTaskDelay(1);
	}
	uint32_t msp = TimerMsGet();
	while (!(RXtemp == 0xA5 || RXtemp == 0xff))
	{
		while (!UART_RxCountGet(uartx))//如果fifo为空
		{
			if (TimerMsGet() - msp >= 200)//如果200ms都没有收到回应，则说明初始化失败
			{
				PRINTF("time out! Did not receive a reply from camera\r\n!");
				return kStatus_Timeout;
			}
		}
		UART_ReadBlocking(uartx, &RXtemp, 1);
	}

	//利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
	//获取配置便于查看配置是否正确
	status_t status;
	status = ZZF_ConfigGet();
	if (status != kStatus_Success) { return status; }
	//检查配置是否一致
	PRINTF("camera AUTO_EXP	= %d\n", (int)GET_CFG[AUTO_EXP - 1][1]);
	PRINTF("camera EXP_TIME	= %d\n", (int)GET_CFG[EXP_TIME - 1][1]);
	PRINTF("camera FPS		= %d\n", (int)GET_CFG[FPS - 1][1]);
	PRINTF("camera SET_COL	= %d\n", (int)GET_CFG[SET_COL - 1][1]);
	PRINTF("camera SET_ROW	= %d\n", (int)GET_CFG[SET_ROW - 1][1]);
	PRINTF("camera LR_OFFSET	= %d\n", (int)GET_CFG[LR_OFFSET - 1][1]);
	PRINTF("camera UD_OFFSET	= %d\n", (int)GET_CFG[UD_OFFSET - 1][1]);
	PRINTF("camera GAIN		= %d\n", (int)GET_CFG[GAIN - 1][1]);
	if ((MT9V032_CFG[SET_COL - 1][1] != GET_CFG[SET_COL - 1][1])
		|| (MT9V032_CFG[SET_ROW - 1][1] != GET_CFG[SET_ROW - 1][1]))
	{
		PRINTF("camera cfg error\r\n!");
		return kStatus_Fail;
	}
	//摄像头上面的配置初始化完了，开始初始化摄像头接收的东西
	zzf_csi_config.width = ZZF_FRAME_WIDTH(frameSize);
	zzf_csi_config.height = ZZF_FRAME_HEIGHT(frameSize);
	zzf_csi_config.linePitch_Bytes = zzf_csi_config.width * zzf_csi_config.bytesPerPixel;
	return CAMERA_ReceiverInit(&zzf_csi_config);
}

status_t ZZF_ConfigGet(void)
{
	int16_t temp, i;
	uint8_t  send_buffer[4];
	uint8_t tempppp = 0;
	uint8_t txlen;
	uint8_t rxlen;
	uint8_t   receive[3];
	while (UART_RxCountGet(uartx))//清空FIFO
	{
		UART_ReadBlocking(uartx, &tempppp, 1);
	}
	uint32_t msp = TimerMsGet();
	for (i = 0; i < CONFIG_FINISH - 1; i++)
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = GET_STATUS;
		temp = GET_CFG[i][0];
		send_buffer[2] = temp >> 8;
		send_buffer[3] = (uint8_t)temp;
		txlen = 4;
		rxlen = 3;
		while (txlen || rxlen)
		{
			if ((UART_RxCountGet(uartx)) && rxlen != 0)
			{
				UART_ReadBlocking(uartx, &receive[3 - rxlen], 1);
				if (receive[0] == 0xa5) {
					rxlen--;
				}
			}
			if (UART_TcGet(uartx) && txlen != 0)
			{
				UART_WriteBlocking(uartx, &send_buffer[4 - txlen], 1);
				txlen--;
			}
			if (TimerMsGet() - msp >= 500)//如果500ms都没有拔完
			{
				PRINTF("time out! Did not receive a reply from camera\r\n!");
				return kStatus_Timeout;
			}
		}
		GET_CFG[i][1] = receive[1] << 8 | receive[2];
	}

	return kStatus_Success;
}
