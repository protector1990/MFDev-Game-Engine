player = {}

currentClass = "player"

function player:update(deltaTime)
	local x = 0
	local y = 0
	
	if queryKeyDown(97) then
		x = x - 1
	end
	if queryKeyDown(100) then
		x = x + 1
	end
	if queryKeyDown(119) then
		y = y + 1
	end
	if queryKeyDown(115) then
		y = y - 1
	end
	
	x = x * deltaTime * 50
	y = y * deltaTime * 50
	
	translate(self.gameObject, x, y, 0)
end