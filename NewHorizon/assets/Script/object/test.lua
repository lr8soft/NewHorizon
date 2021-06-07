--wrapper
Test = {}

function Test.onFixedUpdate()
    local gameObject = NewHorizon.getObject()
    x, y, z = gameObject:getPosition()
    y = math.sin(gameObject:getAccmulateTime())
    gameObject:setPosition(x, y, z)


    accmulateTime = gameObject:getAccmulateTime()
    rx, ry, rz = gameObject:getRotation()
    ry = accmulateTime
    gameObject:setRotation(rx, ry, rz)

    
    if (accmulateTime > 5) then
        gameObject:setDead(true)
        print("Now object", gameObject:getTagName(), "release.")

        sx, sy, sz = gameObject:getScale()

        local newGameObject = NewHorizon.newInstance("Cube", "kksk")
        newGameObject:setPosition(x, y, z)
        newGameObject:setRotation(rx, ry, rz)
        newGameObject:setScale(sx, sy, sz)

        print("Create new instance")
    end        
end

