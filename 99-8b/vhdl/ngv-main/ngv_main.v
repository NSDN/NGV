module ngv_main(
	rst, clk,
	
	fmc_nwe, fmc_noe, fmc_ne, fmc_addr, fmc_data,

	fmc_nbl0, fmc_nbl1, fmc_sdne0, fmc_sdcke0,
	fmc_sdnwe, fmc_sdclk, fmc_sdnras, fmc_sdncas,
	
	dram_dqmh, dram_dqml, dram_cs, dram_cke,
	dram_nwe, dram_clk, dram_nras, dram_ncas,
	
	ctl_blk, ctl_cs, ctl_rs, ctl_rst,
	usart_tx, usart_rx,
	
	lcd_blk, lcd_cs, lcd_rs, lcd_wr, lcd_rd, lcd_rst, lcd_data,
	
	beep_l, beep_r,
	led_w, led_y
);

	input usart_tx, usart_rx;
	output beep_l, beep_r;
	
	input rst, clk;
	output led_w, led_y;
	
	input fmc_nbl0, fmc_nbl1, fmc_sdne0, fmc_sdcke0;
	input fmc_sdnwe, fmc_sdclk, fmc_sdnras, fmc_sdncas;
	output dram_dqmh, dram_dqml, dram_cs, dram_cke;
	output dram_nwe, dram_clk, dram_nras, dram_ncas;
	
	input fmc_nwe, fmc_noe;
	input[3:0] fmc_ne;
	input[23:0] fmc_addr;
	input[15:0] fmc_data;
	
	input ctl_blk, ctl_cs, ctl_rs, ctl_rst;
	
	output lcd_blk, lcd_cs, lcd_rs, lcd_wr, lcd_rd, lcd_rst;
	output[23:0] lcd_data;
	
	wire freq_168;
	wire freq_1176;
	
	pll core_pll(
		.refclk(clk),
		.clk0_out(freq_168),
		.clk1_out(freq_1176)
	);
	
	assign beep_l = usart_tx;
	assign beep_r = usart_rx;
	
	lcd_trans lcd_conv(
		.i_blk(ctl_blk),
		.i_cs(ctl_cs),
		.i_rs(ctl_rs),
		.i_rst(ctl_rst),
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
	
	dram_trans dram_conv(
		.nbl0(fmc_nbl0),
		.nbl1(fmc_nbl1),
		.sdne(fmc_sdne0),
		.sdcke(fmc_sdcke0),
		.sdnwe(fmc_sdnwe),
		.sdclk(fmc_sdclk),
		.sdnras(fmc_sdnras),
		.sdncas(fmc_sdncas),
		.dqmh(dram_dqmh),
		.dqml(dram_dqml),
		.cs(dram_cs),
		.cke(dram_cke),
		.nwe(dram_nwe),
		.clk(dram_clk),
		.nras(dram_nras),
		.ncas(dram_ncas)
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
