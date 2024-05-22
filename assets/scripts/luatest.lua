local luatest = {}

function luatest.onCreate(ecs, e)
	local position = {
		type = "comp_position",
		value = {
			x = 10,
			y = 10
		}
	}

	--local flammable = {
	--	type = "comp_flammable",
	--	burntime = 2.0
	--}

	ecs:add_component(e, position)
	--ecs:add_component(e, flammable)
end
-- TO RUN THIS YOU NEED TO REGISTER USER DATA FOR ecs
