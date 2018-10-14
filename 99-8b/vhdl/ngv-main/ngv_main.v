module ngv_main(
	rst, clk,
	
	fmc_nwe, fmc_noe, fmc_ne, fmc_addr, fmc_data,
	lcd_blk, lcd_rs, lcd_wr, lcd_rd, lcd_rst, lcd_data,
	
	led_w, led_y
);
	
	input rst, clk;
	output led_w, led_y;
	
	input fmc_nwe, fmc_noe;
	input[3:0] fmc_ne;
	input[23:0] fmc_addr;
	input[15:0] fmc_data;
	
	output lcd_blk, lcd_rs, lcd_wr, lcd_rd, lcd_rst;
	output[23:0] lcd_data;
	
	wire freq_168;
	wire freq_1176;
	
	pll core_pll(
		.refclk(clk),
		.clk0_out(freq_168),
		.clk1_out(freq_1176)
	);
	
	lcd_trans lcd_conv(
		.i_blk(fmc_data[0]),
		.i_rs(fmc_data[1]),
		.i_wr(fmc_nwe),
		.i_rd(fmc_noe),
		.i_rst(fmc_data[2]),
		.i_data(fmc_addr),
		
		.o_blk(lcd_blk),
		.o_rs(lcd_rs),
		.o_wr(lcd_wr),
		.o_rd(lcd_rd),
		.o_rst(lcd_rst),
		.o_data(lcd_data)
	);

	assign led_w = fmc_addr[23];
	assign led_y = fmc_data[7];

	/*
	blink #(
		.CNT(32'd168000000)
	) bi_blink(
		.clk(freq_168),
		.rst(rst),
		.out1(led_w),
		.out2(led_y)
	);
	*/

endmodule
