`timescale 1ns/1ps

module ngv_conv(
	pclk, prst,
	r, g, b,
	
	sblk, scs, srs, swr, srd, srst, sdata,
	blk, cs, rs, wr, rd, rst, data,
	
	stx, srx,
	tx, rx,
	
	unused
);
	input[2:0] unused;

	input pclk, prst;
	output r, g, b;
	Blink blink1(pclk, prst, r, g, b);
	
	input sblk, scs, srs, swr, srd, srst;
	input[15:0] sdata;
	output blk, cs, rs, wr, rd, rst;
	output[15:0] data;
	LCD lcd1(
		sblk, scs, srs, swr, srd, srst, sdata,
		blk, cs, rs, wr, rd, rst, data
	);
	
	input stx, rx;
	output srx, tx;
	USART usart1(stx, srx, tx, rx);
	
endmodule
