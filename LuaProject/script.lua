local adam = Player.New("adam")

local wallTest = Wall.New(1,1)
wallTest:Print()
wallTest:SetPosition(2, 2)
wallTest:Print()
print(wallTest:GetColor())

adam:SetPosition(100,50)

adam:Print()

print( "[Lua] Jump returned: " .. adam:Jump( 4 ) )

print( "[Lua] GetX returned: " .. adam:GetX() )

print( "[Lua] GetY returned: " .. adam:GetY() )

function getX()
	return adam:GetX()
end

function getY()
	return adam:GetY()
end

function update(dt)
	
	adam:SetPosition(300 + 100 * math.sin(math.rad(dt)),50)
end

function movePlayerX( input )
	adam:SetPosition( adam:GetX() + input ,adam:GetY())
end

function movePlayerY( input )
	adam:SetPosition(adam:GetX() ,adam:GetY() + input)
end