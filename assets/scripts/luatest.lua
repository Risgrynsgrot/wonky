Luatest = {}

function Luatest.testfunc(e)
	local position = ECS.get_component(e, "position")
	position.x = position.x + 10
	position.y = position.y + 10
end

function Luatest.onCreate(e)
	print("HEJ")
	local position = {
		type = "position",
		value = {
			x = 800,
			y = 400
		}
	}

	local velocity = {
		type = "velocity",
		value = {
			x = 0,
			y = 0
		}
	}

	--local draw_box = {
	--	type = "draw_box",
	--	width = 1000,
	--	height = 1000,
	--	color = {
	--		r = 255,
	--		g = 0.0,
	--		b = 255,
	--		a = 255
	--	}
	--}

	local draw_sprite = {
		type = "asset_sprite",
		path = "assets/test.png"
	}

	local input = {
		type = "input",
		input_id = 0
	}

	print("entityID in lua: " .. tostring(e))
	ECS.add_component(e, position)
	ECS.add_component(e, velocity)
	ECS.add_component(e, draw_sprite)
	--ECS.add_component(e, draw_box)
	ECS.add_component(e, input)
end

-- TO RUN THIS YOU NEED TO REGISTER USER DATA FOR ecs
