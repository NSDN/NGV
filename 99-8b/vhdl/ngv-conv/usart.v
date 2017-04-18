module USART(
	stx, srx,
	tx, rx
);
	input stx, rx;
	output srx, tx;
	
	assign tx = stx;
	assign srx = rx;

endmodule
