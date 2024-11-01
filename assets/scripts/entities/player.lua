Comp = Comp or {}
Trait = Trait or {}
Asset = Asset or {}

Player = {}

function Player.on_create(e)
	print("player on create")
	Asset.add_sprite(e, "assets/test.png") --this might want an optional offset
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

	Comp.set(e, {
		type = Comp.input,
		input_id = 0,
		direction = {
			0,
			0
		}
	})

	Comp.set(e, {
		type = Comp.mover,
		movement_speed = 5
	})
	Trait.add(e, Trait.movable)
	Trait.add(e, Trait.player_controlled)
end

Entity.register("player", Player)
