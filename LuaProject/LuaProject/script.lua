--Recursive Fibonacci
function fib(nr)
	if nr > 2 then
		return fib(nr - 1) + fib(nr - 2)
	else
		 return 1
	end
end

--Returns table with fibonacci from 1 to nr
function getFibTable(nr)
	returnTable = {}
	for i = 1, nr do
		returnTable[i] = fib(i)
	end
	return returnTable
end

--[[
funcCalled = 0
--Function returns a string with todays date and how many time the function has been called
function dateAndNrOfCalls()
	date = os.date()
	returnStr = date .. " nrOfCalls: " .. funcCalled
	return returnStr
end
]]--

