
A ={}

A[1] = {3,0,0,0,0,0,0,0}
A[2] = {0,0,0,0,0,0,0,0}
A[3] = {0,0,0,0,0,0,0,0}
A[4] = {0,0,0,0,0,0,0,0}
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

PLAYER = {}
PLAYER["x"] = 1
PLAYER["y"] = 1
PLAYER["color"] = 3

function Move(x, y)
	print("Move ", x, y)
	A[y][x] = PLAYER.color
	print(A[y][x])
end

function ResetTile (x, y)
	A[y][x] = 0
end

local switch = {}
switch["UP"] =		
function() 
	print("UP")
	if PLAYER.y > 1 then
		ResetTile(PLAYER.x, PLAYER.y)
		PLAYER.y = PLAYER.y - 1
		Move(PLAYER.x, PLAYER.y)
	end
end

switch["DOWN"] =	
function() 
	print("DOWN")
	if PLAYER.y < 8 then
		ResetTile(PLAYER.x, PLAYER.y)
		PLAYER.y = PLAYER.y + 1
		Move(PLAYER.x, PLAYER.y)
	end
end

switch["RIGHT"] =	
function() 
	print("RIGHT")
	if PLAYER.x < 8 then
		ResetTile(PLAYER.x, PLAYER.y)
		PLAYER.x = PLAYER.x + 1
		Move(PLAYER.x, PLAYER.y)
	end
end

switch["LEFT"] =
function() 
	print("LEFT")
	if PLAYER.x > 1 then
		ResetTile(PLAYER.x, PLAYER.y)
		PLAYER.x = PLAYER.x - 1
		Move(PLAYER.x, PLAYER.y)
	end
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
			x = x + 1
		end
		y = y + 1
	end
	io.close(f)
end

function HandleKeyPress(key)
	switch[key]()
end