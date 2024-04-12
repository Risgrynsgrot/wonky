# wonky-c

## UI
* system with support for lists of buttons. that's the main thing for now
* potential support for lua, more dynamic and easier to iterate.
* Probably immediate mode

## Systems
* Lua support, think it could be cool for modding support and easy iteration, hot reloading?
* Reworking into better ecs system
* the game itself lol
* Level loading via LdTk or Tiled

* Lua for all entity setup, example:
```lua
function onCreate(e)
    local movement = e:addComponent(C_MOVEMENT)
    movement.speed = 10

    e:addComponent(FLAMMABLE)
    e:addComponent(VELOCITY)
end

function onUpdate(e)
    -- Do bespoke things that can't/shouldn't be put in a system
end
function onDoorOpen(e)
    -- Possibly have event functions for single triggers as well
end
```

## The game itself
Chill multiplayer game, cross between progressiony games like terraria/factorio, animal crossing and runescape

Walk around, interact, use items.

Level up skills, semi grindy.

Inspiration from runescape: Activity level as design philosophy. Different
systems are different levels of active, to encourage playing casually and
actively depending on what works for you at that time.

Tamagotchi like pet system?

Weather system

## Style
I like the idea of a bit of a surreal theme, to allow for weird stuff.

Modern things like elevators, buttons, machines. But also magic things like
portals and such.
