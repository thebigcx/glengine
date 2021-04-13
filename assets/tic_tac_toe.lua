function on_start()

end

function on_update()

    if Mouse.is_mouse_pressed(0) then
        print("Mouse clicked")
        if Mouse.get_mouse_position().x > 0 and Mouse.get_mouse_position().y > 0 then
            local piece = GameObject:create("Piece")
            piece.transform:set_translation(0, 0, 0)
        end
    end

end

function on_destroy()

end