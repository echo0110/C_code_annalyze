/*******************************************************************************
 *
 *  FILE NAME   : rrc_validation.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/

/***********************************************************************************
* Function Name  : rrc_validate_mac_address
* Inputs         : File MAC address
*                  System MAC Address
*
* Outputs        : None
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : This function validate MAC address.
***********************************************************************************/
S32 rrc_validate_mac_address(S8  *file_mac_addr, S8  *sys_mac_addr)
{
    U8 counter            = RRC_NULL;
    /* Converting stored MAC Address from string to integer */
    for (counter = RRC_NULL; counter < MAC_ADD_LEN; counter++)
    {
        if (file_mac_addr[counter] >= '0' && file_mac_addr[counter] <= '9')
           file_mac_addr[counter] = file_mac_addr[counter] - 48;
        else if (file_mac_addr[counter] == 'A')
           file_mac_addr[counter] = 10;
        else if (file_mac_addr[counter] == 'B')
           file_mac_addr[counter] = 11;
        else if (file_mac_addr[counter] == 'C')
           file_mac_addr[counter] = 12;
        else if (file_mac_addr[counter] == 'D')
           file_mac_addr[counter] = 13;
        else if (file_mac_addr[counter] == 'E')
           file_mac_addr[counter] = 14;
        else if (file_mac_addr[counter] == 'F')
           file_mac_addr[counter] = 15;
    }
    for (counter = RRC_NULL; counter < MAC_ADD_LEN; counter++)
    {
        /* Comparing file MAC Address and fetched System Mac Address Byte by Byte */
        if (file_mac_addr[counter] != sys_mac_addr[counter])
        {
            return RRC_FAILURE;
        }
    } 
    return RRC_SUCCESS;
}

/***********************************************************************************
* Function Name  : rrc_validate_duration
* Inputs         : File pointer
*                  
* Outputs        : None
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : This function licence time duration.
***********************************************************************************/
S32 rrc_validate_duration
(
    FILE  *fptr_limitsfile, 
    S8  *file_mac_addr, 
    U32 *p_left_duration
)
{
    /* Variables used for Date validation checking */
    U16 sys_days      = RRC_NULL;
    U16 left_days     = RRC_NULL;  
    U32 allowed_year  = RRC_NULL;
    U8 allowed_month  = RRC_NULL;
    U8 allowed_date   = RRC_NULL;
    U8 count          = RRC_NULL;
    U16 expiry_days   = RRC_NULL;
    U32 cur_year      = RRC_NULL;
    U8 cur_month      = RRC_NULL;
    U8 cur_date       = RRC_NULL;
    U8 flag           = RRC_FAILURE;
    
    /* Variable used For Time Validation */
    U8 cur_hour       = RRC_NULL;
    U8 cur_minute     = RRC_NULL;
    U8 cur_second     = RRC_NULL;
    U8 allowed_hour   = RRC_NULL;
    U8 allowed_minute = RRC_NULL;
    U8 allowed_second = RRC_NULL;
    U8 left_hours     = RRC_NULL;
    U32 total_left_hr  = RRC_NULL;
    struct tm *thetime;
    time_t epoch;
    S8   *token       = PNULL;
    S8 tstr[32];
    /* Checking for Date Validation */
    while (RRC_SUCCESS)
    {
        RRC_FSCANF (fptr_limitsfile, "%s", file_mac_addr);
        if (RRC_NULL == strcmp_wrapper("TIME-LIMIT", file_mac_addr))
        {
            RRC_FSCANF (fptr_limitsfile, "%s", file_mac_addr);
            RRC_FSCANF (fptr_limitsfile, "%s", file_mac_addr);
            
            epoch = time_wrapper((long*)PNULL);
            thetime = localtime_wrapper(&epoch);
            
            /* Fetching System Date */
            strftime_wrapper(tstr, sizeof (tstr), "%Y", thetime);
            cur_year = atoi_wrapper(tstr);
            strftime_wrapper(tstr, sizeof (tstr), "%m", thetime);
            cur_month = atoi_wrapper(tstr);
            strftime_wrapper(tstr, sizeof (tstr), "%d", thetime);
            cur_date = atoi_wrapper(tstr);
            
            /* Fetching Date from limitsfile */
            token = l3_strtok_wrapper(file_mac_addr, "-");
            allowed_year  = atoi_wrapper(token);
            token = l3_strtok_wrapper(PNULL, "-");
            allowed_month  = atoi_wrapper(token);
            token = l3_strtok_wrapper(PNULL, "-");
            allowed_date  = atoi_wrapper(token);
            
            /* Fetching system time */
            strftime_wrapper(tstr, sizeof (tstr), "%H", thetime);
            cur_hour = atoi_wrapper(tstr);
            strftime_wrapper(tstr, sizeof (tstr), "%M", thetime);
            cur_minute = atoi_wrapper(tstr);
            strftime_wrapper(tstr, sizeof (tstr), "%S", thetime);
            cur_second = atoi_wrapper(tstr);
            
            /* Fetching Time from limitsfile */
            RRC_FSCANF (fptr_limitsfile, "%s", file_mac_addr);
            token = l3_strtok_wrapper(file_mac_addr, ":");
            allowed_hour  = atoi_wrapper(token);
            token = l3_strtok_wrapper(PNULL, ":");
            allowed_minute  = atoi_wrapper(token);
            token = l3_strtok_wrapper(PNULL, ":");
            allowed_second  = atoi_wrapper(token);

            if (cur_year < allowed_year) 
            {
                flag = RRC_SUCCESS;
            }
            else if (cur_year == allowed_year)
            {
                if (cur_month < allowed_month)
                {
                    flag = RRC_SUCCESS;
                }
                else if (cur_month == allowed_month)
                {
                    if (cur_date < allowed_date)
                    {
                        flag = RRC_SUCCESS;
                    }
                    else if (cur_date == allowed_date)
                    {
                        if (cur_hour < allowed_hour) /* Validating HOURS */
                        {
                            flag = RRC_SUCCESS;
                        }
                        else if(cur_hour == allowed_hour)
                        {
                            if (cur_minute < allowed_minute) /* Validating MINUTES */
                            {
                                flag = RRC_SUCCESS; 
                            }
                            else if (cur_minute == allowed_minute)
                            {
                                if (cur_second < allowed_second) /* Validating SECONDS */   
                                {
                                    flag = RRC_SUCCESS;
                                }
                            }
                        }
                    }
                }
            }
            if (RRC_FAILURE == flag)
            {
                return flag;
            }

            /* Displaying a reminder message before 3 days of Licence Expiry */
            if ((1 ==(allowed_year - cur_year)) && 
            ((allowed_month == 1) && (cur_month == 12)))
            {
                if (allowed_hour > cur_hour)
                    left_days = 31 - cur_date + allowed_date;
                else
                    left_days = 31 - cur_date + allowed_date - 1;
                if (left_days <= 2)
                {
                    RRC_TRACE (RRC_INFO, "**** REMINDER :::"   
                    "YOUR LICENCE DIFFERENT YEAR WILL EXPIRE AFTER %d days ****\n", left_days);
                }
            }
            else if (allowed_year == cur_year)
            {   /* Converting store date in days */
                for (count = 1; count < allowed_month; count++)
                {
                    if (count == 1 || count == 3 || count == 5 || count == 7 || 
                        count == 8 || count == 10 || count == 12)   
                    {
                        expiry_days = expiry_days + 31;
                    }
                    else if (count == 2)
                    {   /* Checking Leap Year */
                        if ((RRC_NULL == (allowed_year % 100)) && (RRC_NULL == (allowed_year % 400)))
                        {
                            expiry_days = expiry_days + 29;
                        }
                        else if (RRC_NULL == (allowed_year % 4))
                        {
                            expiry_days = expiry_days + 29;
                        }
                        else
                        {
                            expiry_days = expiry_days + 28;
                        }
                    }   
                    else
                        expiry_days = expiry_days + 30;
                }
                expiry_days = expiry_days + allowed_date;

                /* Converting system date in days */
                for (count = 1; count < cur_month; count++)
                {
                    if (count == 1 || count == 3 || count == 5 || count == 7 ||
                        count == 8 || count == 10 || count == 12)
                    {
                        sys_days = sys_days + 31;
                    }      
                    else if (count == 2)
                    {   /* Checking Leap Year */
                        if ((RRC_NULL == (cur_year % 100)) && (RRC_NULL == (cur_year % 400)))
                        {
                            sys_days = sys_days + 29;
                        }
                        else if (RRC_NULL == (cur_year % 4))
                        {
                            sys_days = sys_days + 29;
                        }     
                        else
                        {
                            sys_days = sys_days + 28;
                        }
                    }    
                    else
                       sys_days = sys_days + 30;
                }
                sys_days = sys_days + cur_date; 
                /* Check for 3 days validation*/
                if ((allowed_hour > cur_hour)||
                    ((allowed_hour == cur_hour) &&( allowed_minute > cur_minute)))
                    left_days = expiry_days - sys_days;
                else
                   left_days = expiry_days - sys_days - 1;
                
                if (2 >= left_days)
                {
                    RRC_TRACE (RRC_INFO, "**** REMINDER :::"
                    "YOUR LICENCE WILL EXPIRE AFTER %d days ****\n", left_days);
                }
            }

            if(allowed_hour >= cur_hour)
                left_hours = allowed_hour - cur_hour;
            else
                left_hours = (24 - cur_hour) + allowed_hour;

            total_left_hr = 24 * left_days + left_hours;
            *p_left_duration = (total_left_hr)*3600000 + (allowed_minute - cur_minute)*60000;
        }
        if(RRC_FEOF(fptr_limitsfile))
        {
                break;
        }
    }
    return RRC_SUCCESS;
}    

/***********************************************************************************
* Function Name  : rrc_authentication 
* Inputs         : File pointer, Expiry duration 
*                  
* Outputs        : None
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : This function validate MAC address and time for rrc_entity exe.
***********************************************************************************/
S32 rrc_authentication
(
    FILE *fptr_limitsfile,
    U32 *p_expiry_duration
)
{
    U8  addr[6]                     = {RRC_NULL};
    S8  sys_mac_addr[MAC_ADD_LEN]   = {RRC_NULL};
    S8  file_mac_addr[MAC_ADD_LEN]  = {RRC_NULL};
    rrc_return_et stat              = RRC_FALSE; 
    U8  counter                     = RRC_NULL;
    U8  count                       = RRC_NULL;
    struct ifreq ifr;
    struct ifreq *IFR;
    struct ifconf ifc;
    S8 buf[1024];
    S32 sock_des = RRC_NULL;
    S32 eth_count = RRC_NULL;
    rrc_bool_t ok = RRC_FALSE;
    /* fetching MAC address from limitsfile */
    while (RRC_SUCCESS)
    {
        RRC_FSCANF(fptr_limitsfile, "%s", file_mac_addr);
        if (RRC_NULL == strcmp_wrapper("HOSTID", file_mac_addr))
        {
           RRC_FSCANF(fptr_limitsfile, "%s", file_mac_addr);
           RRC_FSCANF(fptr_limitsfile, "%s", file_mac_addr);
           break;
        }
    }  
    /* Fetch MAC address of machine */
    sock_des = socket_wrapper(AF_INET, SOCK_DGRAM, RRC_NULL);
    if (sock_des == RRC_SOCKET_ERR)
    {
       return RRC_SOCKET_ERR;
    }
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    ioctl_wrapper(sock_des, SIOCGIFCONF, &ifc);
    IFR = ifc.ifc_req;
    for (eth_count = ifc.ifc_len / sizeof(struct ifreq); --eth_count >= RRC_NULL; IFR++)
    {
        /*coverity_fix_531*/ 
        strcpy(ifr.ifr_name, IFR->ifr_name);
        /*coverity_fix_531*/ 

        if (ioctl_wrapper(sock_des, SIOCGIFFLAGS, &ifr) == RRC_NULL)
        {
            if (!(ifr.ifr_flags & IFF_LOOPBACK))
            {
               if (ioctl_wrapper(sock_des, SIOCGIFHWADDR, &ifr) == RRC_NULL)
               {
                   ok = RRC_TRUE;
               }
            }
        }
        if (ok)
        {
            bcopy (ifr.ifr_hwaddr.sa_data, addr, 6);

            /* Unpacking System MAC address */
            for (counter = RRC_NULL, count = RRC_NULL; 
            counter < MAC_ADD_LEN; counter++, count = count + 2)
            {
                sys_mac_addr[count] = addr[counter]>>4;
                sys_mac_addr[count + 1] = addr[counter] & 0x0F;
            }
            /* MAC address Validation */
            stat = rrc_validate_mac_address(file_mac_addr, sys_mac_addr);
            if (RRC_SUCCESS == stat)
                break;
        } 
    }

    if (RRC_FAILURE == stat) 
    {
        RRC_TRACE(RRC_FATAL,"MAC ADDRESS NOT MATCHED");
        rrc_fclose(fptr_limitsfile);
        close_wrapper(sock_des); 
        exit_wrapper(RRC_FAILURE);
    }    

    /* Time duration Validation */
    stat = rrc_validate_duration(fptr_limitsfile, file_mac_addr, p_expiry_duration);
    if (RRC_FAILURE == stat)
    {
        close_wrapper(sock_des);
        return RRC_FAILURE;
    }
    close_wrapper(sock_des);
    return RRC_SUCCESS;
}
