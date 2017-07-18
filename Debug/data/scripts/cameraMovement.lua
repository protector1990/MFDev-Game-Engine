cameraMovement = {
h = 120,
v = 90,
n = 1,
f = 150
}

currentClass = "cameraMovement"
extends = ""

function cameraMovement:update(deltaTime)
-- print ("cameraMovementUpdate\n")
	if queryKeyDown(97) then
		self.h = self.h - 0.1
	end
	if queryKeyDown(100) then
		self.h = self.h + 0.1
	end
	if queryKeyDown(119) then
		self.v = self.v + 0.1
	end
	if queryKeyDown(115) then
		self.v = self.v - 0.1
	end
	self.cobj.SetPerspective(self.n, self.f, self.h, 0)
	-- print(self.n, self.f, self.h, self.v)
end