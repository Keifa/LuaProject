print("[Lua] Loaded Editor")

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

local switch = {}

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
	print("Saving")

	print("Input Save Name: ")
	local file = io.read()

	local f = io.open(file .. ".save", "w")

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
	print("Finished Saving")
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
end

switch["R"] =		
function()
	print("Reload")
	print("Input Save Name: ")
	local file = io.read()
	LoadMap(file)
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