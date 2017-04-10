`timescale 1ns/1ps

module ngv_test(
	pclk, prst,
	r, g, b,
	blk, cs, rs, wr, rd, rst, data
);
	input pclk, prst;
	output r, g, b;
	
	output reg blk, cs, rs, wr, rd, rst;
	output[15:0] data;
	
	blink blink1(pclk, prst, r, g, b);
	//lcd lcd1(blk, cs, rs, wr, rd, rst, data);
	
	always @(prst) begin
		blk <= 1'b1;
		rst <= prst;
	end
	
endmodule
