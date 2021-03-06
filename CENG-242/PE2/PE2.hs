module PE2 where

---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------

-- Note: undefined is a value that causes an error when evaluated. Replace it with
-- a viable definition! Name your arguments as you like by changing the holes: _

--------------------------
-- Part I: Time to inf up!

-- naturals: The infinite list of natural numbers. That's it!
naturals :: [Integer]
naturals = iterate (+1) 0

-- interleave: Interleave two lists, cutting off on the shorter list.
interleave :: [a] -> [a] -> [a]
interleave xs ys = concat (zipWith (\x y -> [x]++[y]) xs ys)


-- integers: The infinite list of integers. Ordered as [0, -1, 1, -2, 2, -3, 3, -4, 4...].
integers :: [Integer]
integers = 0 : concat [[-x,x] | x <- [1..]]

--------------------------------
-- Part II: SJSON Prettification

-- splitOn: Split string on first occurence of character.

splitOn :: Char -> String -> (String, String)
splitOn _ str = (str,"")
splitOn del "" = ("","")
--splitOn del (x:xs) 
--    | del == x = ()

-- tokenizeS: Transform an SJSON string into a list of tokens.
tokenizeS :: String -> [String]
tokenizeS str = words str

-- prettifyS: Prettify SJSON, better tokenize first!
prettifyS :: String -> String
prettifyS _ = undefined

-- Good luck to you, friend and colleague!

