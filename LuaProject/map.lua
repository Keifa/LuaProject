--PLAYER
PLAYER = {}
PLAYER["x"] = 1
PLAYER["y"] = 1
PLAYER["color"] = 2

--BOX
BOX = {}
BOX["x"] = 4
BOX["y"] = 4
BOX["color"] = 3

--WALL
WALL = {}
WALL["x"] = 0
WALL["y"] = 0
WALL["color"] = 2

WALLS = {}

--MAP/GRID
A ={}
A[1] = {2,0,0,0,0,0,0,0}
A[2] = {0,0,0,0,0,0,0,0}
A[3] = {0,0,0,0,0,0,0,0}
A[4] = {0,0,0,3,0,0,0,0}
A[5] = {0,0,0,0,0,0,0,0}
A[6] = {0,0,0,0,0,0,0,0}
A[7] = {0,0,0,0,0,0,0,0}
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

function Save()
	print("Save")
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

function Load()
	print("Load")
	local f = io.open("save.save", "r")
	y = 1
	for line in f:lines() do
		x = 1
		for l in string.gmatch(line, "([^".. "," .."]+)") do
			A[y][x] = l
			x = x + 1
		end
		y = y + 1
	end
	io.close(f)
end

function Clicked(x,y)
	local value = A[y][x]
	value = (value + 1) % 4
	A[y][x] = value
end

function CheckForBox(x, y)
	local check = false
	if A[y][x] == BOX.color then
		check = true		
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
end

local switch = {}
switch["UP"] =		
function() 
	MovePlayer(0, -1)
end

switch["DOWN"] =	
function() 
	MovePlayer(0, 1)
end

switch["RIGHT"] =	
function() 
	MovePlayer(1, 0)
end

switch["LEFT"] =
function() 
	MovePlayer(-1, 0)
end

switch["S"] =
function()
	print("Save")
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

switch["R"] =		
function()
	print("Load")
	local f = io.open("save.save", "r")
	y = 1
	for line in f:lines() do
		x = 1
		for l in string.gmatch(line, "([^".. "," .."]+)") do
			A[y][x] = l
			if A[y][x] == tostring(PLAYER.color) then
				PLAYER.x = x
				PLAYER.y = y
				MovePlayer(x, y)
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