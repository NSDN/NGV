module ngv_main(
	rst, clk,
	
	fmc_nwe, fmc_noe, fmc_ne, fmc_addr, fmc_data,
	lcd_blk, lcd_cs, lcd_rs, lcd_wr, lcd_rd, lcd_rst, lcd_data,
	
	led_w, led_y
);
	
	input rst, clk;
	output led_w, led_y;
	
	input fmc_nwe, fmc_noe;
	input[3:0] fmc_ne;
	input[23:0] fmc_addr;
	input[15:0] fmc_data;
	
	output lcd_blk, lcd_cs, lcd_rs, lcd_wr, lcd_rd, lcd_rst;
	output[23:0] lcd_data;
	
	wire freq_168;
	wire freq_1176;
	
	pll core_pll(
		.refclk(clk),
		.clk0_out(freq_168),
		.clk1_out(freq_1176)
	);
	
	reg[3:0] lcdCtrl;
	
	initial begin
		lcdCtrl <= 4'b0;
	end
	
	always @(negedge fmc_nwe or negedge rst) begin
		if (!rst) begin
			lcdCtrl <= 4'b0;
		end else begin
			lcdCtrl <= fmc_data[3:0];
		end
	end
	
	lcd_trans lcd_conv(
		.i_blk(lcdCtrl[0]),
		.i_cs(lcdCtrl[1]),
		.i_rs(lcdCtrl[2]),
		.i_rst(lcdCtrl[3]),
		.i_wr(fmc_nwe),
		.i_rd(fmc_noe),
		.i_data(fmc_addr),
		
		.o_blk(lcd_blk),
		.o_cs(lcd_cs),
		.o_rs(lcd_rs),
		.o_wr(lcd_wr),
		.o_rd(lcd_rd),
		.o_rst(lcd_rst),
		.o_data(lcd_data)
	);

	blink #(
		.CNT(32'd168000000)
	) bi_blink(
		.clk(freq_168),
		.rst(rst),
		.out1(led_w),
		.out2(led_y)
	);

endmodule
