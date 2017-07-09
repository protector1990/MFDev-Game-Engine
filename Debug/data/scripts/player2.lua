player2 = {
nestonesto=5, 
nestodrugo=3
}
currentClass = "player2"
extends = ""

function player2:update(deltaTime)
	local x = 0
	local y = 0
	if queryKeyDown(1073741905) then
		x = x - 1
	end
	if queryKeyDown(1073741906) then
		y = y + 1
	end

	
	if x ~= 0 then
		translate(self.cobj.cptr, x * 0.1, 0, 0)
	end
	
	if y ~= 0 then
		rotate(self.cobj.cptr, 0, 0, y * 0.01)
	end
	--print(nestonesto)
	self.nestonesto = self.nestonesto + 2;
	--print("start")
	--for k, v in pairs(self) do
	--	print(k, v)
	--end
	--print ("end")
end