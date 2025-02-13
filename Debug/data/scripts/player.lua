player = {
speed_multiplier = 70,
lower_bound = -150,
is_jumping = false,
jumping_speed = 350,
gravity = -981,
speed_y = 0,
speed_x = 0,
x_force = 1500,
x_speed_limit = 120,
x_low_speed_limit = 20,
x_friction = 0.2,
zz = true,
firstTimeTest = false,
elapsed_time = 0
}

currentClass = "player"
extends = "actor"

function player:update(deltaTime)
	--if(self.zz) then
		--SomeTable()
	--end
	--self.cobj.SampleSpriteFun()
	self.zz = false
	local x = 0
	local y = 0
	local angle = 0
	local scaleFactor = 1
	
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
	
	if queryKeyDown(122) then
		scaleFactor = 0.995
	end
	
	if queryKeyDown(120) then
		scaleFactor = 1.005
	end

	if queryKeyDown(1073741903) then
		angle = angle - 1
	end
	
	if queryKeyDown(1073741904) then
		angle = angle + 1
	end
	
	if x ~= 0 or y ~= 0 then
		translate(self.cobj.cptr, x * 0.1, y * 0.1, 0)
	end
	
	if angle ~= 0 then
		--rotateAround(self.cobj.cptr, 0, 0, 1, angle * 0.001)
		rotate(self.cobj.cptr, 0, 0, angle * 0.001)
	end
	
	if scaleFactor ~= 1 then
		scale(self.cobj.cptr, scaleFactor, 1, 1)
	end
	
	if self.firstTimeTest then
		translate(self.cobj.cptr, 20, 20, 0)
		rotate(self.cobj.cptr, 0, 0, 45.0 * math.pi / 180)
		scale(self.cobj.cptr, 2, 1, 1)
		--scale(self.cobj.cptr, 0.7, 1.3, 1)
		--a, b, c = worldToLocalCoordinates(self.cobj.cptr, 22, 22, 0)
		-- a, b, c = localToWorldCoordinates(self.cobj.cptr, 1.41421356237309505, 0, 0)
		-- print (a)
		-- print (b)
		-- print (c)
		a, b, c = getLocalScale(self.cobj.cptr)
		print (a)
		print (b)
		print (c)
		self.firstTimeTest = false
	end
	
	-- self.local_x, self.local_y, self.local_z = getPosition(self.cobj.cptr)
	-- if y > 0 and self.is_jumping == false then
		-- self.speed_y = self.jumping_speed
		-- self.is_jumping = true
	-- end
	
	-- if self.is_jumping then
		-- self.speed_y = self.speed_y + (self.gravity * deltaTime)
		-- self.local_y = self.local_y + self.speed_y * deltaTime
		-- if self.local_y < self.lower_bound then
			-- self.is_jumping = false
			-- self.local_y = self.lower_bound
		-- end
	-- end
	
	-- local speed_sign = self.speed_x
	-- speed_sign = speed_sign / math.abs(speed_sign)
	-- if self.speed_x == 0 then
		-- speed_sign = 0
	-- end
	-- local inverse_speed_sign = speed_sign * -1
	
	-- local force_x_modifier = 1
	-- if speed_sign + x == 0 then
		-- force_x_modifier = force_x_modifier * 3
	-- end
	
	-- self.speed_x = self.speed_x + self.x_force * force_x_modifier * x * deltaTime
	
	-- local friction_modifier = 1
	-- if speed_sign + x == 1 or speed_sign + x == -1 then
		-- friction_modifier = 12
	-- end
	
	-- self.speed_x = self.speed_x + self.speed_x * self.x_friction * friction_modifier * -1 * deltaTime;
	
	-- if math.abs(self.speed_x) > self.x_speed_limit then
		-- self.speed_x = self.x_speed_limit * speed_sign
	-- end
	
	-- if x == 0 and math.abs(self.speed_x) < self.x_low_speed_limit then
		-- self.speed_x = 0
	-- end
	
	-- self.local_x = self.local_x + self.speed_x * deltaTime
	

	-- -setPosition(self.cobj.cptr, self.local_x, self.local_y, self.local_z)
	
	-- This is sample calling of a parent class function
	-- self:madd(5, 3)
	self.elapsed_time = self.elapsed_time + deltaTime;
	self.cobj.AddUniformFloat("elapsedTime", self.elapsed_time)
end

SampleMeta = {
	__index = function (tb, key)
		return smplFun
	end,
	__call = function (func, ...)
		--print("call triggered")
		print (func)
		print (func[1])
		--func(45, arg[1])
	end,
	something = function()
		print "zzzs"
	end
}

SomeTable = {
	function()
		print ("something")
	end
}

function someFuncs(x, y)
	print (x)
	print (y)
end

setmetatable(SomeTable, SampleMeta)
--setmetatable(someFuncs, SampleMeta)
