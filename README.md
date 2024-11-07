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

### SQLITE for entity setup
One table per component
One table for components per entitytype
Maybe use this for dialog as well

PseudoCode

    id = SELECT id FROM entityTypes WHERE name = "old_man"
    SELECT path FROM compDrawSprite WHERE entityId = id
    

### JSON for entity setup

    {
        "type": "old_man"
        "components": [
            "drawSprite": {
                "path": "assets/sprites/old_man.png"
            }
            "dialog": {
                "path": "assets/dialog/oldman.csv"
            }
        ]
    }

### LUA
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

If I have some missions, potentially have a time warp thing to play missions
again.

## Style
I like the idea of a bit of a surreal theme, to allow for weird stuff.

Modern things like elevators, buttons, machines. But also magic things like
portals and such.

### FONTS
https://www.dafont.com/dico.font?l[]=10&l[]=1


## Trait system instead of components?

basically bunch things up more, and do more like (movable),
to satisfy this you put the entity id in the movable sparse set, 
and then you just access the data through the big array of entities/soa of entities.

### Pros 
* Easier to think about and potentially easy to just add lists of ids,
then just loop through them.
* Could be cool for lua, have callbacks for the different traits,
like (update_flammable, on_flammable_added, on_flammable_removed), that
you call based on this.

### Cons
* Might just be ecs with extra steps, as ecs is the same except you just
do sets of components as the thing


### what do I actually want?
1. make level in ldtk
2. set up entities with traits
3. set up entities with lua scripts for unique behavior
4. handle traits in c and lua, makes the code very easy to reason about
5. we don't care about who needs what data, because the data is allocated
for all entities anyway, which is probably fine.
If we want, we could have trait specific data as well, separated from
components

## NETWORKING
I should probably set things up so code takes the same path locally as
it does when sending it through internet.
So even if I press left locally it should run it through a network event locally,
then send it, and we discard it when it returns again (or use it to check validity)
