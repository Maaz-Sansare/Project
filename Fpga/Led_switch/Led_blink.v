`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:58:22 06/19/2022 
// Design Name: 
// Module Name:    Led_blink 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Led_blink(
    input [7:0] switch,
    output [7:0] led,
    input clk
    );
// assign led to switch

assign led[0] = switch[0];
assign led[1] = switch[1];
assign led[2] = switch[2];
assign led[3] = switch[3];
assign led[4] = switch[4];
assign led[5] = switch[5];
assign led[6] = switch[6];
assign led[7] = switch[7];


endmodule
