`define CNT 32'd12000000

module blink(
	clk, rst,
	r, g, b
);
	input clk, rst;
	output r, g, b;
	reg[31:0] cnt;
	reg[2:0] step;
	
	initial begin
		cnt <= 32'b0;
		step <= 3'b1;
	end
	
	always @(posedge clk or negedge rst) begin
		if (!rst) begin
			cnt <= 32'b0;
			step <= 3'b1;
		end else begin
		
			if (cnt < `CNT) cnt <= cnt + 32'b1;
			else begin
				cnt <= 32'b0;
				if (step == 3'b100) step <= 3'b1;
				else step <= step << 1'b1;
			end
			
		end
	end
	
	assign r = !step[0];
	assign g = !step[1];
	assign b = !step[2];

endmodule
