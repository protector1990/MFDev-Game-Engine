-- primer:
-- self.navGrid.setSampleDensity(5)

Accessor = {}

Accessor.__index = function (tb, key)
	local value = tb._exptb[key]
	local valType = type(value)
	if valType == "table" then
		return value
	elseif valType == "function" then
		value(tb._exptb.reference, value)
	end
end