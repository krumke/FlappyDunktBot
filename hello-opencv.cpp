#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;

int main()
{
    std::string imagePath1 = samples::findFile("/Users/krumke/Documents/krumke_git/FlappyDunktBot/FlappyDunkGame.jpg");
    std::string imagePath2 = samples::findFile("/Users/krumke/Documents/krumke_git/FlappyDunktBot/FlappyDunkPlayer.png");

    auto haystackImg = imread(imagePath1, IMREAD_UNCHANGED);
    auto needleImg = imread(imagePath2, IMREAD_UNCHANGED);

    Mat result;

    matchTemplate(haystackImg, needleImg, result, TM_CCOEFF_NORMED);

    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    double treshold = 0.8;
    if (maxVal >= treshold)
    {
        rectangle(haystackImg, maxLoc, Point(maxLoc.x + needleImg.cols, maxLoc.y + needleImg.rows), Scalar::all(0), 2, 8, 0);

        imshow("result_window", haystackImg);
        waitKey();
    }

    return 0;
}