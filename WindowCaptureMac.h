#ifndef WINDOWCAPTUREMAC_H
#define WINDOWCAPTUREMAC_H

#include <string>
#include <opencv2/core/mat.hpp>
#include <CoreGraphics/CoreGraphics.h>

class WindowCaptureMac
{
private:
    uint32_t windowID;

    void convertImgRefToMat(CGImageRef cgImgRef, cv::Mat &image);

    std::vector<uint8_t> convertImgRefToMatEfficient(CGImageRef cgImgRef);

public:
    WindowCaptureMac(std::string const &windowOwner);
    WindowCaptureMac(uint32_t const newWindowID);

    // virtual ~WindowCaptureMac();

    void testConverter();

    uint32_t getWindowID() const;

    cv::Mat caputre();

    struct NoCaptureableWindow : public std::exception
    {
        const char *what() const throw()
        {
            return "No Window found that ist captureable. Check Window Name or try restarting the aplication you try to capture";
        }
    };
};

#endif