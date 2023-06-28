 /*********************************************************************
  *
  * (C) Copyright Broadcom Corporation 2013-2017
  *
  *  Licensed under the Apache License, Version 2.0 (the "License");
  *  you may not use this file except in compliance with the License.
  *  You may obtain a copy of the License at
  *
  *      http://www.apache.org/licenses/LICENSE-2.0
  *
  *  Unless required by applicable law or agreed to in writing, software
  *  distributed under the License is distributed on an "AS IS" BASIS,
  *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  *  See the License for the specific language governing permissions and
  *  limitations under the License.
  *
  **********************************************************************
  * \file     util.h
  *
  * \brief    OpenNSL utility routines required for example applications
  *
  * \details  OpenNSL utility routines required for example applications
  *
  ************************************************************************/
 #ifndef _OPENNSL_EXAMPLE_UTIL_H
 #define _OPENNSL_EXAMPLE_UTIL_H
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 //#include <sal/driver.h>
 
 #define OPENNSL_BOOT_F_DEFAULT          0x000000
 #define OPENNSL_BOOT_F_WARM_BOOT        0x200000
 
 /*************************************************************************/
 int example_is_dnx_device(int unit);
 
 /*************************************************************************/
 int example_is_qmx_device(int unit);
 
 /*****************************************************************/
 int example_port_default_config(int unit);
 
 /*****************************************************************/
 int example_switch_default_vlan_config(int unit);
 
 /**************************************************************************/
  char *example_read_user_string(char *buf, size_t buflen);
 
 /**************************************************************************/
 int example_read_user_choice(int *choice);
 
 /*****************************************************************/
 int opennsl_mac_parse(char *buf, unsigned char *macp);
 
 /*****************************************************************/
 void l2_print_mac(char *str, bcm_mac_t mac_address);
 
 /*****************************************************************/
 int opennsl_ip_parse(char *ip_str, unsigned int *ip_val);
 
 /**************************************************************************/
 void print_ip_addr(char *str, unsigned int host);
 
 /**************************************************************************/
 int example_max_port_count_get(int unit, int *count);
 
 #endif /* _OPENNSL_EXAMPLE_UTIL_H */