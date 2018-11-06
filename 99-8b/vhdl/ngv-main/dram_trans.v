module dram_trans(
	nbl0, nbl1, sdne, sdcke, sdnwe, sdclk, sdnras, sdncas,
	dqmh, dqml, cs, cke, nwe, clk, nras, ncas,
);

	input nbl0, nbl1, sdne, sdcke, sdnwe, sdclk, sdnras, sdncas;
	output dqmh, dqml, cs, cke, nwe, clk, nras, ncas;
	
	assign dqmh	= nbl1;
	assign dqml	= nbl0;
	assign cs	= sdne;
	assign cke	= sdcke;
	assign nwe	= sdnwe;
	assign clk	= sdclk;
	assign nras	= sdnras;
	assign ncas	= sdncas;

endmodule
