module lcd(
	blk, cs, rs, wr, rd, rst, data
);
	output blk, cs, rs, wr, rd, rst;
	output[15:0] data;
	
	assign blk = 1'b1;
	
endmodule
