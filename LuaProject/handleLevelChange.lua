
-- Access map.lua
require("map")

-- File name of the file with all the level names
local levelsFileName = "load"
-- Table for level names
local levels = {}
local currentLevel = 1

-- Loads all the level file names into a table and loads the first level
function GetLevelNames(file)
	local f = io.open(file .. ".levels", "r")
	local index = 1
	for line in f:lines() do
		levels[index] = line
		print(levels[index])
		index = index + 1
	end
	LoadMap(levels[1])
end

-- Use change level and if there are no more new levels it switches to the first level
function ChangeLevel()
	currentLevel = currentLevel + 1
	if levels[currentLevel] == nil then
		currentLevel = 1
	end
	LoadMap(levels[currentLevel])
	SetMapName(levels[currentLevel])
end

function CheckLevelDone()
	if GetGameOver() then
		ChangeLevel()
	end
end

GetLevelNames(levelsFileName)