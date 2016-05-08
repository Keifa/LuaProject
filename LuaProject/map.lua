--PLAYER
PLAYER = {}
PLAYER["x"] = 1
PLAYER["y"] = 1
PLAYER["color"] = 3

--BOX
BOX = {}
BOX["x"] = 4
BOX["y"] = 4
BOX["color"] = 4

--WALL
WALLS = {}

--MAP/GRID
A ={}
A[1] = {3,0,0,0,0,0,0,0}
A[2] = {0,0,0,0,0,0,0,0}
A[3] = {0,0,0,0,0,0,0,0}
A[4] = {0,0,0,4,0,0,0,0}
A[5] = {0,0,0,0,0,0,0,0}
A[6] = {0,0,0,0,0,0,0,0}
A[7] = {0,1,1,1,1,1,0,0}
A[8] = {0,0,0,0,0,0,0,0}

B = {}
for x=1, 10 do
    B[x] = {}
    for y=1, 10 do
       	B[x][y] = 0
    end
end

function GetTile(x,y)
	return A[y][x]
end

function Clicked(x,y)
	local value = A[y][x]
	value = (value + 1) % 6
	A[y][x] = value
end

function AddWall(x, y)
	WALLS[#WALLS + 1] = Wall.New(x, y)
	A[y][x] = WALLS[#WALLS]:GetColor()
end

function CheckWalls(x, y)
	local check = false
	print(#WALLS)
	if #WALLS > 0 then
		for i = 1, #WALLS do
			--WALLS[i]:Print()
			if WALLS[i]:CheckCollision(x, y) then
				check = true
			end
		end
	end
	return check
end

function CheckMove(dir, pos)
	local check = false
	local posAfter = pos + dir
	if posAfter <= 8 and posAfter >= 1 then
		check = true
	end
	return check
end

function CheckForBox(x, y)
	local check = false
	if x <= 8 and x >= 1 and y <= 8 and y >= 1 and A[y][x] == BOX.color then
		check = true	
	end
	return check
end

function MoveBox(x, y)
	local xCheck = false
	local yCheck = false
	if CheckMove(x, BOX.x) then
		BOX.x = BOX.x + x
		xCheck = true
	end
	if CheckMove(y, BOX.y) then
		BOX.y = BOX.y + y
		yCheck = true
	end
	if xCheck or yCheck then
		A[BOX.y][BOX.x] = BOX.color
	end
	return xCheck, yCheck
end

function ResetTile (x, y)
	A[y][x] = 0
end

function MovePlayer(x, y)
	xCheck = false
	yCheck = false
	if CheckForBox(PLAYER.x + x, PLAYER.y + y) then 
		
		xCheck, yCheck = MoveBox(x, y)
		if xCheck then	
			if CheckMove(x, PLAYER.x) then
				PLAYER.x = PLAYER.x + x
			end
		end
		if yCheck then
			if CheckMove(y, PLAYER.y) then
				PLAYER.y = PLAYER.y + y
			end
		end

		if xCheck or yCheck then
			ResetTile(PLAYER.x - x, PLAYER.y - y)
			A[PLAYER.y][PLAYER.x] = PLAYER.color
		end
	else
		if CheckMove(x, PLAYER.x) then
			PLAYER.x = PLAYER.x + x
		end
		if CheckMove(y, PLAYER.y) then
			PLAYER.y = PLAYER.y + y
		end

		ResetTile(PLAYER.x - x, PLAYER.y - y)
		A[PLAYER.y][PLAYER.x] = PLAYER.color
	end
	if CheckWalls(PLAYER.x, PLAYER.y) then
		print("WALL")
	end
end

function SetPlayerPosition(x, y)
	ResetTile(PLAYER.x, PLAYER.y)
	PLAYER.x = x
	PLAYER.y = y
	A[PLAYER.y][PLAYER.x] = PLAYER.color
end

function SetBoxPosition(x, y)
	ResetTile(BOX.x, BOX.y)
	BOX.x = x
	BOX.y = y
	A[BOX.y][BOX.x] = BOX.color
end

function Save()
	local f = io.open("save.save", "w")
	for y=1, 8 do
    	local str = ""
   			for x=1, 8 do
       			str = str .. A[y][x]
				if x < 8 then
					str = str .. ","
				end
    		end
		str = str .. "\n"
    	f:write(str)
	end
	io.close(f)
end

function AddEntity(x, y)
	if A[y][x] == tostring(PLAYER.color) then
		PLAYER.x = x
		PLAYER.y = y
		SetPlayerPosition(x, y)
	elseif A[y][x] == tostring(BOX.color) then
		BOX.x = x
		BOX.y = y
		SetBoxPosition(x, y)
	elseif A[y][x] == tostring(1) then
		AddWall(x, y)
	end
end

function Load()
	local f = io.open("save.save", "r")
	y = 1
	for line in f:lines() do
		x = 1
		for l in string.gmatch(line, "([^".. "," .."]+)") do
			A[y][x] = l
			AddEntity(x, y)
			x = x + 1
		end
		y = y + 1
	end
	io.close(f)
end 

local switch = {}
switch["UP"] = function() MovePlayer(0, -1) end
switch["DOWN"] = function() MovePlayer(0, 1) end
switch["RIGHT"] = function() MovePlayer(1, 0) end
switch["LEFT"] = function() MovePlayer(-1, 0) end
switch["S"] = function() Save() end
switch["R"] = function() Load() end

--[[
function()
	local f = io.open("save.save", "w")
	for y=1, 8 do
    	local str = ""
   			for x=1, 8 do
       			str = str .. A[y][x]
				if x < 8 then
					str = str .. ","
				end
    		end
		str = str .. "\n"
    	f:write(str)
	end
	io.close(f)
end
--]]

--[[	
function()
	local f = io.open("save.save", "r")
	y = 1
	for line in f:lines() do
		x = 1
		for l in string.gmatch(line, "([^".. "," .."]+)") do
			A[y][x] = l
			if A[y][x] == tostring(PLAYER.color) then
				PLAYER.x = x
				PLAYER.y = y
				SetPlayerPosition(x, y)
			elseif A[y][x] == tostring(BOX.color) then
				BOX.x = x
				BOX.y = y
				SetBoxPosition(x, y)
			end
			x = x + 1
		end
		y = y + 1
	end
	io.close(f)
end
--]]

function HandleKeyPress(key)
	switch[key]()
end
