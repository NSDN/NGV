module LCD(
	sblk, scs, srs, swr, srd, srst, sdata,
	blk, cs, rs, wr, rd, rst, data
);
	input sblk, scs, srs, swr, srd, srst;
	input[15:0] sdata;
	output blk, cs, rs, wr, rd, rst;
	output[15:0] data;
	
	assign blk = sblk;
	assign cs = scs;
	assign rs = srs;
	assign wr = swr;
	assign rd = srd;
	assign rst = srst;
	assign data = sdata;
	
endmodule
