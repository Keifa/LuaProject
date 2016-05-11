
local test = require("levelHandler.lua")

function test()
	test.GetMapNames()
	print(test.nrOfMaps)
	for i = 1, test.nrOfMaps do
		print(test.mapNames[i])
	end
end