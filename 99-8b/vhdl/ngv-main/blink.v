module blink(clk, rst, out1, out2);

	parameter CNT = 32'b1;

	input clk, rst;
	output out1, out2;
	
	reg[31:0] cnt;
	reg state;
	
	initial begin
		cnt <= 32'b0;
		state <= 1'b0;
	end
	
	always @(posedge clk or negedge rst) begin
		if (!rst) begin
			cnt <= 32'b0;
			state <= 1'b0;
		end else begin
			if (cnt < CNT) cnt <= cnt + 32'b1;
			else begin
				cnt <= 32'b0;
				state <= !state;
			end
		end
	end
	
	assign out1 = state;
	assign out2 = !state;

endmodule
