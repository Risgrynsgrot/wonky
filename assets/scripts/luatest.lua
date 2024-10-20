ECS = ECS or {}
Comp = Comp or {}
Trait = Trait or {}
Asset = Asset or {}

Luatest = {}

function Luatest.testfunc(e)
	local position = Comp.get(e, Comp.position)
	print(position.value.x)
	print(position.value.y)
	--position.value.x = position.value.x - 1000
	--position.value.y = position.value.y + 100
	print(position.value.x)
	print(position.value.y)
	Comp.set(e, position)
	print("yas queen")
end

function Luatest.onCreate(e)
	print("HEJ")

	-- local velocity = {
	-- 	type = Comp.velocity,
	-- 	value = {
	-- 		x = 0,
	-- 		y = 0
	-- 	}
	-- }

	--local draw_box = {
	--	type = Comp.draw_box,
	--	width = 1000,
	--	height = 1000,
	--	color = {
	--		r = 255,
	--		g = 0.0,
	--		b = 255,
	--		a = 255
	--	}
	--}
	print("yah")
	print(Comp.position)

	Asset.add_sprite(e, "assets/test.png")

	local input = {
		type = Comp.input,
		input_id = 0,
		direction = {
			0,
			0
		}
	}

	print("entityID in lua: " .. tostring(e))
	Comp.set(e, {
		type = Comp.position,
		value = {
			x = 0.0,
			y = 0.0
		},
		grid_pos = {
			x = 0,
			y = 0
		},
		layer = 0
	})

	--Comp.set(e, draw_box)
	Comp.set(e, input)

	Comp.set(e, {
		type = Comp.mover,
		movement_speed = 5
	})
	Trait.add(e, Trait.movable)
	Trait.add(e, Trait.player_controlled)
end
