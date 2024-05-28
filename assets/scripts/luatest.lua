Luatest = {}

function Luatest.testfunc(e)
	local position = e:get_component("comp_position")
	position.x = position.x + 10
	position.y = position.y + 10
end

function Luatest.onCreate(e)
	print("HEJ")
	local position = {
		type = "comp_position",
		value = {
			x = 800,
			y = 800
		}
	}

	--local flammable = {
	--	type = "comp_flammable",
	--	burntime = 2.0
	--}

	print("entityID in lua: " .. tostring(e))
	ECS.AddComponent(e, position)
	print("HEJ2")
	--ecs:add_component(e, flammable)
end
-- TO RUN THIS YOU NEED TO REGISTER USER DATA FOR ecs
