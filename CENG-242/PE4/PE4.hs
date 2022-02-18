module PE4 where

import Data.Maybe -- up to you if you want to use it or not

-- Generic DictTree definition with two type arguments
data DictTree k v = Node [(k, DictTree k v)] | Leaf v deriving Show

-- Lightweight Char wrapper as a 'safe' Digit type
newtype Digit = Digit Char deriving (Show, Eq, Ord) -- derive equality and comparison too!

-- Type aliases
type DigitTree = DictTree Digit String
type PhoneNumber = [Digit]


---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------


----------
-- Part I:
-- Some Maybe fun! Basic practice with an existing custom datatype.

-- toDigit: Safely convert a character to a digit
toDigit :: Char -> Maybe Digit
toDigit x 
    | (x >= '0') && (x <= '9') = Just (Digit x)
    | otherwise = Nothing

-- toDigits: Safely convert a bunch of characters to a list of digits.
--           Particularly, an empty string should fail.

toDigitsHelper :: String -> Maybe PhoneNumber
toDigitsHelper [] = Just []
toDigitsHelper (x:xs)
    | (x >= '0') && (x <= '9') = Just ([Digit x] ++ fromJust(toDigitsHelper xs))
    | otherwise = Nothing

toDigits :: String -> Maybe PhoneNumber
toDigits [] = Nothing
toDigits x = toDigitsHelper x


-----------
-- Part II:
-- Some phonebook business.
-------------------------------------------------------------------------------------
-- key, node al; leaf check yap
numContactsHelper2 :: DigitTree -> Int
numContactsHelper2 (Leaf a) = 1
numContactsHelper2 (Node b) = numContactsHelper1 b

-- key, node pairlerini gez
numContactsHelper1 :: [(Digit, DigitTree)] -> Int
numContactsHelper1 [] = 0
numContactsHelper1 ((key, tree):xs) = numContactsHelper1 xs + numContactsHelper2 tree

-- numContacts: Count the number of contacts in the phonebook...
numContacts :: DigitTree -> Int
numContacts (Leaf a) = 1
numContacts (Node x) = numContactsHelper1 x
-------------------------------------------------------------------------------------

--key,node pair al leaf bul
getContactsHelper2 :: DigitTree -> String
getContactsHelper2 (Leaf a) = a
getContactsHelper2 (Node ((key, tree):xs)) = getContactsHelper2 tree

-- key-node pairi al keyleri tut
getContactsHelper1 :: DigitTree -> PhoneNumber -> PhoneNumber
getContactsHelper1 (Leaf a) res = res
getContactsHelper1 (Node ((key, tree):xs)) res = getContactsHelper1 (Node xs) ([key] ++ getContactsHelper1 tree res)
getContactsHelper1 _ res = res

    
-- getContacts: Generate the contacts and their phone numbers in order given a tree. 
getContacts :: DigitTree -> [(PhoneNumber, String)]
getContacts (Node ((key, tree):xs)) = [([key] ++ getContactsHelper1 tree [], getContactsHelper2 tree)] ++ getContacts (Node xs)
getContacts _ = []--[([Digit '3'], "abc")]
-------------------------------------------------------------------------------------


-- autocomplete: Create an autocomplete list of contacts given a prefix
-- e.g. autocomplete "32" areaCodes -> 
--      [([Digit '2'], "Adana"), ([Digit '6'], "Hatay"), ([Digit '8'], "Osmaniye")]
autocomplete :: String -> DigitTree -> [(PhoneNumber, String)]
autocomplete _ _ = []


-----------
-- Example Trees
-- Two example trees to play around with, including THE exampleTree from the text. 
-- Feel free to delete these or change their names or whatever!

exampleTree :: DigitTree
exampleTree = Node [
    (Digit '1', Node [
        (Digit '3', Node [
            (Digit '7', Node [
                (Digit '8', Leaf "Jones")])]),
        (Digit '5', Leaf "Steele"),
        (Digit '9', Node [
            (Digit '1', Leaf "Marlow"),
            (Digit '2', Node [
                (Digit '3', Leaf "Stewart")])])]),
    (Digit '3', Leaf "Church"),
    (Digit '7', Node [
        (Digit '2', Leaf "Curry"),
        (Digit '7', Leaf "Hughes")])]

areaCodes :: DigitTree
areaCodes = Node [
    (Digit '3', Node [
        (Digit '1', Node [
            (Digit '2', Leaf "Ankara")]),
        (Digit '2', Node [
            (Digit '2', Leaf "Adana"),
            (Digit '6', Leaf "Hatay"),
            (Digit '8', Leaf "Osmaniye")])]),
    (Digit '4', Node [
        (Digit '6', Node [
            (Digit '6', Leaf "Artvin")])])]

