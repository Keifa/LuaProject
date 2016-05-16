
require("map")

local levelsFileName = "load"
local levels = {}
local currentLevel = 1

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