/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright 2020 Nvidia, Ltd
 */

#ifndef DPI_PARSER_H
#define DPI_PARSER_H

/*
 * The DPI parser is designed to parse the application layer in a flow.
 * Because it can spread accross packets, the parser needs to store the parsing
 * state of the flow, so it can continue parsing in the next packet.
 * 
 * The user can query the parsing results at any state of the flow parsing,
 * But should be aware that if parsing is not done the parsing results are incomplete.
 * 
 * For performance reasons, The parser will not check the correctness of protocol and
 * may produce false positives.
 * 
 * The input to the parser is mbuf, which represents a packet. The user should provide
 * parsing offset of each mbuf provided, i.e. the begining of the L4 payload.
 * 
 * The parsing result is also reported with mbufs. The parser is using the built-in ability
 * of the mbuf to represent a gather list. For example, URI value which is spread accross
 * multiple packets, will be representd by mutiple mbufs, linked to each other.
 * mbuf is also the input to the regex engine, so it saves redundant traslations when
 * preparing a regex job.
 * 
 * Becuase the parser does not copy the packet but holds a reference to the mbuf,
 * user should not delete or reuse the mbuf while the flow context exists.
 * 
 * A typical flow would be:
 * 
 * 	if (new flow)
 * 		client_obj->p_ctx = dpi_parser_ctx_alloc()
 * 	else
 * 		client_obj = sft_client_obj_get(flow_id)
 * 	state = dpi_parser_parse(mbuf, offset, client_obj->p_ctx)
 * 	if (state == DPI_PARSER_DONE)
 * 		...
 * 		protocol = dpi_parser_protocol_get(client_obj->p_ctx)
 * 		uri = dpi_parser_field_get(HTTP_URI_CTS, ctx)
 * 		...
 * 
 * 	if (state == DPI_PARSER_PARTIAL)
 * 		...
 * 		protocol = dpi_parser_protocol_get(client_obj->p_ctx)
 * 		uri = dpi_parser_field_get(HTTP_URI_CTS, ctx)
 * 		...
 * 
 * When done regex/post processing
 * 	dpi_parser_ctx_dealloc(client_obj->p_ctx)
 */

enum dpi_parser_protocol {
	DPI_PARSER_HTTP,
	DPI_PARSER_FTP
};

enum dpi_parser_state {
	DPI_PARSER_INIT,	/* Did not parse any thing yet, payload empty. */
	DPI_PARSER_PARTIAL, 	/* Some fields may be already parsed. */
	DPI_PARSER_DONE, 	/* TBD, when are we done? */
	DPI_PARSER_ERROR 	/* Did not recognize protocol. */
}

struct dpi_parser_ctx;

struct dpi_parser_ctx *dpi_parser_ctx_alloc();
void dpi_parser_ctx_dealloc(struct dpi_parser_ctx *ctx);

/*
 * Parse new flow, or continue parsing prev flow.
 * Parser should be able to store the parsing state on the ctx.
 * Parser should store the mbuf pointer for field retrival in the future.
 * returns the parsing state on sucess.
 * Offset represents the L4 payload start.
 */
int dpi_parser_parse(const struct rte_mbuf *mbuf, uint32_t payload_offset,
		     struct dpi_parser_ctx *ctx);

/* 
 * Returns the protocol ID
 */
int dpi_parser_protocol_get(const struct dpi_parser_ctx *ctx);

/*
 * Returns mbuf pointing to the value of the given field.
 * NULL means means not found.
 */
const struct mbuf* dpi_parser_field_get(const struct dpi_parser_ctx *ctx,
					int field_id);

#endif
