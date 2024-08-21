ECS = ECS or {}

Luatest = {}

function Luatest.testfunc(e)
	local position = ECS.get_component(e, "position")
	print(position.value.x)
	print(position.value.y)
	--position.value.x = position.value.x - 1000
	--position.value.y = position.value.y + 100
	print(position.value.x)
	print(position.value.y)
	ECS.set_component(e, position)
	print("yas queen")
end

function Luatest.onCreate(e)
	print("HEJ")

	-- local velocity = {
	-- 	type = "velocity",
	-- 	value = {
	-- 		x = 0,
	-- 		y = 0
	-- 	}
	-- }

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
	ECS.add_component(e, {
		type = "position",
		value = {
			x = 0.0,
			y = 0.0
		},
		grid_pos = {
			x = 0.0,
			y = 0.0
		},
		layer = 0
	})

	ECS.add_component(e, draw_sprite)
	--ECS.add_component(e, draw_box)
	ECS.add_component(e, input)

	ECS.add_component(e, {
		type = "mover",
		movement_speed = 5
	})
end

-- TO RUN THIS YOU NEED TO REGISTER USER DATA FOR ecs
