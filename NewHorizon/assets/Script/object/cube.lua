function onUpdateFunc()
    local gameObject = NewHorizon.getObject()
    x, y, z = gameObject:getPosition()
    y = math.sin(-gameObject:getAccmulateTime()) * 0.1 + 1.5
    gameObject:setPosition(x, y, z)

 
    rx, ry, rz = gameObject:getRotation()
    rx = gameObject:getAccmulateTime() * 36.0
    rz = gameObject:getAccmulateTime() * 36.0


    gameObject:setRotation(rx, ry, rz) 
end

--wrapper
Cube = {
    onFixedUpdate = onUpdateFunc
}