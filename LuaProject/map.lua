
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
p:SetPosition(1,1)

function GetTile(x,y)
	return map[y][x]
end

--[[
function Save()
	print("Save")
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

function Load()
	print("Load")
	local f = io.open("save.save", "r")
	y = 1
	for line in f:lines() do
		x = 1
		for l in string.gmatch(line, "([^".. "," .."]+)") do
			map[y][x] = l
			x = x + 1
		end
		y = y + 1
	end
	io.close(f)
end
--]]

function Clicked(x,y)
	local value = map[y][x]
	value = (value + 1) % 4
	map[y][x] = value
end

local switch = {}
switch["UP"] =		function() 
	if p:GetY() > 0 and GetTile(p:GetX()+1,(p:GetY())) == 0 then
		p:Move(0,-1)
	end
end
switch["DOWN"] =	function() 
	if p:GetY() < mapSize and GetTile(p:GetX()+1,(p:GetY() + 2)) == 0 then
		p:Move(0,1)
	end
end
switch["RIGHT"] =	function() 
	if p:GetX() < mapSize and GetTile((p:GetX() + 2), p:GetY()+1) == 0 then
		p:Move(1,0)
	end
end
switch["LEFT"] =	function() 
	if p:GetX() > 0 and GetTile((p:GetX()),p:GetY()+1) == 0 then
		p:Move(-1,0)
	end
end
switch["S"] =
function()
	print("Save")
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
			map[y][x] = l
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