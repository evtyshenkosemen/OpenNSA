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
  * \file     util.c
  *
  * \brief    OpenNSL utility routines required for example applications
  *
  * \details  OpenNSL utility routines required for example applications
  *
  ************************************************************************/
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <bcm/types.h>
 //#include <sal/driver.h>
 #include <bcm/error.h>
 #include <bcm/init.h>
 #include <bcm/l2.h>
 #include <bcm/vlan.h>
 #include <bcm/stg.h>
 #include <bcm/link.h>
 #include <util.h>
 
 #define DEFAULT_VLAN          1
 #define MAX_DIGITS_IN_CHOICE  5
 
 /*************************************************************************/
 int example_is_dnx_device(int unit)
 {
   int rv = FALSE;
   bcm_info_t info;
   bcm_info_get(unit, &info);
 
   if(info.device == 0x8375) /* Qumran MX */
   {
     rv = TRUE;
   }
   return rv;
 }
 /* __doxy_func_body_end__ */
 
 /*************************************************************************/
 int example_is_qmx_device(int unit)
 {
   int rv = FALSE;
   bcm_info_t info;
   bcm_info_get(unit, &info);
 
   if(info.device == 0x8375) /* Qumran MX */
   {
     rv = TRUE;
   }
   return rv;
 }
 /* __doxy_func_body_end__ */
 
 /*****************************************************************/
 int example_port_default_config(int unit)
 {
   bcm_port_config_t pcfg;
   bcm_port_info_t info;
   int rv;
   int port;
   int stp_state = BCM_STG_STP_FORWARD;
   int stg = 1;
   int dnx_device = FALSE;
 
   dnx_device = example_is_dnx_device(unit);
   /*
    * Create VLAN with id DEFAULT_VLAN and
    * add ethernet ports to the VLAN
    */
   bcm_port_config_t_init(&pcfg);
 
   rv = bcm_port_config_get(unit, &pcfg);
   if (rv != BCM_E_NONE)
   {
     printf("Failed to get port configuration. Error %s\n", bcm_errmsg(rv));
     return rv;
   }
 
   /* Set the STP state to forward in default STG for all ports */
   BCM_PBMP_ITER(pcfg.e, port)
   {
     rv = bcm_stg_stp_set(unit, stg, port, stp_state);
     if (rv != BCM_E_NONE)
     {
       printf("Failed to set STP state for unit %d port %d, Error %s\n",
           unit, port, bcm_errmsg(rv));
       return rv;
     }
   }
 
   /* Setup default configuration on the ports */
   bcm_port_info_t_init(&info);
 
   if(dnx_device == FALSE)
   {
     info.speed        = 0;
   }
   info.duplex       = BCM_PORT_DUPLEX_FULL;
   info.pause_rx     = BCM_PORT_ABILITY_PAUSE_RX;
   info.pause_tx     = BCM_PORT_ABILITY_PAUSE_TX;
   info.linkscan     = BCM_LINKSCAN_MODE_SW;
   info.autoneg      = FALSE;
   info.enable = 1;
 
   info.action_mask |= ( BCM_PORT_ATTR_AUTONEG_MASK |
       BCM_PORT_ATTR_DUPLEX_MASK   |
       BCM_PORT_ATTR_PAUSE_RX_MASK |
       BCM_PORT_ATTR_PAUSE_TX_MASK |
       BCM_PORT_ATTR_LINKSCAN_MASK |
       BCM_PORT_ATTR_ENABLE_MASK   );
 
   if(dnx_device == FALSE)
   {
     info.action_mask |= BCM_PORT_ATTR_SPEED_MASK;
   }
   BCM_PBMP_ITER(pcfg.e, port)
   {
     rv = bcm_port_selective_set(unit, port, &info);
     if (BCM_FAILURE(rv))
     {
       printf("Failed to set port config for unit %d, port %d, Error %s",
           unit, port, bcm_errmsg(rv));
       return rv;
     }
   }
 
   return BCM_E_NONE;
 }
 /* __doxy_func_body_end__ */
 
 /*****************************************************************/
 int example_switch_default_vlan_config(int unit)
 {
   bcm_port_config_t pcfg;
   int port;
   int rv;
 
   /*
    * Create VLAN with id DEFAULT_VLAN and
    * add ethernet ports to the VLAN
    */
   rv = bcm_port_config_get(unit, &pcfg);
   if (rv != BCM_E_NONE) {
     printf("Failed to get port configuration. Error %s\n", bcm_errmsg(rv));
     return rv;
   }
 
   rv = bcm_vlan_port_add(unit, DEFAULT_VLAN, pcfg.e, pcfg.e);
   if (rv != BCM_E_NONE) {
     printf("Failed to add ports to VLAN. Error %s\n", bcm_errmsg(rv));
     return rv;
   }
 
   BCM_PBMP_ITER(pcfg.e, port)
   {
     rv = bcm_port_untagged_vlan_set(unit, port, DEFAULT_VLAN);
     if (BCM_FAILURE(rv))
     {
       printf("Failed to set port untagged VLAN for unit %d, port %d, Error %s",
           unit, port, bcm_errmsg(rv));
       return rv;
     }
   }
 
   return BCM_E_NONE;
 }
 /* __doxy_func_body_end__ */
 
 /**************************************************************************/
 char *example_read_user_string(char *buf, size_t buflen)
 {
   int ch;
 
   if (fgets(buf, buflen, stdin) != 0)
   {
     size_t len = strlen(buf);
     if (len > 0 && buf[len-1] == '\n')
     {
       buf[len-1] = '\0';
     }
     else
     {
       while ((ch = getc(stdin)) != EOF && ch != '\n');
     }
     return buf;
   }
   return 0;
 }
 /* __doxy_func_body_end__ */
 /**************************************************************************/
 int example_read_user_choice(int *choice)
 {
   char val;
   char digits[MAX_DIGITS_IN_CHOICE + 1];
   int idx = 0;
   int valid = TRUE;
 
   /* parse input string until \n */
   while((val = getchar()) != '\n')
   {
     if ((val >= '0' && val <= '9') && idx < MAX_DIGITS_IN_CHOICE)
     {
       digits[idx++] = val;
     }
     else
     {
       valid = FALSE;
     }
   }
   if ((valid == TRUE) && idx != 0)
   {
     digits[idx] = '\0';
     *choice = atoi(digits);
     return(BCM_E_NONE);
   }
   else
   {
     *choice = -1;
     return(BCM_E_FAIL);
   }
 }
 /* __doxy_func_body_end__ */
 
 /*****************************************************************/
 int bcm_mac_parse(char *buf, unsigned char *macp)
 {
   int   i, c1, c2;
   char  *s;
 #define MAC_ADDR_LEN 17
 
   macp[0] = macp[1] = macp[2] = 0;
   macp[3] = macp[4] = macp[5] = 0;
 
   if ((buf == NULL) || (strlen(buf) > MAC_ADDR_LEN)) {
     return BCM_E_FAIL;
   }
 
   /* skip leading 0x if plain hex format */
   if (buf[0] == '0' && (buf[1] == 'x' || buf[1] == 'X')) {
     buf += 2;
   }
 
   /* start at end of string and work backwards */
   for (s = buf; *s; s++) {
     ;
   }
   for (i = 5; i >= 0 && s >= buf; i--) {
     c1 = c2 = 0;
     if (--s >= buf) {
       if (*s >= '0' && *s <= '9') {
         c2 = *s - '0';
       } else if (*s >= 'a' && *s <= 'f') {
         c2 = *s - 'a' + 10;
       } else if (*s >= 'A' && *s <= 'F') {
         c2 = *s - 'A' + 10;
       } else if (*s == ':') {
         ;
       } else {
         return BCM_E_FAIL;
       }
     }
     if (*s != ':' && --s >= buf) {
       if (*s >= '0' && *s <= '9') {
         c1 = *s - '0';
       } else if (*s >= 'a' && *s <= 'f') {
         c1 = *s - 'a' + 10;
       } else if (*s >= 'A' && *s <= 'F') {
         c1 = *s - 'A' + 10;
       } else if (*s == ':') {
         ;
       } else {
         return BCM_E_FAIL;
       }
     }
 
     if (s > buf && s[-1] == ':') {
       --s;
     }
     macp[i] = c1 << 4 | c2;
   }
   return BCM_E_NONE;
 }
 /* __doxy_func_body_end__ */
 
 /*****************************************************************/
 void l2_print_mac(char *str, bcm_mac_t mac_address){
   unsigned int a,b,c,d,e,f;
   a = 0xff & mac_address[0];
   b = 0xff & mac_address[1];
   c = 0xff & mac_address[2];
   d = 0xff & mac_address[3];
   e = 0xff & mac_address[4];
   f = 0xff & mac_address[5];
   printf("%s %02x:%02x:%02x:%02x:%02x:%02x",
       str,
       a,b,c,
       d,e,f);
   return;
 }
 /* __doxy_func_body_end__ */
 
 /*****************************************************************/
 int bcm_ip_parse(char *ip_str, unsigned int *ip_val)
 {
   unsigned int num = 0, val;
   char *tok;
   int count = 0;
   char buf[16]; /* Maximum length of IP address in dotten notation */
 
   if((ip_str == NULL) || (ip_val == NULL) || (strlen(ip_str) > 16))
   {
     return -1;
   }
   strcpy(buf, ip_str);
   tok = strtok(buf, ".");
   while(tok != NULL)
   {
     count++;
     val = atoi(tok);
     if((val < 0) || (val > 0xff))
     {
       return -1;
     }
     num = (num << 8) + val;
     tok = strtok(NULL, ".");
   }
   if(count != 4)
   {
     return -1;
   }
   *ip_val = num;
 
   return 0;
 }
 /* __doxy_func_body_end__ */
 
 /**************************************************************************/
 void print_ip_addr(char *str, unsigned int host)
 {
   int a,b,c,d;
 
   a = (host >> 24) & 0xff;
   b = (host >> 16) & 0xff;
   c = (host >> 8 ) & 0xff;
   d = host & 0xff;
   printf("%s %d.%d.%d.%d", str, a,b,c,d);
 }
 /* __doxy_func_body_end__ */
 
 /**************************************************************************/
 int bcm_ctoi(const char *s, char **end)
 {
   unsigned int  n, neg;
   int base = 10;
 
   if (s == 0) {
     if (end != 0) {
       end = 0;
     }
     return 0;
   }
 
   s += (neg = (*s == '-'));
 
   if (*s == '0') {
     s++;
     if (*s == 'x' || *s == 'X') {
       base = 16;
       s++;
     } else if (*s == 'b' || *s == 'B') {
       base = 2;
       s++;
     } else {
       base = 8;
     }
   }
 
   for (n = 0; ((*s >= 'a' && *s < 'a' + base - 10) ||
         (*s >= 'A' && *s < 'A' + base - 10) ||
         (*s >= '0' && *s <= '9')); s++) {
     n = n * base + ((*s <= '9' ? *s : *s + 9) & 15);
   }
 
   if (end != 0) {
     *end = (char *) s;
   }
 
   return (int) (neg ? -n : n);
 }
 /* __doxy_func_body_end__ */
 
 /**************************************************************************/
 int example_max_port_count_get(int unit, int *count)
 {
   int rc;
   bcm_port_config_t pcfg;
   int num_ports;
   int num_front_panel_ports;
 
   rc = bcm_port_config_get(unit, &pcfg);
   if (rc != BCM_E_NONE) {
     printf("Failed to get port configuration. Error %s\n", bcm_errmsg(rc));
     return rc;
   }
 
   BCM_PBMP_COUNT(pcfg.ge, num_ports);
   num_front_panel_ports = num_ports;
   BCM_PBMP_COUNT(pcfg.xe, num_ports);
   num_front_panel_ports += num_ports;
 
   *count = num_front_panel_ports;
   return rc;
 }
 /* __doxy_func_body_end__ */