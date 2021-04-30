/****************************************************************************************
*																						      *
* Copyright (c) 2012 Analog Devices, Inc.  All Rights Reserved.                               *
* This software is proprietary and confidential to Analog Devices, Inc. and its licensors.    *
*                                                                                             *
***********************************************************************************************/

/*============================================================================
 * This file contains all functions that are chip-specific
 *
 *===========================================================================*/
#include "rx_lib.h"

#if (RX_DEVICE == 7612 || RX_DEVICE == 7611)

#include "rx_cfg.h"
#include "rx_isr.h"


/*==========================================
 * ADV7612 Initialisation Setting
  *=========================================*/
CONSTANT UCHAR RxPllInitTable[] =
{
    /*===============================
     * PLL Recommendations
     *==============================*/

#if ((ES1_7612)|| (ES2_7612)|| (ES1_7611)|| (ES2_7611) )
    RX_I2C_HDMI_MAP_ADDR,      0x57, 0xDA, /*PLL RC Setting for HF modes*/
    RX_I2C_HDMI_MAP_ADDR,      0x58, 0x01, /*PLL RC Setting for HF modes*/

#endif
    0
};

CONSTANT UCHAR RxEqInitTable[] =
{
    /*===============================
     * Equaliser Recommendations
     *==============================*/
#if ((ES1_7612)|| (ES2_7612)|| (ES1_7611)|| (ES2_7611) )
    RX_I2C_HDMI_MAP_ADDR,      0x85, 0x1F,  /*ADI Recommended Eq Setting: AGC MAN*/
    RX_I2C_HDMI_MAP_ADDR,      0x87, 0x70,  /*ADI Recommended Eq Setting: AGC MAN*/
    RX_I2C_HDMI_MAP_ADDR,      0x8D, 0x04,  /*ADI Recommended Eq Setting: LFG PORT A*/
    RX_I2C_HDMI_MAP_ADDR,      0x8E, 0x1E,  /*ADI Recommended Eq Setting: HFG PORT A*/
    RX_I2C_HDMI_MAP_ADDR,      0x90, 0x04,  /*ADI Recommended Eq Setting: LFG PORT B*/
    RX_I2C_HDMI_MAP_ADDR,      0x91, 0x1E,  /*ADI Recommended Eq Setting: HFG PORT B*/
#endif

    0
};

CONSTANT UCHAR RxHdmiAdiRecSetTable[] =
{
    /*===============================
     * HDMI ADI Recommended Settings
     *==============================*/
#if ((ES1_7612)|| (ES2_7612)|| (ES1_7611)|| (ES2_7611) )
    RX_I2C_HDMI_MAP_ADDR,      0x9B, 0x03,      /*ADI Recommended setting for Gear Selection Algorithm*/
    RX_I2C_HDMI_MAP_ADDR,      0x75, 0x10,      /*ADI Recommended setting for DDC line Drive Strength*/
    RX_I2C_HDMI_MAP_ADDR,      0x6F, 0x08,      /*ADI Recommendation for Extended CTL*/
    /*RX_I2C_HDMI_MAP_ADDR,      0x42, 0x3F,*/       /*Evaluation mode only*/
    RX_I2C_IO_MAP_ADDR,        0x19, 0x83,      /* DLL setting */
    RX_I2C_IO_MAP_ADDR,        0x33, 0x40,      /* DLL setting */
    RX_I2C_CP_MAP_ADDR,        0xC8, 0x08,      /* Set digital fine clamp setting for HDMI Mode*/
#endif
    RX_I2C_HDMI_MAP_ADDR,      0x10, 0xA5,      /* Trun ON HDCP mute flag bit 7  */
    RX_I2C_HDMI_MAP_ADDR,      0x45, 0x04,      /* Set Burst Err Filter, prevent PS3 audio noise */
#if (MULTI_RX_NUM)
    RX_I2C_IO_MAP_ADDR,        0x14, 0x6D,
#endif
    0
};


/*===========================================================================
 * Initialise the part into a specific mode.
 *
 * Entry:   OperatingMode = Mode the part mus be set into
 *
 * Return:  ATVERR_OK
 *
 * Notes:   For the ADV7840 a hardware reset should be done
 *          before this function is called
 *
 *==========================================================================*/
ATV_ERR HAL_RxInit (RX_OP_MODE OperatingMode)
{
    UCHAR i;

    /*===============================================
     * Software init
     *==============================================*/
    CurrVideoDetected = FALSE;
    CurrBurstLock     = FALSE;
    CurrSspdLock = FALSE;
    CurrSspdLockCh1 = FALSE;
    CurrSspdLockCh2 = FALSE;
    RxPrevOperatingMode = RX_OPMODE_PWRDWN;
    RxOperatingMode     = RX_OPMODE_PWRDWN;

    for (i=0; i<RX_ENABLE_EVENTS_SIZE; i++)
    {
        RxEnabledEvents[i] = 0;
    }

    /*===============================================
     * Apply Full SW reset
     *==============================================*/
     VRX_set_MAIN_RESET(0x1);
     HAL_DelayMs(5);

    /*===============================================
     * Set I2C address of I2C programmabled maps
     *==============================================*/
    HAL_SetI2cMapAddress(RX_CP_MAP, VRX_CP_MAP_ADDR);
    HAL_SetI2cMapAddress(RX_INFOFRAME_MAP, VRX_INFOFRAME_MAP_ADDR);
    HAL_SetI2cMapAddress(RX_CEC_MAP, VRX_CEC_MAP_ADDR);
    HAL_SetI2cMapAddress(RX_HDMI_MAP, VRX_HDMI_MAP_ADDR);
    HAL_SetI2cMapAddress(RX_REPEATER_MAP, VRX_REPEATER_MAP_ADDR);
    HAL_SetI2cMapAddress(RX_EDID_MAP, VRX_EDID_MAP_ADDR);
    HAL_SetI2cMapAddress(RX_DPLL_MAP, VRX_AFE_DPLL_MAP_ADDR);

    /*===============================================
     * Configure the part into the requested
     * operating mode
     *==============================================*/
    HAL_SetOperatingMode(OperatingMode, NULL);

#if RX_USER_INIT
    /*====================================
     * Set User-specified init values
     *===================================*/
    HAL_RxI2CWriteTable ((UINT8 *)UserRxRegInitTable, 0);
    HAL_RxI2CWriteFields ((UINT8 *)UserRxFieldInitTable, 0);
#endif
    return (ATVERR_OK);
}

/*==========================================================================
 *
 *
 * Entry:
 *
 * Return:  ATVERR_OK
 *          ATVERR_FAILED
 *          ATVERR_INV_PARM
 *
 * Notes:
 *
 *=========================================================================*/
ATV_ERR HAL_SetI2cMapAddress(RX_I2C_MAP  Map, UINT8 Address)
{
    ATV_ERR ErrVal = ATVERR_OK;

    Address >>= 1;
    switch (Map)
    {
        case RX_CP_MAP:
            VRX_set_CP_SLAVE_ADDR(Address);
            break;
        case RX_HDMI_MAP:
            VRX_set_HDMI_SLAVE_ADDR(Address);
            break;
        case RX_REPEATER_MAP:
            VRX_set_KSV_SLAVE_ADDR(Address);
            break;
        case RX_EDID_MAP:
            VRX_set_EDID_SLAVE_ADDR(Address);
            break;
        case RX_INFOFRAME_MAP:
            VRX_set_INFOFRAME_SLAVE_ADDR(Address);
            break;
        case RX_CEC_MAP:
            VRX_set_CEC_SLAVE_ADDR(Address);
            break;
        case RX_DPLL_MAP:
            VRX_set_DPLL_SLAVE_ADDR(Address);
            break;
        default:
            ErrVal = ATVERR_INV_PARM;
            break;
    }
    return (ErrVal);
}

/*==========================================================================
 *
 *
 * Entry:
 *
 * Return:  ATVERR_OK
 *          ATVERR_FAILED
 *          ATVERR_INV_PARM
 *
 * Notes:   Ideally the part should be reset before calling this function
 *
 *=========================================================================*/
ATV_ERR HAL_SetOperatingMode (RX_OP_MODE OperatingMode, RX_OP_MODE_CFG *OpModeCfg)
{
    BOOL DisPowerMgt;
    BOOL DisTriStateMgt;
    RX_OP_MODE TmpOperatingMode;

    if (OpModeCfg == NULL)
    {
       DisPowerMgt = FALSE;
       DisTriStateMgt = FALSE;
    }
    else
    {
        DisPowerMgt = OpModeCfg->DisPowerMgt;
        DisTriStateMgt = OpModeCfg->DisTriStateMgt;
    }

    switch(OperatingMode)
    {
        case RX_OPMODE_PWRDWN:
            if (!DisPowerMgt)
            {
                VRX_set_POWER_DOWN(0x1);
            }
            break;
        case RX_OPMODE_PWRUP:
            if(RxOperatingMode != RX_OPMODE_PWRDWN)
            {
                return (ATVERR_FAILED);
            }
            VRX_set_POWER_DOWN(0);
            TmpOperatingMode = RxPrevOperatingMode;
            RxPrevOperatingMode = RxOperatingMode;
            RxOperatingMode = TmpOperatingMode;
            return (ATVERR_OK);
        case RX_OPMODE_HDMI:
            if(!DisPowerMgt)
            {
                VRX_set_POWER_DOWN(0x0);
            }
            VRX_set_PRIM_MODE(0x6);
            VRX_set_VID_STD(0x2);
            if(!DisTriStateMgt)
            {
#if ( RX_SPLITTER )
                VRX_set_TRI_PIX(1);
                VRX_set_TRI_LLC(1);
                VRX_set_TRI_SYNCS(1);
                VRX_set_TRI_AUDIO(1);
#else
                VRX_set_TRI_PIX(0);
                VRX_set_TRI_LLC(0);
                VRX_set_TRI_SYNCS(0);
                VRX_set_TRI_AUDIO(0);
#endif
            }
            HAL_RxI2CWriteTable((UCHAR *)RxPllInitTable,0);
            HAL_RxI2CWriteTable((UCHAR *)RxEqInitTable,0);
            HAL_RxI2CWriteTable((UCHAR *)RxHdmiAdiRecSetTable,0);
            break;
        default:
            return (ATVERR_INV_PARM);
    }
    if(RxOperatingMode != OperatingMode)
    {
        RxPrevOperatingMode = RxOperatingMode;
        RxOperatingMode = OperatingMode;
    }
    return (ATVERR_OK);
}

/*==========================================================================
 *
 *
 *=========================================================================*/
ATV_ERR HAL_RxHdmiEnAudioClock (BOOL Enable)
{
    VRX_set_audio_core_pdn(!Enable);
    return ATVERR_OK;
}
/*==========================================================================
 *
 *
 *=========================================================================*/
ATV_ERR HAL_RxChipSelect (UCHAR DevIdx)
{
    HAL_SetRxChipSelect(DevIdx);
    return ATVERR_OK;
}

#endif
