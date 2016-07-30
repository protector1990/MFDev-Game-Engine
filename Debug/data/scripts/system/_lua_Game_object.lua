Accessors = {}

Accessors.__index = function (tb, key)
	local value = tb[key]
	local valType = type(value)
	if valType == "table" then
		return value
	elseif valType == "function" then
		luaObjectCall(object, value)
	end
end