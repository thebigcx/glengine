function on_start()
    
    child = GameObject:this():create_child("New Child From Lua")
    child.transform:set_translation(1, 0, 0)
    --debug.dumpstack()
    --child:create_component("Sprite")

end

function on_update(dt)
    obj = GameObject:this()
    obj:get_component("AudioSource")
    tr = obj.transform
    tr:set_translation(0, 0, 0)
    
    if Keyboard.is_key_pressed(87) then
        sprite = GameObject:this():get_component("Sprite")
        sprite:set_color(Vector3.new(1, 0, 0))
    end
    if Keyboard.is_key_pressed(83) then
        sprite = GameObject:this():get_component("Sprite")
        sprite:set_color(Vector3.new(0, 1, 0))
    end


    if Keyboard.is_key_pressed(65) then
        if GameObject:this():has_component("AudioSource") then
            audio = GameObject:this():get_component("AudioSource"):play()
        end
    end
    if Keyboard.is_key_pressed(68) then
        if GameObject:this():has_component("AudioSource") then
            audio = GameObject:this():get_component("AudioSource"):pause()
        end
    end

    
    
    camera = Camera.main

end

function on_destroy()
    
end