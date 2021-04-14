function on_start()
end


function on_update(dt)
    
    if Keyboard.is_key_pressed(87) then
        GameObject:this().transform:translate(0, 0.2, 0)
    end
    if Keyboard.is_key_pressed(83) then
        GameObject:this().transform:translate(0, -0.2, 0)
    end
    if Keyboard.is_key_pressed(65) then
        GameObject:this().transform:translate(-0.2, 0, 0)
    end
    if Keyboard.is_key_pressed(68) then
        GameObject:this().transform:translate(0.2, 0, 0)
    end

end

function on_destroy()
    
end