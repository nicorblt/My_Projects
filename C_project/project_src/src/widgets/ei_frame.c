#include "widgets/ei_frame.h"
#include "helpers/ei_memory.h"

void ei_frame_set_text(ei_frame_t* frame, const char *text, size_t length)
{
    if (text == NULL)
    {
        free(frame->text);
        frame->text = NULL;
        return;
    }
    frame->text = verified_realloc(frame->text, (length+1) * sizeof(char));
    strncpy(frame->text, text, length+1);
}