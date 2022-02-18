module PE3 where

data Cell = SpaceCraft Int | Sand | Rock Int | Pit deriving (Eq, Read, Show)

type Grid = [[Cell]]
type Coordinate = (Int, Int)

data Move = North | East | South | West | PickUp | PutDown deriving (Eq, Read, Show)

data Robot = Robot { name :: String,
                     location :: Coordinate,
                     capacity :: Int,
                     energy :: Int,
                     storage :: Int } deriving (Read, Show)

-------------------------------------------------------------------------------------------
--------------------------------- DO NOT CHANGE ABOVE -------------------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------
-------------------------------------- PART I ---------------------------------------------

isInGrid :: Grid -> Coordinate -> Bool
isInGrid grid coor 
    | fst coor <= -1 = False
    | snd coor <= -1 = False
    | length grid -1 < snd coor = False
    | length (grid !! 0)-1 < fst coor = False
    | otherwise = True

-------------------------------------------------------------------------------------------
-- cell listesinin elemanlarını gez- rock sayısı bul
totalCountRocks :: Cell -> Int
totalCountRocks (Rock x) = x
totalCountRocks (SpaceCraft y) = 0
totalCountRocks (Sand) = 0
totalCountRocks (Pit) = 0

-- cell listesi alıyor
totalCountCell :: [Cell] -> Int
totalCountCell [] = 0
totalCountCell (x:xs) = totalCountRocks x + totalCountCell xs

totalCount :: Grid -> Int
totalCount [] = 0
totalCount (x:xs) = totalCountCell x + totalCount xs

-------------------------------------------------------------------------------------------

coordinatesOfPits :: Grid -> [Coordinate]
coordinatesOfPits grid = []
-------------------------------------------------------------------------------------------

tracePath :: Grid -> Robot -> [Move] -> [Coordinate]
tracePath grid robot moves = []

------------------------------------- PART II ----------------------------------------------

energiseRobots :: Grid -> [Robot] -> [Robot]
energiseRobots grid robots = robots

-------------------------------------------------------------------------------------------

applyMoves :: Grid -> Robot -> [Move] -> (Grid, Robot)
applyMoves grid robot moves = (grid, robot)
