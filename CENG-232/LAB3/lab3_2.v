// Code your design here
// Code your design here

`timescale 1ns / 1ps
module QueueCounter(
    input [7:0] llmt,
    input [7:0] ulmt,
    input en,
    input mode, // 1:delete, 0:insert
    input clk,
    output reg [7:0] head,
    output reg [7:0] tail,
    output reg empty,
    output reg full);
    
    //you may write your code here

    initial
    begin
        head = llmt;
        tail = llmt;
        empty = 1;
        full = 0;
    end

  always@(posedge clk)
    begin
      if (en == 1)
        begin
          if (mode == 0)
            begin
              if (empty == 1 & full != 1)
                begin
                  tail = tail + 1;
                  empty <= 0;
                end
              else if (tail < ulmt & full != 1)
                begin
                  tail = tail + 1;
                  if (tail == head)
                    begin
                      full <=1;
                      empty<=0;
                    end
                end
              else if (tail == ulmt & full != 1)
                begin
                  tail = head;
                  full <= 1;
                end
            end
          else if (mode == 1)
            begin
              if (empty != 1 & head < ulmt)
                begin
                  head = head + 1;
                  if (full == 1)
                    begin
                      full <= 0;
                    end
                  if (tail == head)
                    begin
                      empty <= 1;
                    end
                end
              else if (empty != 1 & head == ulmt)
                begin
                  head = llmt;
                  if (tail == head)
                    begin
                      empty <= 1;
                    end
                  if (full == 1)
                    begin
                      full <= 0;
                    end
                end
            end
        end
    end
              
endmodule


module Numerator(
    input clinic,
    input mode,
    input en,
    input clk,
    output reg [7:0] ann,
    output reg [7:0] print);

	  input en_c0, en_c1;
	  input queue_state;
  
  	assign en_c0 = !clinic;
  	assign en_c1 = clinic;
  	assign Empty = 1;
  	assign Full = 0;
  
    wire [7:0] c0_h;
  	wire [7:0] c0_t;
  	wire [7:0] c1_h;
 	  wire [7:0] c1_t;
  
  	QueueCounter c0(5, 9, en_c0, mode, clk, c0_h, c0_t, queue_state, queue_state);
  	QueueCounter c1(10, 14, en_c1, mode, clk, c1_h, c1_t, queue_state, queue_state);

  always@(posedge clk, en, en_c0, en_c1, c0_h, c0_t, c1_h, c1_t)
    begin
      if(en_c0 & en == 1)  
        begin  
       	  ann <= c0_h;
          print <= c0_t;
        end
      else if (en_c1 & en == 1)
        begin
          ann <= c1_h;
          print <= c1_t;
        end
    end
endmodule




