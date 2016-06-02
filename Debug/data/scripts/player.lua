player = {}

currentClass = "player"

function player:update(self, deltaTime)
	self.local_x = self.local_x + deltaTime
	mprint(self.local_x)
end