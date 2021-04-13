Vector3 = {}
Vector3.__index = Vector3
Vector3MetaTable = {}

function Vector3:new(x, y, z)
    return setmetatable({x = x or 0, y = y or 0, z = z or 0}, Vector3)
end

function Vector3.__add(a, b)
    return Vector3.new(a.x + b.x, a.y + b.y, a.z + b.z)
end

function Vector3.__sub(a, b)
    return Vector3.new(a.x - b.x, a.y - b.y, a.z - b.z)
end

function Vector3.__mul(a, b)
    return Vector3.new(a.x * b.x, a.y * b.y, a.z * b.z)
end

function Vector3.__div(a, b)
    return Vector3.new(a.x / b.x, a.y / b.y, a.z / b.z)
end