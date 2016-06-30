player = {}

currentClass = "player"
extends = "actor"

player.speed_multiplier = 70
player.lower_bound = -150
player.is_jumping = false
player.jumping_speed = 350
player.gravity = -981
player.speed_y = 0
player.speed_x = 0
player.x_force = 1500
player.x_speed_limit = 120
player.x_low_speed_limit = 20
player.x_friction = 0.2

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
	
	self.local_x, self.local_y, self.local_z = getPosition(self.gameObject)
	
	if y > 0 and self.is_jumping == false then
		self.speed_y = self.jumping_speed
		self.is_jumping = true
	end
	
	if self.is_jumping then
		self.speed_y = self.speed_y + (self.gravity * deltaTime)
		self.local_y = self.local_y + self.speed_y * deltaTime
		if self.local_y < self.lower_bound then
			self.is_jumping = false
			self.local_y = self.lower_bound
		end
	end
	
	local speed_sign = self.speed_x
	speed_sign = speed_sign / math.abs(speed_sign)
	if self.speed_x == 0 then
		speed_sign = 0
	end
	local inverse_speed_sign = speed_sign * -1
	
	local force_x_modifier = 1
	if speed_sign + x == 0 then
		force_x_modifier = force_x_modifier * 3
	end
	
	self.speed_x = self.speed_x + self.x_force * force_x_modifier * x * deltaTime
	
	local friction_modifier = 1
	if speed_sign + x == 1 or speed_sign + x == -1 then
		friction_modifier = 12
	end
	
	self.speed_x = self.speed_x + self.speed_x * self.x_friction * friction_modifier * -1 * deltaTime;
	
	if math.abs(self.speed_x) > self.x_speed_limit then
		self.speed_x = self.x_speed_limit * speed_sign
	end
	
	if x == 0 and math.abs(self.speed_x) < self.x_low_speed_limit then
		self.speed_x = 0
	end
	
	self.local_x = self.local_x + self.speed_x * deltaTime
	

	setPosition(self.gameObject, self.local_x, self.local_y, self.local_z)
	
	-- This is sample calling of a parent class function
	self:madd(5, 3)
	
end