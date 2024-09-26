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

    cv::Mat cvMat;
    convertCGImageRefToMatReuseMemory(cgImgRef, cvMat);

    CGImageRelease(cgImgRef);

    return cvMat;
}

// not working
void WindowCaptureMac::convertCGImageRefToMatReuseMemory(CGImageRef cgImageRef, cv::Mat &image)
{
    size_t width = CGImageGetWidth(cgImageRef);
    size_t height = CGImageGetHeight(cgImageRef);
    size_t bytesPerRow = CGImageGetBytesPerRow(cgImageRef);
    size_t expectedDataSize = height * bytesPerRow;

    // Falls die Größe übereinstimmt, wird der Speicher wiederverwendet
    if (image.empty() || image.cols != width || image.rows != height)
    {
        image.create(height, width, CV_8UC4);
    }

    // Rohdaten von CGImageRef abrufen
    auto coreGraphicsDataProvider = CGImageGetDataProvider(cgImageRef);
    CFDataRef coreGraphicsData = CGDataProviderCopyData(coreGraphicsDataProvider);
    const UInt8 *rawBytes = CFDataGetBytePtr(coreGraphicsData);

    // OpenCV-Matrix mit neuen Daten füllen, prüfe ob die Größe korrekt ist
    if (image.data && rawBytes)
    {
        memcpy(image.data, rawBytes, expectedDataSize); // Kopiere die Daten
    }

    CFRelease(coreGraphicsData);
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

void WindowCaptureMac::convertImgRefToMatEfficient2(CGImageRef cgImageRef, cv::Mat &image)
{
    size_t bytesPerRow = CGImageGetBytesPerRow(cgImageRef);
    size_t width = CGImageGetWidth(cgImageRef);
    size_t height = CGImageGetHeight(cgImageRef);

    auto coreGraphicsDataProvider = CGImageGetDataProvider(cgImageRef);
    CFDataRef coreGraphicsData = CGDataProviderCopyData(coreGraphicsDataProvider);

    const UInt8 *rawBytes = CFDataGetBytePtr(coreGraphicsData);

    image = cv::Mat(height, width, CV_8UC4, (UInt8 *)rawBytes, bytesPerRow);

    cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);

    CFRelease(coreGraphicsData);
}

void WindowCaptureMac::convertImgRefToMatEfficient1(CGImageRef cgImageRef, cv::Mat &image)
{
    size_t width = CGImageGetWidth(cgImageRef);
    size_t height = CGImageGetHeight(cgImageRef);
    size_t bytesPerPixel = 4; // Assuming RGBA (32-bit)
    size_t bytesPerRow = bytesPerPixel * width;
    size_t bitsPerComponent = 8; // 8 bits per channel (32 bits total)

    // Directly allocate the cv::Mat with appropriate size and type
    image.create(height, width, CV_8UC4); // Avoids multiple allocations

    // Get the color space of the image
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImageRef);

    // Create a CGContext that draws directly into the Mat's data
    CGContextRef context = CGBitmapContextCreate(image.data, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast);

    // Draw the image directly into the Mat's data buffer
    if (context != nullptr)
    {
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgImageRef);
        CGContextRelease(context); // Release the context as soon as it's no longer needed
    }

    // Convert from RGBA to BGR (OpenCV uses BGR by default)
    // cv::cvtColor(image, image, cv::COLOR_RGBA2BGR);
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
        cv::Mat image;
        convertImgRefToMatEfficient2(cgImage, image);
        // cv::cvtColor(cvMat, cvMat, cv::COLOR_RGB2BGR);
        //  cvMat.convertTo(cvMat, CV_32F, 1.0 / 255);
        cv::imshow("test", image);
        cv::waitKey();
    }

    CGImageRelease(cgImage);
    CGDataProviderRelease(cgDataProvider);
}