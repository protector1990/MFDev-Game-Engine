player2 = {}

currentClass = "player2"

function player2:update(deltaTime)
	if not self.local_x then
		self.local_x = 0
	end
	self.local_x = self.local_x + deltaTime + 5
	print(self)
	print(self.local_x)
end

--TODO: Proveriti zasto ova sintaksa nece da radi
--player2 = {
--	update = function(self, deltaTime)
--				if not self.local_x then
--					self.local_x = 0
--				end
--				self.local_x = self.local_x + deltaTime + 5
--				print(self)
--				print(self.local_x)
--			end 
--} 
