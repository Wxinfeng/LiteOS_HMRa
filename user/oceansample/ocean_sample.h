#ifndef _OCEAN_SAMPLE_H
#define _OCEAN_SAMPLE_H

void led_set_status(void);
int led_get_status(void);
int LOS_SetIoTGW_AddrInfo(const char *ipaddr, const char *port);
int LOS_OceanCon_Init(void);
void LOS_OceanCon_Sample(void);

#endif
