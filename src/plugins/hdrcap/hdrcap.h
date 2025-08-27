#ifndef __included_hdrcap_h__
#define __included_hdrcap_h__

typedef struct {
	vlib_main_t * vnet_main;
	uint16_t msg_id_base; 
} hdrcap_main_t;


hdrcap_main_t * hdrcap_main;

#endif // __included_hdrcap_h__