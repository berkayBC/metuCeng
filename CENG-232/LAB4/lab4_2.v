// Code your design here
// Code your design here
`timescale 1ns / 1ps
module IntersectionSimulator(
	input [2:0] mode, //1xx:display, 000:remA, 001:remB, 010:addA, 011:addB
	input [4:0] plateIn,
	input action,
	input clk,
	output reg  greenForA,
	output reg  greenForB,
	output reg 	rushHourWarning,
	output reg [3:0]  hour,
	output reg [5:0]  minute,	
	output reg [5:0]  second,
	output reg	am_pm, //0:am, 1:pm
	output reg [4:0]  numOfCarsA,
	output reg [4:0]  numOfCarsB,
	output reg [6:0]  remainingTime,
	output reg [4:0]  blackListDisplay
	);
	
	//You may write your code anywhere
  
  	reg [4:0] BlackList [7:0];
  	reg [3:0] bList_Disp_index;
  	reg LastGreenLight;
  	reg [6:0]  remainingTime_A;
  	reg [6:0]  remainingTime_B;
  	integer i, A_add_index, B_add_index, A_remove_index, B_remove_index, bList_add_index;
  	reg [4:0] roadA [7:0];
  	reg [4:0] roadB [7:0];
  	reg [4:0] plate;
	
	initial begin
		greenForA=1;
		greenForB=0;
		rushHourWarning=0;
		hour=6;
		minute=0;
		second=0;
		am_pm=0;
		numOfCarsA=0;
		numOfCarsB=0;
		remainingTime=50;
		blackListDisplay=0;
		
		//...
    	LastGreenLight = 0; // 0: A - 1: B
    	
      	remainingTime_A = 40;
    	remainingTime_B = 50;
      	
      	A_add_index = 0;
        B_add_index = 0;
      	A_remove_index = 0;
      	B_remove_index = 0;
      	
      	bList_add_index = 0;
        bList_Disp_index = 0;
      for (i=0; i<512; i=i+1) 
          BlackList[i] <= 0;
        i = 0;
	end


  always@(posedge action) // ADD-REMOVE OPERATIONS
	begin
      if (mode == 3'b000) // remove from A
        begin
          if(numOfCarsA == 0)
            begin
            	numOfCarsA = 0;
            end
          else
            begin
              plate = roadA[A_remove_index];
              A_remove_index = A_remove_index + 1;
              numOfCarsA = numOfCarsA - 1;
            end
          if(greenForA != 1)
                begin
                  BlackList[bList_add_index] = plate;
                  bList_add_index = bList_add_index + 1;
                end
          
        end
      if (mode == 3'b001) // remove from B
        begin
          if(greenForB != 1)
            begin
                BlackList[bList_add_index] = roadB[B_remove_index];
                bList_add_index = bList_add_index + 1;
            end
          
          if(numOfCarsB == 0)
            begin
            	numOfCarsB = 0;
            end
          else
            begin
              B_remove_index = B_remove_index + 1;
              numOfCarsB = numOfCarsB - 1;
            end
          
          
        end
      if (mode == 3'b010) // add to A
        begin
          if(numOfCarsA + numOfCarsB == 30) // intersection check
          begin
            numOfCarsA = numOfCarsA;
          end
          else 
            begin
              numOfCarsA = numOfCarsA + 1;
              roadA[A_add_index] = plateIn;
              A_add_index = A_add_index + 1;
            end
        end
      if (mode == 3'b011) // add to B
        begin
          if(numOfCarsA + numOfCarsB == 30) // intersection check
          begin
            numOfCarsB = numOfCarsB;
          end
          else 
            begin
              numOfCarsB = numOfCarsB + 1;
              roadB[B_add_index] = plateIn;
              B_add_index = B_add_index + 1;
            end
        end
	end


  always @(posedge clk) // BLACKLIST DISPLAY - TIME OPERATIONS
	begin
      if(mode === 3'b1xx) // blacklist display
		begin
        	blackListDisplay = BlackList[bList_Disp_index];
        	bList_Disp_index = bList_Disp_index + 1;
            if (BlackList[bList_Disp_index] === 0)
                bList_Disp_index = 0;
          end
      else // am_pm SET - RUSH HOUR CHECK - MIDNIGHT RESET - GREEN/RED SET
        begin
          // am_pm SET
        	second = second + 1;
            if (second == 60)
              begin
                minute = minute + 1;
                second = 0;
                if (minute == 60)
                  begin
                    hour = hour + 1;
                    minute = 0;
                    if (hour == 13) 
                      begin
                        hour = 1;
                      end
                    else if (hour == 12 & am_pm == 0) // 11.59 AM -> 12.00 PM
                      begin
                        am_pm = 1;
                      end
                    else if (hour == 12 & am_pm == 1)
                      begin
                        am_pm = 0;
                      end
                  end
              end
          
          // RUSH HOUR CHECK
          if((hour == 7 | hour == 8) && am_pm == 0) // 7:00:00-8:59:59 AM
              begin
                rushHourWarning = 1;
              end
          else if((hour == 5 | hour == 6) && am_pm == 1) // 5:00:00-6:59:59 PM
              begin
                rushHourWarning = 1;
              end
            else
              begin
                rushHourWarning = 0;
              end
          
          // MIDNIGHT RESET
          if(hour == 12 && am_pm == 0) // 12:00:00 AM
              begin
                blackListDisplay = 0;
        		    bList_Disp_index = bList_Disp_index + bList_add_index;
                bList_add_index = 0;
              end
          
          // GREEN/RED SET
          if(remainingTime == 1 & greenForA == 1) // table from 13
              begin
                LastGreenLight = 0;
                greenForA = 0;
                remainingTime = remainingTime - 1;
              end

          else if(remainingTime == 1 & greenForB == 1)
              begin
                LastGreenLight = 1;
                greenForB = 0;
                remainingTime = remainingTime - 1;
              end 
          
          else if(LastGreenLight == 1 & remainingTime == 0) // A turns GREEN
              begin
                greenForA = 1;
                greenForB = 0;
                
                if(0 <= numOfCarsA <= 10)  
                  begin
                  	remainingTime_A = remainingTime_A + 5;
                  
                  	if(rushHourWarning == 1 && remainingTime_A > 60)
                    	begin
                      		remainingTime_A = 60;
                    	end
                    else if(rushHourWarning == 1 && remainingTime_A < 30)
                    	begin
                      		remainingTime_A = 30;
                    	end
                  	else if(rushHourWarning == 0 && remainingTime_A > 70)
  						begin
                      		remainingTime_A = 70; 
                    	end
                    else if(rushHourWarning == 0 && remainingTime_A < 40)
  						begin
                      		remainingTime_A = 40; 
                    	end
                  end
  
                else if(11 <= numOfCarsA && numOfCarsA <= 19)
                  begin
                    remainingTime_A = remainingTime_A;
                  end
                  
                else
                  begin
                    remainingTime_A = remainingTime_A - 5;
                    
                    if(rushHourWarning == 1 && remainingTime_A > 60)
                    	begin
                      		remainingTime_A = 60;
                    	end
                    else if(rushHourWarning == 1 && remainingTime_A < 30)
                    	begin
                      		remainingTime_A = 30;
                    	end
                  	else if(rushHourWarning == 0 && remainingTime_A > 70)
  						begin
                      		remainingTime_A = 70; 
                    	end
                    else if(rushHourWarning == 0 && remainingTime_A < 40)
  						begin
                      		remainingTime_A = 40; 
                    	end
                  end       
              		
                remainingTime = remainingTime_B; // B turns RED
              end
          
          else if(LastGreenLight == 0 & remainingTime == 0) // B turns GREEN
              begin
                greenForA = 0;
                greenForB = 1;
                
                if(0 <= numOfCarsB <= 10)  
                  begin
                  	remainingTime_B = remainingTime_B + 5;
                  
                    if(rushHourWarning == 1 && remainingTime_B > 70)
                    	begin
                      		remainingTime_B = 70;
                    	end
                    else if(rushHourWarning == 1 && remainingTime_B < 40)
                    	begin
                      		remainingTime_B = 40;
                    	end
                    else if(rushHourWarning == 0 && remainingTime_B > 80)
  						begin
                      		remainingTime_B = 80; 
                    	end
                    else if(rushHourWarning == 0 && remainingTime_B < 50)
  						begin
                      		remainingTime_B = 50; 
                    	end
                  end
  
                else if(11 <= numOfCarsB && numOfCarsB <= 19)
                  begin
                    remainingTime_B = remainingTime_B;
                  end
                  
                else
                  begin
                    remainingTime_B = remainingTime_B - 5;
                    
                    if(rushHourWarning == 1 && remainingTime_B > 70)
                    	begin
                      		remainingTime_B = 70;
                    	end
                    else if(rushHourWarning == 1 && remainingTime_B < 40)
                    	begin
                      		remainingTime_B = 40;
                    	end
                    else if(rushHourWarning == 0 && remainingTime_B > 80)
  						begin
                      		remainingTime_B = 80; 
                    	end
                    else if(rushHourWarning == 0 && remainingTime_B < 50)
  						begin
                      		remainingTime_B = 50; 
                    	end
                  end       
              		
                remainingTime = remainingTime_A; // A turns RED
              end
          else // NO CHANGE ON LIGHTS
              begin
                remainingTime = remainingTime - 1;
              end
        end
	end


endmodule


