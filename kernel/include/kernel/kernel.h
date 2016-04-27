#ifndef _KERNEL_H_

#define _KERNEL_H_
typedef struct {
	/*
	 * BDA data
	 */
	uint16_t iobase;
	uint8_t displaymode;
	uint8_t cols;
	uint8_t rows;
} rho_context;

extern rho_context rho;

#endif
