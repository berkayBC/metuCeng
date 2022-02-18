`timescale 1ns / 1ps
module kb(
    input K,
    input B,
    input clk,
    output reg Q
    );
    
    initial begin
        Q = 0;
    end
    
    always@(posedge clk)
      begin
        if(K == 0 & B == 0)
          Q <= ~Q;
        else if(K == 0 & B == 1)
          Q <= 0;
        else if(K == 1 & B == 0)
          Q <= 1;
        else
          Q <= Q;
         end
    
    // write your code here

endmodule


module ic232(
    input A0,
    input A1,
    input A2,
    input clk,
    output Q0,
    output Q1,
    output Z
    );
  
  kb kb_1 ((~A2 & (A0 ~| A1)), (A1), (clk), (Q0));
  kb kb_2 ((~A2), (A2 ^ (~A0 | A1)), (clk), (Q1));
  
  assign Z = (Q0 ~^ Q1);
 
  
   
    // write your code here
    
endmodule



