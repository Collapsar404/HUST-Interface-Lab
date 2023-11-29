`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2016/02/17 09:16:32
// Design Name: 
// Module Name: PWM_Controller_Int
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module PWM_Controller_Int #(
    parameter integer period = 20)
(
    input Clk,
    input [31:0] DutyCycle, 
    input Reset,
    output reg [1:0] PWM_out,
	output reg Interrupt,
	output reg [19:0] count,
	output reg [7:0] AN,
    output wire [6:0] Digits_Bits
 );
    
    // Sets PWM Period.  Must be calculated vs. input clk period.
    // For example, setting this to 20 will divide the input clock by 2^20, or 1 Million.
    // So a 50 MHz input clock will be divided by 1e6, thus this will have a period of 1/50
    always @(posedge Clk)
        if (!Reset)
            count <= 0;
        else
            count <= count + 1;
             
    wire [15:0] SW;
    reg [6:0] SEG;
    
    assign SW[15:0] = DutyCycle[15:0];
    
            
    always @(SW[3:0])
	   	begin
	       case(SW[3:0])
	           4'b0000 : SEG [6:0] = 8'b1000000;
	           4'b0001 : SEG [6:0] = 8'b1111001;
	           4'b0010 : SEG [6:0] = 8'b0100100;
	           4'b0011 : SEG [6:0] = 8'b0110000;
	           4'b0100 : SEG [6:0] = 8'b0011001;
	           4'b0101 : SEG [6:0] = 8'b0010010;
	           4'b0110 : SEG [6:0] = 8'b0000010;
               4'b0111 : SEG [6:0] = 8'b1111000;
	           4'b1000 : SEG [6:0] = 8'b0000000;
               4'b1001 : SEG [6:0] = 8'b0011000;
               4'b1010 : SEG [6:0] = 8'b0001000;
               4'b1011 : SEG [6:0] = 8'b0000011;
               4'b1100 : SEG [6:0] = 8'b1000110;
               4'b1101 : SEG [6:0] = 8'b0100001;
               4'b1110 : SEG [6:0] = 8'b0000110;
               4'b1111 : SEG [6:0] = 8'b0001110;
            endcase
        end
    assign Digits_Bits[0] = SEG[6];
    assign Digits_Bits[1] = SEG[5];
    assign Digits_Bits[2] = SEG[4];
    assign Digits_Bits[3] = SEG[3];
    assign Digits_Bits[4] = SEG[2];
    assign Digits_Bits[5] = SEG[1];
    assign Digits_Bits[6] = SEG[0];    
    always @(SW[15:13])
        begin
            AN [7:0] = 8'b11111111;
            case(SW [15:13])
                3'b000 : AN[0] = 0;
                3'b001 : AN[1] = 0;
                3'b010 : AN[2] = 0;
                3'b011 : AN[3] = 0;
                3'b100 : AN[4] = 0;
                3'b101 : AN[5] = 0;
                3'b110 : AN[6] = 0;
                3'b111 : AN[7] = 0;
            endcase
        end
        
    
    always @(posedge Clk)
        if (count < DutyCycle)
            PWM_out <= 2'b01; 
        else
            PWM_out <= 2'b10;
		   
	always @(posedge Clk)
        if (!Reset)
            Interrupt <= 0;
        else if (DutyCycle > 990000)
            Interrupt <= 1;
        else if (DutyCycle < 0)
            Interrupt <= 1;
        else
            Interrupt <= 0; 
        
endmodule
