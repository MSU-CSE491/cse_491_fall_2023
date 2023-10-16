#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

cv::CascadeClassifier face_cascade;

void faceDetection(cv::Mat frame)
{
    cv::Mat frame_gray;

    // Convert to gray so that we don't have to deal with color and reduce computation time
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

    // Detect faces
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces);

    for (auto & face : faces)
    {
        cv::Point center(face.x + face.width/2, face.y + face.height/2);
        cv::ellipse(frame, center, cv::Size(face.width/2, face.height/2), 0, 0, 360, cv::Scalar(255, 0, 255), 4);

        cv::Mat faceROI = frame_gray(face);
    }

    cv::imshow("Face Detection", frame);
}

int main()
{
    std::string faceClassifierPath = R"(C:\opencv\sources\data\haarcascades\haarcascade_frontalface_alt.xml)";

    if (!face_cascade.load(faceClassifierPath))
    {
        std::cout << "Error loading face classifier" << std::endl;
        return -1;
    }

    std::cout << "Face classifier loaded successfully" << std::endl;


    cv::VideoCapture cap(1);

    if (!cap.isOpened())
    {
        std::cout << "Error opening camera" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame))
    {
        if (frame.empty())
        {
            std::cout << "Error reading frame" << std::endl;
            break;
        }

        faceDetection(frame);

        if (cv::waitKey(10) == 'q')
        {
            break;
        }
    }

    return 0;
}