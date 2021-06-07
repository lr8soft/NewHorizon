--wrapper
Scene = {hasPlayerMusic = false}

function Scene.onFixedUpdate()
    local gameObject = NewHorizon.getObject()
 
    if(not Scene.hasPlayerMusic) then
        gameObject:playAudio("1645")    --play audo file 1645
        Scene.hasPlayerMusic = true

        print("start play music 1645test")
    end
end