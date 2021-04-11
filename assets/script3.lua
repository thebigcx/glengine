function on_start()

end

function on_update(dt)
    if Keyboard.is_key_pressed(87) then
        GameObject:this():get_component("AudioSource"):play()
    end
    if Keyboard.is_key_pressed(83) then
        GameObject:this():get_component("AudioSource"):pause()
    end
end

function on_destroy()
    
end