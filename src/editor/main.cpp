#include <iostream>

#include "editor/editor.h"

int main(int argc, char* argv[])
{
    Editor* editor = new Editor();
    editor->run(argv[1]);
    delete editor;

    return 0;
}