// Verilog netlist created by TD v4.1.389
// Tue Oct 16 22:12:07 2018

`timescale 1ns / 1ps
module pll  // E:/Works/NGV/99-8b/vhdl/ngv-main/al_ip/pll.v(23)
  (
  refclk,
  clk0_out,
  clk1_out
  );

  input refclk;  // E:/Works/NGV/99-8b/vhdl/ngv-main/al_ip/pll.v(27)
  output clk0_out;  // E:/Works/NGV/99-8b/vhdl/ngv-main/al_ip/pll.v(28)
  output clk1_out;  // E:/Works/NGV/99-8b/vhdl/ngv-main/al_ip/pll.v(29)

  wire clk0_buf;  // E:/Works/NGV/99-8b/vhdl/ngv-main/al_ip/pll.v(31)

  AL_PHY_GCLK bufg_feedback (
    .clki(clk0_buf),
    .clko(clk0_out));  // E:/Works/NGV/99-8b/vhdl/ngv-main/al_ip/pll.v(33)
  AL_PHY_PLL #(
    .CLKC0_CPHASE(7),
    .CLKC0_DIV(7),
    .CLKC0_ENABLE("ENABLE"),
    .CLKC0_FPHASE("0"),
    .CLKC1_CPHASE(1),
    .CLKC1_DIV(1),
    .CLKC1_ENABLE("ENABLE"),
    .CLKC1_FPHASE("0"),
    .CLKC2_CPHASE(1),
    .CLKC2_DIV(1),
    .CLKC2_ENABLE("DISABLE"),
    .CLKC2_FPHASE("0"),
    .CLKC3_CPHASE(1),
    .CLKC3_DIV(1),
    .CLKC3_ENABLE("DISABLE"),
    .CLKC3_FPHASE("0"),
    .CLKC4_CPHASE(1),
    .CLKC4_DIV(1),
    .CLKC4_ENABLE("DISABLE"),
    .CLKC4_FPHASE("0"),
    .DERIVE_PLL_CLOCKS("ENABLE"),
    .DPHASE_SOURCE("DISABLE"),
    .FBCLK_DIV(7),
    .FEEDBK_PATH("CLKC0_EXT"),
    .FIN("24.000"),
    .FREQ_LOCK_ACCURACY("2"),
    .GEN_BASIC_CLOCK("ENABLE"),
    .GMC_GAIN("6"),
    .GMC_TEST("14"),
    .ICP_CURRENT(3),
    .INTFB_WAKE("DISABLE"),
    .INT_FEEDBK_PATH("VCO_PHASE_0"),
    .KVCO("6"),
    .LPF_CAPACITOR("3"),
    .LPF_RESISTOR(2),
    .NORESET("DISABLE"),
    .ODIV_MUXC0("DIV"),
    .ODIV_MUXC1("DIV"),
    .ODIV_MUXC2("DIV"),
    .ODIV_MUXC3("DIV"),
    .ODIV_MUXC4("DIV"),
    .PLLC2RST_ENA("DISABLE"),
    .PLLC34RST_ENA("DISABLE"),
    .PLLMRST_ENA("DISABLE"),
    .PLLRST_ENA("DISABLE"),
    .PLL_LOCK_MODE("0"),
    .PREDIV_MUXC0("VCO"),
    .PREDIV_MUXC1("VCO"),
    .PREDIV_MUXC2("VCO"),
    .PREDIV_MUXC3("VCO"),
    .PREDIV_MUXC4("VCO"),
    .REFCLK_DIV(1),
    .REFCLK_SEL("INTERNAL"),
    .STDBY_ENABLE("DISABLE"),
    .STDBY_VCO_ENA("DISABLE"),
    .SYNC_ENABLE("DISABLE"),
    .VCO_NORESET("DISABLE"))
    pll_inst (
    .fbclk(clk0_out),
    .phasestep(1'b0),
    .phaseupdown(1'b0),
    .phcntsel(3'b000),
    .pllreset(1'b0),
    .refclk(refclk),
    .scanclk(1'b0),
    .stdby(1'b0),
    .clkc({open_n0,open_n1,open_n2,clk1_out,clk0_buf}));  // E:/Works/NGV/99-8b/vhdl/ngv-main/al_ip/pll.v(67)

endmodule 

