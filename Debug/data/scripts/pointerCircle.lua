pointerCircle = {
innerCircleLatency = 0.6,
innerCircle = 0,
outerCircle = 0,
currentPhase = 1.3,
speed = 2.5
}
currentClass = "pointerCircle"
extends = ""

function pointerCircle:update(deltaTime)

	if (queryMouseDown('1')) then
		self.currentPhase = 0
		self.innerCircle = 0
		self.outerCircle = 0
		local x, y = queryMousePosition()
		setPosition(self.cobj.cptr, x, y, -5)
		
	end
	
	if (self.currentPhase <= 1 + self.innerCircleLatency) then
		self.currentPhase = self.currentPhase + deltaTime * self.speed
		self.outerCircle = self.outerCircle + deltaTime * self.speed
		self.innerCircle = self.currentPhase - self.innerCircleLatency
		if (self.outerCircle > 1) then
			self.outerCircle = 1
		end
		self.cobj.AddUniformFloat("outsideCircle", self.outerCircle)
		self.cobj.AddUniformFloat("insideCircle", self.innerCircle)
	end
	
	
end