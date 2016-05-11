local levelHandler = {}

levelHandler.currentMap = 1
levelHandler.nrOfMaps = 0
levelHandler.mapNames = {}

local function GetMapNames()
	local f = io.open("list.map", "r")
	nr = 1
	for line in f:lines() do
		levelHandler.mapNames[nr] = line
		levelHandler.nrOfMaps = levelHandler.nrOfMaps + 1 
		--print(levelHandler.mapNames[nr])
		
		nr = nr + 1
	end
	io.close(f)		
end

levelHandler.GetMapNames = GetMapNames

return levelHandler