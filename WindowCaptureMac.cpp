#include "WindowCaptureMac.h"
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

// Constructors:
WindowCaptureMac::WindowCaptureMac(std::string const &windowOwner)
{
    auto windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);

    auto windowOwnerCFString = CFStringCreateWithCString(NULL, windowOwner.c_str(), kCFStringEncodingUTF8);

    CFIndex windowCount = CFArrayGetCount(windowList);
    for (CFIndex i = 0; i < windowCount; i++)
    {
        auto windowInfo = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
        auto windowOwnerNameRef = (CFStringRef)CFDictionaryGetValue(windowInfo, kCGWindowOwnerName);

        if (windowOwnerNameRef != NULL)
        {
            if (CFStringCompare(windowOwnerCFString, windowOwnerNameRef, 0) == kCFCompareEqualTo)
            {
                windowID = (CFNumberRef)CFDictionaryGetValue(windowInfo, kCGWindowNumber);
                break;
            }
        }
    }

    CFRelease(windowList);
    CFRelease(windowOwnerCFString);
}

WindowCaptureMac::WindowCaptureMac(int const newWindowID)
{
    windowID = CFNumberCreate(NULL, kCFNumberIntType, &newWindowID);
}

// Destuctor:
WindowCaptureMac::~WindowCaptureMac()
{
    CFRelease(windowID);
}

// Methods:
int WindowCaptureMac::getWindowID() const
{
    if (windowID == NULL)
    {
        return -1;
    }

    int windowIDInt;
    CFNumberGetValue(windowID, kCFNumberSInt32Type, &windowIDInt);
    return windowIDInt;
}
