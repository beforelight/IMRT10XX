#include"sc_camera_common.h"
#include"fsl_debug_console.h"
#include"status.h"
#include"task.h"

volatile uint32_t frame_count;
csi_handle_t csi_handel;



status_t CAMERA_MclkSet(uint32_t clk)
{
	if (clk == 24 * 1000 * 1000)
	{
		/* CSI MCLK select 24M. */
		/*
		 * CSI clock source:
		 *
		 * 00 derive clock from osc_clk (24M)
		 * 01 derive clock from PLL2 PFD2
		 * 10 derive clock from pll3_120M
		 * 11 derive clock from PLL3 PFD1
		 */
		CLOCK_SetMux(kCLOCK_CsiMux, 0);
		/*
		 * CSI clock divider:
		 *
		 * 000 divide by 1
		 * 001 divide by 2
		 * 010 divide by 3
		 * 011 divide by 4
		 * 100 divide by 5
		 * 101 divide by 6
		 * 110 divide by 7
		 * 111 divide by 8
		 */
		CLOCK_SetDiv(kCLOCK_CsiDiv, 0);
	}
	else
	{
		CLOCK_SetMux(kCLOCK_CsiMux, 1);
		uint32_t clk_src = CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk);
		int div = 1;
		while (div <= 8)
		{
			if (clk >= (clk_src / div)) { break; }
			div++;
		}
		if (div == 9) {
			CAMERA_MclkSet(24 * 1000 * 1000);
			return kStatus_Fail;
		}
		CLOCK_SetDiv(kCLOCK_CsiDiv, div - 1);
	}
	return kStatus_Success;
}

status_t CAMERA_ReceiverInit(const csi_config_t* csi_config)
{
	CSI_Init(CSI, csi_config);
	return CSI_TransferCreateHandle(CSI, &csi_handel, CAMERA_Callback, NULL);
}

status_t CAMERA_ReceiverDeinit(void)
{
	CSI_Deinit(CSI);
	return kStatus_Success;
}

status_t CAMERA_SubmitBuff(void* buff)
{
	return CSI_TransferSubmitEmptyBuffer(CSI, &csi_handel, buff);
}

status_t CAMERA_FullBufferGet(void* buff)
{
	return CSI_TransferGetFullBuffer(CSI, &csi_handel, buff);
}

status_t CAMERA_ReceiverStart(void)
{
	return CSI_TransferStart(CSI, &csi_handel);
}

status_t CAMERA_ReceiverStop(void)
{
	return CSI_TransferStop(CSI, &csi_handel);
}

float CAMERA_FpsGet(void)
{
	uint32_t msn;
	uint32_t count_now;
	uint32_t msp = TimerMsGet();
	frame_count = 0;
	vTaskDelay(200);
	while (1)
	{
		msn = TimerMsGet();
		count_now = frame_count;
		if (count_now > 60)
		{
			return count_now * 1000 / (msn - msp);
		}
		vTaskDelay(200);
	}
}
/*__WEAK*/ void CAMERA_Callback(CSI_Type* base, csi_handle_t* handle, status_t status, void* userData)
{
	if (status == kStatus_CSI_FrameDone)
	{
		++frame_count;
	}
	else if (status == kStatus_CSI_NoEmptyBuffer)
	{
		PRINTF("No empty frame buffer in queue to load to CSI\r\n");
	}
	else if (status == kStatus_CSI_QueueFull)
	{
		PRINTF("Queue is full, no room to save new empty buffer\r\n");
	}
	else if (status == kStatus_CSI_NoFullBuffer)
	{
		PRINTF("No full frame buffer in queue to read out\r\n");
	}
}

