/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright 2020 Nvidia, Ltd
 */

#include <rte_regexdev.h>

#include "dpi_parser.h"

int process_packet_enqueue(struct rte_mbuf *pkt, uint32_t flow_id)
{
	/*
	sft_client_object_get(flow_id);
	if (no object)
		IANA check
		create new object
	
	dpi_parser_parse(pkt, obj->parse_ctx);

	if (parser_done(obj->parse_ctx)) {
		Send packet to regex engine (user_id == flow_id).
		return PROCESSING
	} //else need to wait for next packet
	return NEED_MORE_PACKETS
	*/
}

int process_packet_dequeue()
{
	/*
	num_res = rte_regexedv_dequeue(&jobs);
	for i in num_res 
		obj = sft_client_object_get(jobs[i]->user_id);
		dpi_post_process(jobs[i]->matches, &obj, post_process_table);
	*/
}
