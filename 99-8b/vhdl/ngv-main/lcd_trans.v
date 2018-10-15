module lcd_trans(
	i_blk, i_cs, i_rs, i_wr, i_rd, i_rst, i_data,
	o_blk, o_cs, o_rs, o_wr, o_rd, o_rst, o_data
);

	input i_blk, i_cs, i_rs, i_wr, i_rd, i_rst;
	input[23:0] i_data;
	
	output o_blk, o_cs, o_rs, o_wr, o_rd, o_rst;
	output[23:0] o_data;
	
	assign o_blk = i_blk;
	assign o_cs = i_cs;
	assign o_rs = i_rs;
	assign o_wr = i_wr;
	assign o_rd = i_rd;
	assign o_rst = i_rst;
	assign o_data = i_data;

endmodule
