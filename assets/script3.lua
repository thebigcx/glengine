function on_start()
    Scene.switch_scene("assets/temp.scene")
end

function on_update(dt)
    this = GameObject:this()
    this.transform:set_translation(0, 0, 0)

    vec = Vector3.new(1, 2, 2)
    vec2 = Vector3.new(1, 1, 1)
    this.transform:set_translation(vec)
end

function on_destroy()
    
end