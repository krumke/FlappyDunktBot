#include "WindowCaptureMac.h"
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <opencv2/core/mat.hpp>

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
                auto windowIDNumberRef = (CFNumberRef)CFDictionaryGetValue(windowInfo, kCGWindowNumber);
                CFNumberGetValue(windowIDNumberRef, kCFNumberSInt32Type, &windowID);
                break;
            }
        }
    }

    CFRelease(windowList);
    CFRelease(windowOwnerCFString);
}

WindowCaptureMac::WindowCaptureMac(uint32_t const newWindowID)
    : windowID(newWindowID)
{
}

// Methods:
uint32_t WindowCaptureMac::getWindowID() const
{
    return windowID;
}

cv::Mat WindowCaptureMac::caputre()
{
    auto cgImgRef = CGWindowListCreateImage(CGRectNull, kCGWindowListOptionIncludingWindow, windowID, kCGWindowImageBoundsIgnoreFraming);
}
