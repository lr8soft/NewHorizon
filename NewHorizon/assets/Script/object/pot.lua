function onUpdateFunc()
    local gameObject = NewHorizon.getObject()
    rx, ry, rz = gameObject:getRotation()
    ry = -gameObject:getAccmulateTime() * 72.0
    gameObject:setRotation(rx, ry, rz)      
end

--wrapper
LightPot = {
    onFixedUpdate = onUpdateFunc
}