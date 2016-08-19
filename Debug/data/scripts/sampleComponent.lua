SampleComponent = {}

currentClass = "SampleComponent"
extends = ""

function SampleComponent:update(deltaTime)
	print (self.Area.Contains(55, 45, 0))
end