function on_start()

    for x = 0, 4 do
        for y = 0, 1 do
            for z = 0, 4 do
                obj = GameObject:this():create_child("Block")
                mesh = obj:create_component("Mesh")
                mesh:set_mesh("assets/test.fbx")
                mesh_renderer = obj:create_component("MeshRenderer")
                mesh_renderer:set_material("block_material")
                obj.transform:set_translation(x, y, z);
                obj.transform:set_scale(0.5, 0.5, 0.5)
            end
        end
    end

end

function on_update()

end

function on_destroy()

end