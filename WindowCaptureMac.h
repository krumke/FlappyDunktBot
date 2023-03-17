#ifndef WINDOWCAPTUREMAC_H
#define WINDOWCAPTUREMAC_H

#include <string>
#include <opencv2/core/mat.hpp>

class WindowCaptureMac
{
private:
    uint32_t windowID;

public:
    WindowCaptureMac(std::string const &windowOwner);
    WindowCaptureMac(uint32_t const newWindowID);

    // virtual ~WindowCaptureMac();

    uint32_t getWindowID() const;

    cv::Mat caputre();
};

#endif