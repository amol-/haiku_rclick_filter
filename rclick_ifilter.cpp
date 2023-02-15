#include <InputServerFilter.h>
#include <Message.h>
#include <Window.h>


class RClickInputFilter : public BInputServerFilter
{
private:
	bool scrollingInProgress;
	BPoint previousMousePosition;
public:
	RClickInputFilter();
    virtual status_t InitCheck();
    virtual filter_result Filter(BMessage *message, BList *outList);
};

RClickInputFilter::RClickInputFilter()
: scrollingInProgress(false)
{
}

status_t RClickInputFilter::InitCheck()
{
    return B_OK;
}

filter_result RClickInputFilter::Filter(BMessage *message, BList *outList)
{
    if ((message->what != B_MOUSE_DOWN) && (message->what != B_MOUSE_MOVED))
        return B_DISPATCH_MESSAGE;

	int32 modifiers;
	if (message->FindInt32("modifiers", &modifiers) != B_OK) {
		scrollingInProgress = false;
		return B_DISPATCH_MESSAGE;
	}

	if (!(modifiers & B_CONTROL_KEY)) {
		scrollingInProgress = false;
		return B_DISPATCH_MESSAGE;
	}

	if (message->what == B_MOUSE_DOWN) {
		// Turn CTRL+CLICK into RIGHT CLICK
		int32 buttons;
		if (message->FindInt32("buttons", &buttons) != B_OK)
			return B_DISPATCH_MESSAGE;

		if (buttons != B_PRIMARY_MOUSE_BUTTON)
			return B_DISPATCH_MESSAGE;

		message->ReplaceInt32("buttons", B_SECONDARY_MOUSE_BUTTON);
	}
	else if (message->what == B_MOUSE_MOVED) {
		// Turn CTRL+MOVE into SCROLLING
		BPoint newMousePosition;
		if (message->FindPoint("where", &newMousePosition) != B_OK)
			return B_DISPATCH_MESSAGE;

		if (!scrollingInProgress) {
			scrollingInProgress = true;
			previousMousePosition = newMousePosition;
			return B_DISPATCH_MESSAGE;
		}

		float deltay = newMousePosition.y - previousMousePosition.y;
		message->what = B_MOUSE_WHEEL_CHANGED;
		message->AddFloat("be:wheel_delta_y", deltay);
	}
    return B_DISPATCH_MESSAGE;
}

extern "C" BInputServerFilter* instantiate_input_filter() {
	return new(std::nothrow) RClickInputFilter();
}
