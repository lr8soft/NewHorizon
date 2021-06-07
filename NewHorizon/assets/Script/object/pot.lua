--wrapper
LightPot = {}

function LightPot.onFixedUpdate()
    local gameObject = NewHorizon.getObject()
    rx, ry, rz = gameObject:getRotation()
    ry = -gameObject:getAccmulateTime() * 72.0
    gameObject:setRotation(rx, ry, rz)      
end

