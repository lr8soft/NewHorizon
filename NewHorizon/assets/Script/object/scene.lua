function onUpdateFunc()
    local gameObject = NewHorizon.getObject()
end

--wrapper
Scene = {
    onFixedUpdate = onUpdateFunc
}