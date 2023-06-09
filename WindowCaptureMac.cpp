#include "WindowCaptureMac.h"
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
// #include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>

// Constructors:
WindowCaptureMac::WindowCaptureMac(std::string const &windowOwner)
{
    auto windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);

    auto windowOwnerCFString = CFStringCreateWithCString(NULL, windowOwner.c_str(), kCFStringEncodingUTF8);

    bool windowFound = false;

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

                auto cgImgRef = CGWindowListCreateImage(CGRectNull, kCGWindowListOptionIncludingWindow, windowID, kCGWindowImageBoundsIgnoreFraming);

                if (cgImgRef != NULL)
                {
                    CGImageRelease(cgImgRef);
                    windowFound = true;
                    break;
                }
                else
                {
                    CGImageRelease(cgImgRef);
                }
            }
        }
    }

    if (windowFound)
    {
        std::cout << "Window Found" << std::endl;
    }
    else
    {
        throw NoCaptureableWindow();
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

    if (cgImgRef == NULL)
    {
        std::cout << "Window not found" << std::endl;
    }
    cv::Mat cvMat;
    convertImgRefToMat(cgImgRef, cvMat);

    CGImageRelease(cgImgRef);

    return cvMat;
}

void WindowCaptureMac::convertImgRefToMat(CGImageRef cgImageRef, cv::Mat &image)
{
    size_t width = CGImageGetWidth(cgImageRef);
    size_t height = CGImageGetHeight(cgImageRef);
    size_t bytesPerPixel = 4;
    size_t bytesPerRow = bytesPerPixel * width;
    size_t bitsPerComponent = 8;
    image = cv::Mat(cv::Size(width, height), CV_8UC4, bytesPerRow);
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImageRef);

    unsigned char *rawData = image.data;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast);

    CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgImageRef);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    CGContextRelease(context);
}

void WindowCaptureMac::testConverter()
{

    const char *path = "/Users/krumke/Documents/krumke_git/FlappyDunktBot/GameCanvas1.png";

    auto cgDataProvider = CGDataProviderCreateWithFilename(path);

    auto cgImage = CGImageCreateWithPNGDataProvider(cgDataProvider, nullptr, true,
                                                    kCGRenderingIntentDefault);

    if (cgImage == nullptr)
    {
        std::cout << "Picture is NULL" << std::endl;
    }
    else
    {
        cv::Mat cvMat;
        convertImgRefToMat(cgImage, cvMat);
        // cv::cvtColor(cvMat, cvMat, cv::COLOR_RGB2BGR);
        //  cvMat.convertTo(cvMat, CV_32F, 1.0 / 255);
        cv::imshow("test", cvMat);
        cv::waitKey();
    }

    CGImageRelease(cgImage);
    CGDataProviderRelease(cgDataProvider);
}