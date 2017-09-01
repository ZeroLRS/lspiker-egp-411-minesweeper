#include "MouseEvent.h"


MouseEvent::MouseEvent(UINT x, UINT y, MouseButton theButton)
	:Event(MOUSE_CLICKED_EVENT)
	,mX(x)
	,mY(y)
	,mButton(theButton)
{

}
