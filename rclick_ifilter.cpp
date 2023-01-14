#include <InputServerFilter.h>
#include <Message.h>
#include <Window.h>


class RClickInputFilter : public BInputServerFilter
{
public:
    virtual status_t InitCheck();
    virtual filter_result Filter(BMessage *message, BList *outList);
};

status_t RClickInputFilter::InitCheck()
{
    return B_OK;
}

filter_result RClickInputFilter::Filter(BMessage *message, BList *outList)
{
    if (message->what != B_MOUSE_DOWN)
        return B_DISPATCH_MESSAGE;

    int32 buttons;
    if (message->FindInt32("buttons", &buttons) != B_OK)
        return B_DISPATCH_MESSAGE;

    if (buttons != B_PRIMARY_MOUSE_BUTTON)
        return B_DISPATCH_MESSAGE;

    int32 modifiers;
    if (message->FindInt32("modifiers", &modifiers) != B_OK)
        return B_DISPATCH_MESSAGE;

    if (!(modifiers & B_CONTROL_KEY))
        return B_DISPATCH_MESSAGE;

    message->ReplaceInt32("buttons", B_SECONDARY_MOUSE_BUTTON);
    return B_DISPATCH_MESSAGE;
}

extern "C" BInputServerFilter* instantiate_input_filter() {
	return new(std::nothrow) RClickInputFilter();
}
