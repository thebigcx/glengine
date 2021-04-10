function on_start()
    
end

function on_update(dt)
    tr = GameObject:this().transform

    
    if Keyboard.is_key_pressed(87) then
        tr:translate(0, 0.1, 0)
    end
    if Keyboard.is_key_pressed(83) then
        tr:translate(0, -0.1, 0)
    end
    if Keyboard.is_key_pressed(65) then
        tr:translate(-0.1, 0, 0)
    end
    if Keyboard.is_key_pressed(68) then
        tr:translate(0.1, 0, 0)
    end
    
    camera = Camera.main

end

function on_destroy()
    
end