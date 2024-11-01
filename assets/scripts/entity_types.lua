Entity = {
	types = {
	}
}

function Entity.register(name, data)
	print('registering ' .. name)
	print(data)
	Entity.types[name] = data
end

function Entity.get_type(type)
	return Entity.types[type]
end

function Entity.get_event(type, event)
	return Entity.types[type][event]
end

function Entity.call_event(type, event, ...)
	Entity.types[type][event](...)
end
