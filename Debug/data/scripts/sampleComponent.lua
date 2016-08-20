SampleComponent = {}

currentClass = "SampleComponent"
extends = ""

function SampleComponent:update(deltaTime)
	print (self.Area.Contains(55, 45, 0))
end

function SampleComponent:draw(deltaTime)
	draw(55, 45, 0, 1)
end