Id_comp_position = 10 --This comes from C
Id_comp_flammable = 11 --This comes from C

local luatest = {}

function luatest.onCreate(ecs, e)
	local position = {
		type = Id_comp_position,
		value = {
			x = 10,
			y = 10
		}
	}

	local flammable = {
		type = Id_comp_flammable,
		burntime = 2.0
	}

	ecs:Add_component(e, position)
	ecs:Add_component(e, flammable)
end
