#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

int main(int argc, char *argv[])
{
    cv::VideoCapture capture = cv::VideoCapture(0);
    cv::Mat frame, filtered;
	
	int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
	
    while (true)
    {
        auto time1 = std::chrono::high_resolution_clock::now();
        capture.read(frame);
		auto time2 = std::chrono::high_resolution_clock::now();
		
        cv::medianBlur(frame, filtered, 15);
		auto time3 = std::chrono::high_resolution_clock::now();
        		 

        cv::imshow("Bilateral Filtered", filtered);
		auto time4 = std::chrono::high_resolution_clock::now();
        char c = cv::waitKey(1);
        if (c == 27) {
            break;
        }
		
		frameCount++;
		
		auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;

        if (elapsedTime.count() >= 1.0) {
			std::chrono::duration<double> Time1 = time2 - time1;
			std::chrono::duration<double> Time2 = time3 - time2;
			std::chrono::duration<double> Time3 = time4 - time3;
		
			std::cout << "get img time: " << Time1.count() << std::endl << "filter img time: " << Time2.count() << std::endl;
			std::cout << "print img time:" << Time3.count() << std::endl;
            double fps = frameCount / elapsedTime.count();
            std::cout << "FPS: " << fps << std::endl <<std::endl;
            frameCount = 0;
            startTime = currentTime;
        }
    }
	
}