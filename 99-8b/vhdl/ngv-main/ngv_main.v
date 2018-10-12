module ngv_main(rst, clk, out1, out2);

	parameter CNT = 32'd168000000;
	
	input rst, clk;
	output out1, out2;
	
	wire freq_168;
	wire freq_1176;
	
	pll core_pll(
		.refclk(clk),
		.reset(rst),
		.clk0_out(freq_168),
		.clk1_out(freq_1176)
	);

	reg[31:0] cnt;
	
	initial begin
		cnt <= 32'b0;
	end
	
	always @(posedge freq_168 or negedge rst) begin
		if (!rst) begin
			cnt <= 32'b0;
		end else begin
		
			if (cnt < CNT) cnt <= cnt + 32'b1;
			else cnt <= 32'b0;
			
		end
	end
	
	wire cnt_out;
	assign cnt_out = (cnt == CNT);
	
	reg state;
	
	initial begin
		state <= 1'b0;
	end
	
	always @(posedge cnt_out or negedge rst) begin
		if (!rst) begin
			state <= 1'b0;
		end else begin
			state <= !state;
		end
	end
	
	assign out1 = state;
	assign out2 = !state;

endmodule
