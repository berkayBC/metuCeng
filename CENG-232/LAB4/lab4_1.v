// Code your design here
`timescale 1ns / 1ps

module ROM (
input [2:0] addr, 
output reg [7:0] dataOut);

	// write your code below
  always@(addr)
    begin
      case (addr)
        0: dataOut = 8'd0;
        1: dataOut = 8'd85;
        2: dataOut = 8'd170;
        3: dataOut = 8'd51;  
      	4: dataOut = 8'd204; 
        5: dataOut = 8'd15; 
        6: dataOut = 8'd240; 
        7: dataOut = 8'd255; 
      endcase
    end
endmodule


module XOR_RAM (
input mode, 
input [2:0] addr, 
input [7:0] dataIn, 
input [7:0] mask, 
input CLK, 
output reg [7:0] dataOut);
	
	// write your code below
  reg [7:0] ram [0:7];
  // assign dataOut = 0; //if deneme
  
  initial
    begin
      ram[0]<=0;
      ram[1]<=0;
      ram[2]<=0;
      ram[3]<=0;
      ram[4]<=0;
      ram[5]<=0;
      ram[6]<=0;
      ram[7]<=0;
    end
  
  
  
  always@ (posedge CLK)
    begin
      if(mode == 0)
        ram[addr]<=(dataIn^mask);
    end 
   //if (mode)
    //assign dataOut = ram[addr]; // if deneme
  assign dataOut = mode?ram[addr]:dataOut; // ifsiz deneme
  
endmodule


module EncodedMemory (
input mode, 
input [2:0] index, 
input [7:0] number, 
input CLK, 
output [7:0] result);
	
	// DO NOT EDIT THIS MODULE
	
	wire [7:0] mask;
	
	ROM R(index, mask);
	XOR_RAM XR(mode, index, number, mask, CLK, result);

endmodule


