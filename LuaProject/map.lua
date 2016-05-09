
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

--Box
local b = Entity.New("box.png")
local bID = 6
b:SetPosition(5,5)

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

local switch = {}
switch["UP"] =		function() 
	if ValidMove(-1, p:GetY()) and GetTile(p:GetX(), p:GetY() - 1) == 0 then
		p:Move(0,-1)
		if p:CollisionCheck(b:GetX(), b:GetY()) then
			b:Move(0, -1)
		 end
	end
end

switch["DOWN"] =	function() 
	if ValidMove(1, p:GetY()) and GetTile(p:GetX(),(p:GetY() + 1)) == 0 then
		p:Move(0,1)
		if p:CollisionCheck(b:GetX(), b:GetY()) then
			b:Move(0, 1)
		 end
	end
end

switch["RIGHT"] =	function() 
	if ValidMove(1, p:GetX()) and GetTile((p:GetX() + 1), p:GetY()) == 0 then
		p:Move(1,0)
		if p:CollisionCheck(b:GetX(), b:GetY()) then
			b:Move(1, 0)
		 end
	end
end

switch["LEFT"] =	function() 
	if ValidMove(-1, p:GetX()) and GetTile((p:GetX() - 1),p:GetY()) == 0 then
		p:Move(-1,0)
		if p:CollisionCheck(b:GetX(), b:GetY()) then
			b:Move(-1, 0)
		 end
	end 
end
switch["S"] =
function()
	print("Save")
	map[p:GetY()][p:GetX()] = pID
	map[b:GetY()][b:GetX()] = bID
	local f = io.open("save.save", "w")
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

switch["R"] =		
function()
	print("Load")
	local f = io.open("save.save", "r")
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
			end
			x = x + 1
		end
		y = y + 1
	end
	io.close(f)
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