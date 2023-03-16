#ifndef WINDOWCAPTUREMAC_H
#define WINDOWCAPTUREMAC_H

#include <string>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CFNumber.h>

class WindowCaptureMac
{
private:
    CFNumberRef windowID = NULL;

public:
    WindowCaptureMac(std::string const &windowOwner);
    WindowCaptureMac(int const newWindowID);

    virtual ~WindowCaptureMac();

    int getWindowID() const;

    // CGImageRef caputre();
};

void printRunningWindowOwners();

#endif