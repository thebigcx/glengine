function on_start()
    generate_world()
end

function generate_world()
    for x = 0, 10 do
        for y = 0, 10 do

            local obj = GameObject:this():create_child("Block")

            local sprite = obj:create_component("Sprite")

            sprite:set_texture("assets/texture_test.png")
            sprite:use_uvs(true)
            sprite:set_uv1(240, 0)
            sprite:set_uv2(256, 16)

            obj.transform:set_translation(x, y, 0)


        end
    end
end

function on_update(dt)
    
    
end

function on_destroy()
    
end