module PE1 where

import Text.Printf


-- This function takes a Double and rounds it to 2 decimal places as requested in the PE --
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

convertTL :: Double -> String -> Double
convertTL money curr = if curr == "USD" then getRounded (money / 8.18)
                        else if curr == "EUR" then getRounded (money / 9.62)
                        else if curr == "BTC" then getRounded (money / 473497.31)
                        else money
-------------------------------------------------------------------------------------------

countOnWatch :: [String] -> String -> Int -> Int
countOnWatch [] employee days = 0
countOnWatch schedule employee 0 = 0
countOnWatch (x:xs) employee days = if x == employee then countOnWatch (xs) employee (days-1) + 1
                                        else countOnWatch (xs) employee (days-1)

-------------------------------------------------------------------------------------------

lastDigit :: Int -> Int
lastDigit a = if a >= 10 then lastDigit (a - 10)
                else a

-------------------------------------------------------------------------------------------

encryptDigit :: Int -> Int
encryptDigit x = if x `mod` 3 == 0 then (x - 1)
            else if x `mod` 4 == 0 then lastDigit (2 * x)
            else if x `mod` 5 == 0 then (x + 3)
            else lastDigit (x + 4)
            
-------------------------------------------------------------------------------------------

encrypt :: Int -> Int
encrypt x = (encryptDigit (x `div` 1000)) * 1000 + (encryptDigit (lastDigit (x `div` 100))) * 100 + (encryptDigit (lastDigit (x `div` 10))) * 10 + (encryptDigit (lastDigit x))
    

-------------------------------------------------------------------------------------------

interest :: (Double, Int) -> Double
interest (money, year) = if money >= 10000.0 && year >= 2 then money * (1 + 0.115 / 12)^^(12 * year)
                            else if money >= 10000.0 && year < 2 then money * (1 + 0.105 / 12)^^(12 * year)
                            else if money < 10000.0 && year >= 2 then money * (1 + 0.095 / 12)^^(12 * year)
                            else money * (1 + 0.090 / 12)^^(12 * year)

-------------------------------------------------------------------------------------------

compoundInterests :: [(Double, Int)] -> [Double]
compoundInterests [] = []
compoundInterests (x:xs) = [getRounded (interest (x))] ++ compoundInterests (xs)