/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright 2020 Nvidia, Ltd
 */

#ifndef DPI_PARSER_H
#define DPI_PARSER_H

enum dpi_parser_protocol {
	DPI_PARSER_HTTP,
	DPI_PARSER_FTP
};

struct dpi_parser_ctx;

struct dpi_parser_ctx *dpi_parser_ctx_alloc();
void dpi_parser_ctx_dealloc(struct dpi_parser_ctx *ctx);

/*
 * Parse new flow, or continue parsing prev flow.
 * Parser should be able to store the parsing state on the ctx.
 * Parser should store the mbuf pointer for field retrival in the future.
 * returns 0 on sucess.
 * Error code otherwise: TBD
 * Offset represents the L4 payload
 */
int dpi_parser_parse(struct rte_mbuf *mbuf, uint32_t payload_offset, struct dpi_parser_ctx *ctx);

/* 
 * Returns the protocol ID
 */
int dpi_parser_protocol_get(const struct dpi_parser_ctx *ctx);

/*
 * Returns string to the value of the given field.
 * NULL means not found
 */
struct mbuf* dpi_parser_field_get(const struct dpi_parser_ctx *ctx,
				  int field_id);

#endif
