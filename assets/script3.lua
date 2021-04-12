function on_start()

end

function on_update(dt)
    this = GameObject:this()
    this.transform:set_translation(0, 0, 0)
    print(this)

    
end

function on_destroy()
    
end