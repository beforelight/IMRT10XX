//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_SC_SD_H
#define IMXRT_SC_SD_H

#include "fsl_sd_disk.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


status_t SD_CardDetect(void);

status_t SD_Init2(void);

status_t SD_Mount(void);

status_t SD_MscInit(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_SC_SD_H
