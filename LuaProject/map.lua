
local gameOver = false
local mapName = ""
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

--Selected
local select = nil

--Editing mode
local edit = false

--Player
local p = Entity.New("player.png")
local pID = 5
p:SetPosition(1,1)

function MovePlayer(xDir, yDir)
	p:Move(xDir, yDir)
end

--Goal
local g = Entity.New("button.png")
local gID = 7
g:SetPosition(mapSize/2, mapSize/2)

--Box
local b = Entity.New("box.png")
local bID = 6
b:SetPosition(3,3)

function MoveBox(xDir, yDir)
	map[b:GetY()][b:GetX()] = 0
	b:Move(xDir, yDir)
	if g:CollisionCheck(b:GetX(), b:GetY()) then
		gameOver = true
		print(gameOver)
	end
end

--Wall
local wID = 1

function GetTile(x,y)
	return map[y][x]
end

function Clicked(x,y)
	if HasSelected() then
		select:SetPosition(x,y)
		select = nil
	else
		local value = map[y][x]
		value = (value + 1) % 2
		map[y][x] = value
	end
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

switch["1"] =	function() 
	select = p
	print("Player selected")
end

switch["2"] =	function() 
	select = g
	print("Goal button selected")
end

switch["3"] =	function() 
	select = b
	print("Box selected")
end

switch["S"] =
function()
	print("Save")
	local f = io.open("save.save", "w")

	local string = ""
	--Player save
	string = "[player]," .. p:GetTexture() .. "," .. p:GetX() .. "," .. p:GetY() .. "\n"
	f:write(string)
	--Box save
	string = "[box]," .. b:GetTexture() .. "," .. b:GetX() .. "," .. b:GetY() .. "\n"
	f:write(string)
	--Button save
	string = "[button]," .. g:GetTexture() .. "," .. g:GetX() .. "," .. g:GetY() .. "\n"
	f:write(string)

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

function LoadMap(file)
	gameOver = false
	local f = io.open(file .. ".save", "r")
	y = 1
	for line in f:lines() do
		t = {}
		k = 1
		for v in string.gmatch(line, "([^".. "," .."]+)") do
			if string.len(v) == 1 then
				t[k] = tonumber(v)
			else
				t[k] = v
			end
			k = k + 1
		end

		if t[1] == "[player]" then
			p:SetTexture(t[2])
			p:SetPosition(t[3],t[4])
		elseif t[1] == "[box]" then
			b:SetTexture(t[2])
			b:SetPosition(t[3],t[4])
		elseif t[1] == "[button]" then
			g:SetTexture(t[2])
			g:SetPosition(t[3],t[4])
		else
			for i = 1, k - 1 do
				map[y][i] = t[i]
			end
			y = y + 1
		end
	end
	io.close(f)
	for y = 1, mapSize do
		print(map[y][1]..map[y][2]..map[y][3]..map[y][4]..map[y][5]..map[y][6]..map[y][7]..map[y][8]..map[y][9]..map[y][10])
	end
end

switch["R"] =		
function()
	print("Load")
	LoadMap(mapName)
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

function GetPlayer()
	return p:GetTexture(),p:GetX()-1,p:GetY()-1
end

function GetBox()
	return b:GetTexture(),b:GetX()-1,b:GetY()-1
end

function GetButton()
	return g:GetTexture(),g:GetX()-1,g:GetY()-1
end
function HasSelected()
	if select == nil then
		return false
	else
		return true
	end
end
function GetSelected()
	return g:GetTexture(),g:GetX(),g:GetY()
end

function isEditing()
	return edit
end

function GetGameOver()
	return gameOver
end

function SetMapName(name)
	mapName = name
end