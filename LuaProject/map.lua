
local gameOver = false
local entitys = {}

--Map
mapSize = 10
local map = {}
for x=1, mapSize do
    map[x] = {}
    for y=1, mapSize do
       	map[x][y] = 0
    end
end

--Player
local p = Entity.New("player.png")
local pID = 5
p:SetPosition(1,1)
map[p:GetY()][p:GetX()] = pID

function MovePlayer(xDir, yDir)
	map[p:GetY()][p:GetX()] = 0
	p:Move(xDir, yDir)
	map[p:GetY()][p:GetX()] = pID
end

--Goal
local g = Entity.New("button.png")
local gID = 7
g:SetPosition(mapSize, mapSize)
map[g:GetY()][g:GetX()] = bID

--Box
local b = Entity.New("box.png")
local bID = 6
b:SetPosition(5,5)
map[b:GetY()][b:GetX()] = bID

function MoveBox(xDir, yDir)
	map[b:GetY()][b:GetX()] = 0
	b:Move(xDir, yDir)
	if g:CollisionCheck(b:GetX(), b:GetY()) then
		gameOver = true
		print(gameOver)
	end
	map[b:GetY()][b:GetX()] = bID
end

--Wall
local wID = 1

function GetTile(x,y)
	return map[y][x]
end

function Clicked(x,y)
	local value = map[y][x]
	value = (value + 1) % 4
	map[y][x] = value
end

function ValidMove(dir, currentPos)
	local posAfter = currentPos + dir
	if posAfter <= 0 or posAfter > mapSize then
		do return false end
	end
	return true
end

function MoveY(dir)
	-- if player pos is inside map and if the tile is walkable
	if ValidMove(dir, p:GetY()) and GetTile(p:GetX(), p:GetY() + dir) ~= wID then
		-- Check if collision with box
		if b:CollisionCheck(p:GetX(), p:GetY() + dir) then
			--  Check if box pos is inside map and if the tile is walkable
			if ValidMove(dir, b:GetY()) and GetTile(b:GetX(), b:GetY() + dir) ~= wID then
				MoveBox(0, dir)
				MovePlayer(0, dir)
			end
		-- if no collision with box -> move
		else
			MovePlayer(0, dir)
		end
	end
end

function MoveX(dir)
	--Check if pos is inside map and if the tile is walkable
	if ValidMove(dir, p:GetX()) and GetTile(p:GetX() + dir, p:GetY()) ~= wID then
		-- Check if collision with box
		if b:CollisionCheck(p:GetX() + dir, p:GetY()) then
			--  Check if box pos is inside map and if the tile is walkable
			if ValidMove(dir, b:GetX()) and GetTile(b:GetX() + dir, b:GetY()) ~= wID then
				MoveBox(dir, 0)
				MovePlayer(dir, 0)
			end
		-- if no collision with box -> move
		else
			MovePlayer(dir, 0)
		end
	end
end

local switch = {}
switch["UP"] =		function() 
	MoveY(-1)
end

switch["DOWN"] =	function() 
	MoveY(1)
end

switch["RIGHT"] =	function() 
	MoveX(1)
end

switch["LEFT"] =	function() 
	MoveX(-1)
end

function Save(fileName)
	map[g:GetY()][g:GetX()] = gID
	local f = io.open(fileName .. ".map", "w")
	for y=1, mapSize do
    	local str = ""
   			for x=1, mapSize do
       			str = str .. map[y][x]
				if x < mapSize then
					str = str .. ","
				end
    		end
		str = str .. "\n"
    	f:write(str)
	end
	io.close(f)
end

switch["S"] =
function()
	print("Save")
	Save("save")
end

function LoadMap(fileName)
	local file = fileName .. ".map"
	local f = io.open(file, "r")
	y = 1
	for line in f:lines() do
		x = 1
		for l in string.gmatch(line, "([^".. "," .."]+)") do			
			map[y][x] = tonumber(l)
			if map[y][x] == pID then
				p:SetPosition(x, y)
				print("Player: " .. p:GetX() .. " ".. p:GetY())
			elseif map[y][x] == bID then
				b:SetPosition(x, y)
				print("Box: " .. b:GetX() .. " ".. b:GetY())
			elseif map[y][x] == gID then
				g:SetPosition(x, y)
				print("Goal: " .. b:GetX() .. " ".. b:GetY())
			end
			x = x + 1
		end
		y = y + 1
	end
	io.close(f)	
end


switch["R"] =		
function()
	print("Load")
	LoadMap("level1")
end

function HandleKeyPress(key)
	switch[key]()
end

function GetPlayerX()
	return p:GetX()
end

function GetPlayerY()
	return p:GetY()
end

function GetBoxX()
	return b:GetX()
end

function GetBoxY()
	return b:GetY()
end