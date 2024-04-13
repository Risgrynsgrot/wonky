Id_comp_position = 10 --This comes from C
Id_comp_flammable = 11 --This comes from C

--alt 1
function OnCreate(e)
	local position = e:get_component(Id_comp_position) --getComponent is c function
	-- in C: LuaTable_t get_component(ecs_id_t entity, ecs_id_t component)
	-- returns/pushes a luatable to the stack with the right component values

end
function OnCreate(ecs, e)
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
