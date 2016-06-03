player = {}

currentClass = "player"

function player:update(deltaTime)
	if not self.local_x then
		self.local_x = 0
	end
	self.local_x = self.local_x + deltaTime
	print(self)
	print(self.local_x)
end