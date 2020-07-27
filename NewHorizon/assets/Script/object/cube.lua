function onUpdateFunc()
    local gameObject = NewHorizon.getObject()
    x, y, z = gameObject:getPosition()
    y = math.sin(-gameObject:getAccmulateTime())
    gameObject:setPosition(x, y, z)

    rx, ry, rz = gameObject:getRotation()
    ry = -gameObject:getAccmulateTime()
    gameObject:setRotation(rx, ry, rz)        
end

--wrapper
Cube = {
    onFixedUpdate = onUpdateFunc
}