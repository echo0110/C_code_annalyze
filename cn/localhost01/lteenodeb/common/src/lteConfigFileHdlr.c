
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteConfigFileHdlr.c 
 *
 *************************************************************************
 *
 * File Description : This file contains the functions and structure used 
 *                    configuration 
 *************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include "lteConfigFileHdlr.h"
#include "lteLog.h"
#include "lteMisc.h"
#include "lteLayer2CommanTypes.h"

/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define SUCCESS 1
#define FAILURE 0

/******************************************************************************
 Private Types
 *****************************************************************************/
typedef struct PhyPortStrT
{
	char l1ControlRxPortStr[MAX_STR_LENGTH];
    char l1ControlTxPortStr[MAX_STR_LENGTH];
	char l1DataTxPortStr[MAX_STR_LENGTH];
	char l1DataRxPortStr[MAX_STR_LENGTH];
	char l1IpAddressStr[MAX_STR_LENGTH];
}PhyPortStr; 

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
PhyPortInfo phyPortInfo_g[MAX_NUM_CELL] = {{0,0,0,0,{" "}}};
/*****************************************************************************
 * Global variables 
 ****************************************************************************/

/* CA Change start */
/****************************************************************************
 * Function Name  : l2CommonCreateTokenStringForPhyParams
 * Inputs         : pointer of Strings for tokens
 *                : cellIndex: Internal cell index
 *                : instancel2: Layer instance number
 * Outputs        : Creation of runtime string for tokens
 * Returns        : None
 * Variables      :
 * Description    : This function creates the runtime string for parsing
 *     	          : configuration file.
 ****************************************************************************/
void l2CommonCreateTokenStringForPhyParams( PhyPortStr *phyPortStr,
                       InternalCellIndex cellIndex,SInt32 instancel2)
{
    snprintf(phyPortStr[cellIndex].l1ControlRxPortStr,MAX_STR_LENGTH, "L1_CONTROL_RX_PORT_%d%d",
                                                         instancel2,cellIndex);
    snprintf(phyPortStr[cellIndex].l1ControlTxPortStr,MAX_STR_LENGTH, "L1_CONTROL_TX_PORT_%d%d",
                                                         instancel2,cellIndex);
    snprintf(phyPortStr[cellIndex].l1DataRxPortStr,MAX_STR_LENGTH, "L1_DATA_RX_PORT_%d%d",
                                                         instancel2,cellIndex);
    snprintf(phyPortStr[cellIndex].l1DataTxPortStr,MAX_STR_LENGTH, "L1_DATA_TX_PORT_%d%d",
                                                         instancel2,cellIndex);
    snprintf(phyPortStr[cellIndex].l1IpAddressStr,MAX_STR_LENGTH, "L1_IP_ADDRESS_%d%d",
                                                         instancel2,cellIndex);
    return;
}

/****************************************************************************
 * Function Name  : lteCreateTokenString
 * Inputs         : pointer of Strings for tokens
 *                      
 * Outputs        : Creation of runtime string for tokens
 * Returns        : None
 * Variables      :
 * Description    : This function creates the runtime string for parsing
 *        	      : configuration file.
 ****************************************************************************/
void lteCreateTokenString(char *enbMacRxPortStr,char *enbRlcRxPortStr,
                          char *enbPdcpRxPortStr,
                          /*SPR 20863 FIX*/
                          char *enbMacIpAddressStr,char *enbPdcpIpAddressStr,
                          /*SPR 20863 FIX*/
#ifdef DL_UL_SPLIT_TDD
                          char *l1DataRxULThreadPortStr,
#endif
                          SInt32 instanceL2
)
{
    if ( instanceL2 != 0 )
    {
        snprintf(enbRlcRxPortStr,MAX_STR_LENGTH, "ENB_RLC_RX_PORT_%d",instanceL2);
        snprintf(enbMacRxPortStr,MAX_STR_LENGTH, "ENB_MAC_RX_PORT_%d",instanceL2);
        snprintf(enbPdcpRxPortStr,MAX_STR_LENGTH, "ENB_PDCP_RX_PORT_%d",instanceL2);
#ifdef DL_UL_SPLIT_TDD
	snprintf(l1DataRxULThreadPortStr,MAX_STR_LENGTH, "L1_DATA_RX_UL_THREAD_PORT_%d",instanceL2);
#endif
/*SPR 20863 FIX*/
        snprintf(enbMacIpAddressStr,MAX_STR_LENGTH, "ENB_MAC_IP_ADDRESS_%d",instanceL2);
        snprintf(enbPdcpIpAddressStr,MAX_STR_LENGTH, "ENB_PDCP_IP_ADDRESS_%d",instanceL2);
/*SPR 20863 FIX*/

    }
    else
    {
        strNCpy(enbRlcRxPortStr,"ENB_RLC_RX_PORT",MAX_STR_LENGTH);
        strNCpy(enbMacRxPortStr,"ENB_MAC_RX_PORT",MAX_STR_LENGTH);
        strNCpy(enbPdcpRxPortStr,"ENB_PDCP_RX_PORT",MAX_STR_LENGTH);
#ifdef DL_UL_SPLIT_TDD
	strNCpy(l1DataRxULThreadPortStr,"L1_DATA_RX_UL_THREAD_PORT",MAX_STR_LENGTH);
#endif
/*SPR 20863 FIX*/
        strNCpy(enbMacIpAddressStr,"ENB_MAC_IP_ADDRESS",MAX_STR_LENGTH);
        strNCpy(enbPdcpIpAddressStr,"ENB_PDCP_IP_ADDRESS",MAX_STR_LENGTH);
/*SPR 20863 FIX*/
    }
    return;
}
/* CA Change end */

void trimSpace(char *buf,unsigned int buf_size)
{
    unsigned int idx = 0, loc = 0;

    while((buf[idx] != '\n') && (idx < buf_size ))
    {
        if(buf[idx] != ' ')
        {
            buf[loc] = buf[idx];
            loc++;
        }
        idx++;
    }
     buf[loc] = '\0';
}

Char8* isKeyPresent( Char8* string_p, Char8 *keyName_p)
{
    UInt32 keyLen = strlen_wrapper(keyName_p);
    if ( 0 == strNCmp((char*)keyName_p, (char*)string_p, keyLen))
    {
        if ( '=' == string_p[keyLen] )
        {
            /* Returning position for Key Value */
            return string_p + keyLen + 1;
        }
    }
    return PNULL;
}

/* CA Change start */
/****************************************************************************
 * Function Name  : fillIpAndPortInfo
 * Inputs         : pointer of :
 *                      RRC receiving port, PDCP tx port,  RLC tx port
 *                       MAC tx port
 * Outputs        : success (1) or failure (0)
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to value read from file

 ****************************************************************************/
UInt8 fillIpAndPortInfo(UInt16 *rxPortMAC, UInt16 *rxPortRLC, 
        UInt16 *txPortOAM, UInt16 *txPortRRC, UInt16 *rxPortPDCP,
        Char8 *oamIp_p, Char8 *rrcIp_p,
        /*SPR 20863 FIX*/
        Char8 *macIp, Char8 *pdcpIP,Char8 *rrmIp_p, UInt16 * rrmTxPort_p,
        /*SPR 20863 FIX*/
        /*SPR 2118 +*/
#if defined PDCP_GTPU_INTF
        Char8 *gtpuIp_p, UInt16 *gtpuRxPort_p,UInt16 *pdcpGtpuRxPort_p,
#ifdef ENDC_ENABLED
        /*Dual Connectivity SeNB changes +*/
        UInt16 *rxX2uDataPortRlc_p,
        /*Dual Connectivity SeNB changes -*/
#endif
#endif
        /*SPR 2118 -*/
#ifdef DL_UL_SPLIT_TDD
        UInt16 *rxPortULThreadPhy, 
#endif 
        Char8 *portConfigFileName, SInt32 instanceL2)
{
/* SPR 23260 + */
    SInt32 temp_port = 0;
/* SPR 23260 - */
    FILE *fp;
    UInt32 port=0;
    char buf[101] = {0,};
    Char8* str_p = PNULL;
    
#ifdef DL_UL_SPLIT_TDD
    char l1DataRxULThreadPortStr[MAX_STR_LENGTH];
#endif
    char enbMacRxPortStr[MAX_STR_LENGTH],enbRlcRxPortStr[MAX_STR_LENGTH],enbPdcpRxPortStr[MAX_STR_LENGTH],
#ifdef DL_UL_SPLIT_TDD
         l1DataRxULThreadPortStr,
#endif
         /*SPR 20863 FIX*/
        enbMacIpAddressStr[MAX_STR_LENGTH] = {0},
        enbPdcpIpAddressStr[MAX_STR_LENGTH]= {0};

    /* SPR 5178 changes starts here */
    UInt16 flag = 0;

    /*SPR 20863 FIX*/
    lteCreateTokenString(enbMacRxPortStr,enbRlcRxPortStr,enbPdcpRxPortStr,
		    enbMacIpAddressStr,enbPdcpIpAddressStr
		    ,instanceL2);
    /*SPR 20863 FIX*/
    /* SPR 5178 changes ends here */

    fp = fopen(portConfigFileName,"r");
    if(fp == PNULL) {
        LTE_LOG(LOG_INFO,"Fail to open the port config file eNodeb_Configuration.cfg");
        return FAILURE;
    }

    /*Coverity Fix 94097 +-*/

    while( fgets(buf,100,fp)){

        trimSpace(buf,sizeof(buf));
        if(PNULL != (str_p = isKeyPresent(buf,"ENB_OAM_RX_PORT")))
        {
            port = atoi(str_p);
            if(port > 0 && port < 65535 )
            {
                *(txPortOAM) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }
#if defined PDCP_GTPU_INTF
        else if(PNULL != (str_p = isKeyPresent(buf,"ENB_GTPU_IP_ADDRESS")))
        {
            memCpy(gtpuIp_p,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1;
        }
/* SPR 23260 + */
        else if(sscanf(buf,"ENB_GTPU_RX_PORT=%d",&temp_port) > 0)
        {
            *gtpuRxPort_p = temp_port;
/* SPR 23260 - */
            flag = (flag << 1) | 1;
        }
/* SPR 23260 + */
        else if(sscanf(buf,"ENB_PDCP_GTPU_RX_PORT=%d",&temp_port) > 0)
        {
            *pdcpGtpuRxPort_p = temp_port;
/* SPR 23260 - */
            flag = (flag << 1) | 1;
        }
#ifdef ENDC_ENABLED
        /*Dual Connectivity SeNB changes +*/
        else if(sscanf(buf,"ENB_X2U_RLC_RX_PORT=%d",(UInt32 *)rxX2uDataPortRlc_p) > 0)
        {
            flag = (flag << 1) | 1;
        }
        /*Dual Connectivity SeNB changes -*/
#endif
#endif
        else if(PNULL != (str_p = isKeyPresent(buf,"ENB_L3_PORT")))
        {
            port = atoi(str_p);
            if(port > 0 && port < 65535 )
            {
                *(txPortRRC) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }
        else if(PNULL != (str_p = isKeyPresent(buf,enbRlcRxPortStr)))
        {
            port = atoi(str_p);
            if(port > 0 && port < 65535 )
            {
                *(rxPortRLC) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }
        else if(PNULL != (str_p = isKeyPresent(buf,enbMacRxPortStr)))
        {
            port = atoi(str_p);
            if(port > 0 && port < 65535 )
            {
                *(rxPortMAC) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }
        else if(PNULL != (str_p = isKeyPresent(buf,enbPdcpRxPortStr)))
        {
            port = atoi(str_p);
            if(port > 0 && port < 65535 )
            {
                *(rxPortPDCP) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }
        /* SPR 5178 changes starts here */
        else if(PNULL != (str_p = isKeyPresent(buf,"ENB_OAM_IP_ADDRESS")))
        {
            memCpy(oamIp_p, str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1;
        }
        else if(PNULL != (str_p = isKeyPresent(buf,"ENB_L3_IP_ADDRESS")))
        {
            memCpy(rrcIp_p,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1;
        }
        /*SPR 20863 FIX*/
        else if(PNULL != (str_p = isKeyPresent(buf,enbMacIpAddressStr)))
        {
            memCpy(macIp,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1;
        }
        else if(PNULL != (str_p = isKeyPresent(buf,enbPdcpIpAddressStr)))
        {
            memCpy(pdcpIP,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1;
        }
        /*SPR 20863 FIX*/
        else if(sscanf(buf,"ENB_RRM_IP_ADDRESS=%s", rrmIp_p) > 0)
        {
            flag = (flag << 1) | 1;
        }
/* SPR 23260 + */
        else if(sscanf(buf,"ENB_RRM_RX_PORT=%d",&temp_port) > 0)
        {
            *rrmTxPort_p = temp_port;
/* SPR 23260 - */
            flag = (flag << 1) | 1;
        }
        /*SPR 20863 FIX*/

#ifdef DL_UL_SPLIT_TDD
        else if(PNULL != (str_p = isKeyPresent(buf,l1DataRxULThreadPortStr)))
        {
            port = atoi(str_p);
            if(port > 0 && port < 65535 )
            {
                *(rxPortULThreadPhy) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }

#endif
    }
    fClose(fp);
#ifdef DL_UL_SPLIT_TDD
        if(flag ==  0x3FFF)
#else
                        /*SPR 21188 +*/  
#if defined PDCP_GTPU_INTF
                        if(flag ==  0x3FFF)
#else
                            if(flag ==  0x7FF)
#endif
                                /*SPR 21188 -*/   
#endif
                            {
                                return SUCCESS;
                            }
                            else
                            {
                                exit (0);
                            }
}

/****************************************************************************
 * Function Name  : l2CommonFillPhyIpAndPortInfo
 * Inputs         : portConfigFileName: Config File name 
 *                : cellIndex: Internal cell index
 *                : instancel2: Layer instance number
 * Outputs        : pointer of :
 *                :     PHY IP address
 *                :     Phy Tx Data port
 *                :     Phy Rx Data port
 *                :     Phy Tx Control port
 *                :     Phy Rx Control port
 * Returns        : success (1) or failure (0)
 * Variables      :
 * Description    : set all the passed port to value read from file

 ****************************************************************************/
#ifndef UE_SIM_TESTING
UInt8  l2CommonFillPhyIpAndPortInfo(Char8 *portConfigFileName,SInt32 instanceL2
                                  ,UInt8 *phyFlag_p)
{

	FILE *fp;
	UInt32 port=0;
	char buf[101] = {0,};
	Char8* str_p = PNULL;
    UInt8 ret;
	InternalCellIndex cellIndex = 0;

    PhyPortStr phyPortStr[MAX_NUM_CELL]; 
     
	UInt8 *flag = phyFlag_p;
	for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)  
	{ 
		l2CommonCreateTokenStringForPhyParams(&phyPortStr[0],
                      				cellIndex,instanceL2);
	}

	fp = fopen(portConfigFileName,"r");
	if(fp == PNULL) {
		LTE_LOG(LOG_INFO,"Fail to open the port config file eNodeb_Configuration.cfg");
		return FAILURE;
	}

	while( fgets(buf,100,fp)){

		trimSpace(buf,sizeof(buf));
         cellIndex = 0;
		for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
		{
			if(PNULL != (str_p = isKeyPresent(buf,phyPortStr[cellIndex].l1ControlRxPortStr)))
			{
				port = atoi(str_p);
				if(port > 0 && port < 65535 )
				{
					phyPortInfo_g[cellIndex].phyControlTxPort  = (UInt16)port;
					LTE_LOG(LOG_INFO,"Control Info Send Port = %u\n",phyPortInfo_g[cellIndex].phyControlTxPort);
					flag[cellIndex] = (flag[cellIndex] << 1) | 1;
				}
			}
			else if(PNULL != (str_p = isKeyPresent(buf,phyPortStr[cellIndex].l1ControlTxPortStr)))
			{
				port = atoi(str_p);
				if(port > 0 && port < 65535 )
				{
					phyPortInfo_g[cellIndex].phyControlRxPort  = (UInt16)port;
					LTE_LOG(LOG_INFO,PNULL,"Control Info Receive Port = %u\n",
							phyPortInfo_g[cellIndex].phyControlRxPort );
					flag[cellIndex] = (flag[cellIndex] << 1) | 1;
				}
			}
			else if(PNULL != (str_p = isKeyPresent(buf,phyPortStr[cellIndex].l1DataRxPortStr)))
			{
				port = atoi(str_p);
				if(port > 0 && port < 65535 )
				{
					phyPortInfo_g[cellIndex].txPortPHY = (UInt16)port;
					flag[cellIndex] = (flag[cellIndex] << 1) | 1;
				}
			}
			else if(PNULL != (str_p = isKeyPresent(buf,phyPortStr[cellIndex].l1DataTxPortStr)))
			{
				port = atoi(str_p);
				if(port > 0 && port < 65535 )
				{
					phyPortInfo_g[cellIndex].rxPortPHY = (UInt16)port;
					flag[cellIndex] = (flag[cellIndex] << 1) | 1;
				}
			}
			else if(PNULL != (str_p = isKeyPresent(buf,phyPortStr[cellIndex].l1IpAddressStr)))
			{
				memCpy(phyPortInfo_g[cellIndex].phyIP, str_p, strlen_wrapper((const char*)str_p));
				flag[cellIndex] = (flag[cellIndex] << 1) | 1;
			}
		}
       
	}
	fClose(fp);
    for (cellIndex = 0;cellIndex < MAX_NUM_CELL;cellIndex++)
	{
		if(flag[cellIndex] !=  0x1F)
		{
exit (0);
/* SPR 21001 Changes end */
		}
        else
        {
            ret = SUCCESS;
        }
	}
   return ret;
    
}
/* CA Change end */
#endif 
/******************************************************************************
 * Function Name  : createTokenStringRelay
 * Inputs         : character array for strings 
 * Outputs        : None
 * Returns        : success
 * Description    : This funtion generates the run time string
 * *****************************************************************************/
/* + Coverity_10723 */
/* +- SPR 17777 */
void createTokenStringRelay(char *enbL2IpAddressStr,
                            char *enbS1uRxPortStr,SInt32 instanceL2
/* +- SPR 17777 */
)
{
    if ( instanceL2 != 0 )
    {
        /*SPR 20863 Start*/
        snprintf(enbL2IpAddressStr,MAX_STR_LENGTH,"ENB_MAC_IP_ADDRESS_%d",instanceL2);
        snprintf(enbS1uRxPortStr,MAX_STR_LENGTH,"ENB_S1U_RX_PORT_%d",instanceL2);
    }
    else
    {
        /*SPR 21188 +*/
#if defined PDCP_GTPU_INTF
        strNCpy(enbL2IpAddressStr,"ENB_GTPU_IP_ADDRESS",MAX_STR_LENGTH);
#else
        strNCpy(enbL2IpAddressStr,"ENB_MAC_IP_ADDRESS",MAX_STR_LENGTH);
#endif
        /*SPR 21188 +*/
        strNCpy(enbS1uRxPortStr,"ENB_S1U_RX_PORT",MAX_STR_LENGTH);
        /*SPR 20863 End*/
    }
    return;
}

/* - Coverity_10723 */
/******************************************************************************
 * Function Name  : getCfgForSocket 
 * Inputs         : None
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion read config information for packet relay.
 *****************************************************************************/
UInt32 getCfgForSocket(UInt8  *cfgFile_p, SInt8 * oamSocketInf,  
        SInt8 * s1ApSocketInf, Char8 * adapterSocketInf,
        UInt32*  oamRxPort,UInt32*  s1ApRxPort, UInt32* s1ApTxPort, 
        Char8 * peerTgInf, Char8 * peerTgInfV6, Char8 * peerTgAddrType,
        Char8 * secPeerTgInf, Char8 * secPeerTgInfV6, Char8 * secPeerTgAddrType
        , SInt32 instanceL2
    /*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
        ,UInt32* gtpuRxPort
#ifdef ENDC_ENABLED
        ,UInt32* rlcRxPort
#endif
#endif
    /*SPR 21366 +*/
        )
{
    Char8 readBuf[100] = {0};
    Char8* str_p = PNULL;
    UInt16 flag = 0;
    FILE *fp = PNULL;
    if (PNULL == cfgFile_p)
        cfgFile_p = (UInt8*) "../cfg/eNodeB_Configuration.cfg";
    fp = fopen((Char8*)cfgFile_p,"r");
    if (PNULL == fp)
    {
        LTE_LOG(LOG_INFO,"[ERROR] Not able to open configuration file [../cfg/lteLayer2PortConfig.cfg]\n");
        return FAILURE;
    }

    char enbL2IpAddressStr[MAX_STR_LENGTH], enbS1uRxPortStr[MAX_STR_LENGTH];

    /* +- SPR 17777 */
    createTokenStringRelay(enbL2IpAddressStr,enbS1uRxPortStr,
            instanceL2);
    /* +- SPR 17777 */

    while (fgets(readBuf,100,fp))
    {
        trimSpace(readBuf,sizeof(readBuf));
        if(PNULL != (str_p = isKeyPresent(readBuf,"ENB_OAM_IP_ADDRESS")))
        {
            memCpy(oamSocketInf,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1; 
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,"ENB_L3_IP_ADDRESS")))
        {
            memCpy(s1ApSocketInf,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1; 
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,enbL2IpAddressStr)))
        {
            memCpy(adapterSocketInf,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1; 
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,"ENB_OAM_RX_PORT")))
        {
            *oamRxPort = atoi(str_p);
            flag = (flag << 1) | 1; 
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,"ENB_L3_PORT")))
        {
            *s1ApRxPort = atoi(str_p);
            flag = (flag << 1) | 1; 
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,enbS1uRxPortStr)))
        {
            *s1ApTxPort = atoi(str_p);
            flag = (flag << 1) | 1;
        } 
        else if(PNULL != (str_p = isKeyPresent(readBuf,"DL_TG_IP_ADDRESS")))
        {
            memCpy(peerTgInf,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1;
            *peerTgAddrType |= IPV4_ADDR_PRESENT;
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,"DL_TG_IP_ADDRESS_V6")))
        {
            memCpy(peerTgInfV6,str_p, strlen_wrapper((const char*)str_p));
            flag = (flag << 1) | 1;
            *peerTgAddrType |= IPV6_ADDR_PRESENT;
        }
        /*IPV6_CHANGE - END*/
        else if(sscanf(readBuf,"SEC_DL_TG_IP_ADDRESS=%s",secPeerTgInf) > 0)
        {
            flag = (flag << 1) | 1;
            *secPeerTgAddrType |= IPV4_ADDR_PRESENT;
        }
        /*IPV6_CHANGE - START*/
        else if(sscanf(readBuf,"SEC_DL_TG_IP_ADDRESS_V6=%s",secPeerTgInfV6) > 0)
        {
            flag = (flag << 1) | 1;
            *secPeerTgAddrType |= IPV6_ADDR_PRESENT;
        }
        /*IPV6_CHANGE - END*/
        /*SPR 21366 +-*/
        /*SPR 21188 +*/
#if defined PDCP_GTPU_INTF
        else if(PNULL != (str_p = isKeyPresent(readBuf,"ENB_GTPU_RX_PORT")))
        {
            *gtpuRxPort = atoi(str_p);
            flag = (flag << 1) | 1; 
        }
#ifdef ENDC_ENABLED
        /*Dual Connectivity SeNB changes +*/
        else if(PNULL != (str_p = isKeyPresent(readBuf,"ENB_X2U_RLC_RX_PORT")))
        {
            *rlcRxPort = atoi(str_p);
            flag = (flag << 1) | 1; 
        }
        /*Dual Connectivity SeNB changes -*/
#endif
#endif
        /*SPR 21188 -*/
    }
    /* SPR 4047 Fix Start */
    fClose(fp);
    /* SPR 4047 Fix End */

    return SUCCESS;
}

/****************************************************************************
 * Function Name  : ReadConfigFile
 * Inputs         : configFileName - Config File name to be read
 * Outputs        : None
 * Returns        : status (1-Failure,0-Successful)
 * Variables      :
 * Description    : To init config data struct from config file
 ****************************************************************************/
UInt8 ReadConfigFile(Char8* configFileName, Char8 * trafficGeneratorIP_p, 
    UInt16* pdcpAdapterMode_g, UInt16* pdcpAdapterDelay_g)
{
    FILE *fp = PNULL;
    char readBuf[101] = {0};
    Char8* str_p = PNULL;

    fp = fopen(configFileName,"r");
    if(fp == PNULL) 
    {
        *pdcpAdapterMode_g = NORMAL_MODE;
        *pdcpAdapterDelay_g = 0;
        return FAILURE;
    }

    /*read the port numbers */
    while( fgets(readBuf,100,fp))
    {
        trimSpace(readBuf,sizeof(readBuf));
        if(PNULL != (str_p = isKeyPresent(readBuf,"ADAPTER_MODE")))
        {
            *pdcpAdapterMode_g = (UInt8)atoi(str_p);
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,"PUMP_DELAY")))
        {
            *pdcpAdapterDelay_g = (UInt8)atoi(str_p);
        }
        else if(PNULL != (str_p = isKeyPresent(readBuf,"TRAFFIC_GENERATOR_HOST_NAME")))
        {
            memCpy(trafficGeneratorIP_p,str_p, strlen_wrapper((const char*)str_p));
            LTE_LOG(LOG_INFO,"Traffic Generator Config at [%s]",trafficGeneratorIP_p);
        }
    }
    /*As per SPR 713, change start*/
    fClose(fp);
    /*As per SPR 713, change start*/
    return SUCCESS;
}

/****************************************************************************
 * Function Name  : buildConfiguration
 * Inputs         : UChar8* buildStamp_p 
 * Outputs        : Nothing
 * Returns        : RLC_SUCCESS or ErrorCode
 * Variables      : 
 * Description    : This function reads the build stamp file and fills the 
 *                  build version in the array pointed by buildStamp_p 
 ****************************************************************************/
inline UInt32 buildConfiguration( UInt8* responseBuffer_p,UInt32 * Idx_p )
{
    FILE *buildFile_p;
    Char8 *buildStamp_p = PNULL;

    UInt32 size      = 0;
    UInt32 Idx = *Idx_p;
    buildFile_p = fopen("rlcbuildStamp","r");

    if ( PNULL == buildFile_p)
    {
        return 1 ;//RLC_OAM_FILE_NOT_EXIST;
    }
    /* + Coverity 32406 */
    else if ( getline(&buildStamp_p, (size_t *)&size, buildFile_p) != -1 )
    /* - Coverity 32406 */
    {
        while ( buildStamp_p[Idx] != '\n') 
        {
            responseBuffer_p[Idx] = buildStamp_p[Idx];
            Idx++;
        }
        responseBuffer_p[Idx] = '\0';
    }
    else 
    {
        fClose(buildFile_p);
        return 2;
    }
    *Idx_p = Idx;
    fClose(buildFile_p);
    return 0;
}

/****************************************************************************
 * Function Name  : getPDCPBuildInfo 
 * Inputs         : buildInfo_p - Pointer to buffer for storing Build information
 *                  maxSize - Maximum size of build info
 * Outputs        : None.
 * Returns        : Size of BuldInfo or 0 on case of error code.
 * Description    : It returns information about current build version 
 from file which name is defined in PDCP_BUILD_INFO_FILE.
 ****************************************************************************/
UInt16 getPDCPBuildInfo(Char8* buildInfo_p, UInt16 maxSize,Char8* fileName_p)
{
    UInt16  size = 0;
    FILE*   buildInfoFile_p = PNULL;

    /* Open file */
    buildInfoFile_p = fopen( fileName_p, "rb" );
    if ( PNULL != buildInfoFile_p )
    {
        /* Read build info */
        if ( PNULL != fgets( buildInfo_p, maxSize,  buildInfoFile_p ) )
        {
            size = (UInt16)strlen_wrapper( buildInfo_p );
        }
        /* Close file */
        fClose( buildInfoFile_p );
    } 

    if ( 0 == size )
    {
        LTE_LOG( LOG_WARNING, "BUILD_INFO: File io error." );
    }

    return size;
}

/****************************************************************************
 * Function Name  : getBuildInfo 
 * Inputs         : buildInfo_p - Pointer to buffer for storing Build information
 *                  maxSize - Maximum size of build info
 * Outputs        : None.
 * Returns        : Size of BuldInfo or 0 on case of error code.
 * Description    : It returns information about current build version 
 from file which name is defined in PDCP_BUILD_INFO_FILE.
 ****************************************************************************/
/* + Coverity 31402 */
void handleAdapterSockInf(Char8 * adapterSocketInf_p,UInt8 size, UInt8 *gtpuInitDbReq, UInt16 current)
{
    snprintf(adapterSocketInf_p,(size - 1),"%d.%d.%d.%d",
             gtpuInitDbReq[current-4], gtpuInitDbReq[current-3],
             gtpuInitDbReq[current-2], gtpuInitDbReq[current-1] );
    adapterSocketInf_p[size - 1] = '\0';
}
/* - Coverity 31402 */
