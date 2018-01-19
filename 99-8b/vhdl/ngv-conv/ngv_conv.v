`timescale 1ns/1ps

module ngv_conv(
	pclk, prst,
	r, g, b,
	
	sblk, scs, srs, swr, srd, srst, sdata,
	blk, cs, rs, wr, rd, rst, data,
);

	input pclk, prst;
	output r, g, b;
	Blink blink1(pclk, prst, r, g, b);
	
	input sblk, scs, srs, swr, srd, srst;
	input[23:0] sdata;
	output blk, cs, rs, wr, rd, rst;
	output[23:0] data;
	LCD lcd1(
		sblk, scs, srs, swr, srd, srst, sdata,
		blk, cs, rs, wr, rd, rst, data
	);
	
endmodule
