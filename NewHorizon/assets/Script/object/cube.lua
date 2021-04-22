function onUpdateFunc()
    local gameObject = NewHorizon.getObject()
    x, y, z = gameObject:getPosition()
    y = math.sin(-gameObject:getAccmulateTime()) * 0.1 + 1.5
    gameObject:setPosition(x, y, z)

 
    rx, ry, rz = gameObject:getRotation()
    ry = -gameObject:getAccmulateTime()
    rz = math.sin(gameObject:getAccmulateTime())

    gameObject:setRotation(rx, ry, rz) 
end

--wrapper
Cube = {
    onFixedUpdate = onUpdateFunc
}