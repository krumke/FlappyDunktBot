#include <opencv2/opencv.hpp>
#include <CoreGraphics/CoreGraphics.h>
#include <iostream>
#include <chrono>

#include "WindowCaptureMac.h"

using namespace cv;

void testConverter()
{
    auto wc = WindowCaptureMac("Safari");
    wc.testConverter();
}

void testTimeComplexity()
{
    auto wc = WindowCaptureMac("Safari");
    auto loopTime = std::chrono::high_resolution_clock::now();

    std::string imagePath1 = samples::findFile("/Users/krumke/Documents/krumke_git/FlappyDunktBot/FlappyDunkGame.jpg");
    auto haystackImg = imread(imagePath1, IMREAD_UNCHANGED);

    while (true)
    {
        auto screenshot = wc.caputre();
        auto fps = 1000.0f / (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - loopTime)).count();
        std::cout << "FPS: " << fps << std::endl;
        loopTime = std::chrono::high_resolution_clock::now();

        imshow("please", screenshot);

        if (waitKey(1) == 'q')
        {
            cv::destroyAllWindows();
            break;
        }
    }
}

void loop()
{
    auto wc = WindowCaptureMac("Mail");
    std::cout << wc.getWindowID() << std::endl;

    auto loopTime = std::chrono::high_resolution_clock::now();

    while (true)
    {

        auto screenshot = wc.caputre();
        imshow("please", screenshot);

        auto fps = 1000.0f / (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - loopTime)).count();
        std::cout << "FPS: " << fps << std::endl;
        loopTime = std::chrono::high_resolution_clock::now();

        if (waitKey(1) == 'q')
        {
            cv::destroyAllWindows();
            break;
        }
    }
}

int main()
{

    // testConverter();

    // loop();

    testTimeComplexity();

    std::string imagePath1 = samples::findFile("/Users/krumke/Documents/krumke_git/FlappyDunktBot/FlappyDunkGame.jpg");
    std::string imagePath2 = samples::findFile("/Users/krumke/Documents/krumke_git/FlappyDunktBot/FlappyDunkPlayer.png");

    auto haystackImg = imread(imagePath1, IMREAD_UNCHANGED);
    auto needleImg = imread(imagePath2, IMREAD_UNCHANGED);

    Mat result;

    matchTemplate(haystackImg, needleImg, result, TM_CCOEFF_NORMED);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    double treshold = 0.3;
    if (maxVal >= treshold)
    {
        rectangle(haystackImg, maxLoc, cv::Point(maxLoc.x + needleImg.cols, maxLoc.y + needleImg.rows), Scalar::all(0), 2, 8, 0);

        // imshow("result_window", haystackImg);
        // waitKey();
    }

    return 0;
}
