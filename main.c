#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "./game.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

int main(void)
{
    init();

    const size_t width = get_display_width();
    const size_t height = get_display_height();
    uint32_t *pixels = get_display();
    printf("Display %zux%zu at %p\n", width, height, pixels);

    next_frame(0.0);

    const char * const frame_file_path = "frame.png";

    if (!stbi_write_png(frame_file_path, width, height, 4, pixels, width * 4)) {
        fprintf(stderr, "Could not save file `%s`: %s\n",
                frame_file_path, strerror(errno));
        exit(1);
    }

    return 0;
}
