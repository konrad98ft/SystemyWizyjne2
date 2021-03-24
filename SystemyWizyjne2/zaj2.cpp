#include <iostream>
#include<opencv2/opencv.hpp>
#define STEP 8
#define IMG_PATH "lena.jpg"
#define BAYER_PATH "edinburgh_bayer.png"


#if STEP==8
struct SDaneObrazy
{
	cv::Mat imgIn, imgOut;

};


inline double dystans(cv::Vec3b p1, cv::Vec3b p2)
{
	return cv::norm(cv::Scalar(p1) - cv::Scalar(p2));
}

void onMouse(int event, int x, int y, int flags, void* userdata) //przyk�ad akcji po klikni�ciu 
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		SDaneObrazy* param = (SDaneObrazy*)userdata;
		param->imgOut = param->imgIn.clone();
		cv::Mat* pimg = (cv::Mat*)param;

		std::cout << pimg->at<cv::Vec3b>(y, x) << std::endl;
		cv::Mat_<cv::Vec3b> tmp(param->imgOut);
		cv::Vec3b& clicked_p = tmp(y, x);

		for (int y = 0; y < tmp.rows; ++y)
		{
			for (int x = 0; x < tmp.cols; ++x)
			{
				cv::Vec3b& p = tmp(y, x);
				if (dystans(p, clicked_p) < 20)
				{
					p[0] = 255;
					p[1] = 0;
					p[2] = 0;
				}
			}
		}
		cv::imshow("Test", param->imgOut);
	}
}


#endif

#if STEP==5
void filtr(cv::Mat& img, int prog)
{
	for (int y = 0; y < img.rows; ++y)
	{
		for (int x = 0; x < img.cols; ++x)
		{
			cv::Vec3b& p = img.at<cv::Vec3b>(y, x);
			if (p[0] + p[1] + p[2] < prog)
			{
				p[0] = 0;
				p[1] = 0;
				p[2] = 0;
				//2: p=0; prostsza wersja;
			}
		}
	}
}
#endif

#if STEP==6
void filtr(cv::Mat& img, int prog)
{
	cv::Mat_<cv::Vec3b> tmp(img);
	for (int y = 0; y < tmp.rows; ++y)
	{
		for (int x = 0; x < tmp.cols; ++x)
		{
			cv::Vec3b& p = tmp(y, x);
			if (p[0] + p[1] + p[2] < prog)
			{
				p[0] = 0;
				p[1] = 0;
				p[2] = 0;
				//2: p=0; prostsza wersja;
			}
		}
	}
}
#endif

#if STEP==7
void filtr(cv::Mat& img, int prog)
{
	for (int y = 0; y < img.rows; ++y)
	{
		uchar* ptr = img.data + y * img.step;
		for (int x = 0; x < img.cols; ++x, ptr += 3)
		{
			if (ptr[0] + ptr[1] + ptr[2] < prog)
			{
				ptr[0] = 0;
				ptr[1] = 0;
				ptr[2] = 0;
			}
		}
	}
}
#endif

int main()
{
#if STEP==0
	cv::Mat loadedImg = cv::imread(IMG_PATH, 1);
	cv::imshow("Output", loadedImg);
	std::cout << "Liczba kanalow: " << loadedImg.channels() << std::endl;
	cv::waitKey();
#endif
#if STEP==1
	cv::Mat loadedImg = cv::imread(IMG_PATH);
	cv::Mat outImg;
	//cv::cvtColor(loadedImg, outImg, cv::COLOR_BGR2GRAY);
	cv::cvtColor(loadedImg, outImg, cv::COLOR_BGR2HSV);
	std::cout << "Liczba kanalow wejscia: " << loadedImg.channels() << " Liczba kanalow wyjscia: " << outImg.channels() << std::endl;
	cv::imshow("Loaded", loadedImg);
	cv::imshow("Output", outImg);
	cv::waitKey();
#endif
#if STEP==2
	cv::Mat loadedImg = cv::imread(BAYER_PATH, 0);
	cv::Mat outImg;
	cv::cvtColor(loadedImg, outImg, cv::COLOR_BayerBG2BGR);
	std::cout << "Liczba kanalow wejscia: " << loadedImg.channels() << " Liczba kanalow wyjscia: " << outImg.channels() << std::endl;
	cv::imshow("Loaded", loadedImg);
	cv::imshow("Output", outImg);
	cv::waitKey();
#endif
#if STEP==3 || STEP==4
	cv::Mat loadedImg = cv::imread(IMG_PATH);
	std::vector<cv::Mat>  wektorImg;
	cv::split(loadedImg, wektorImg);
	cv::imshow("Loaded", loadedImg);
	cv::imshow("Kanal B", wektorImg[0]);
	cv::imshow("Kanal G", wektorImg[1]);
	cv::imshow("Kanal R", wektorImg[2]);
	cv::waitKey();
#if STEP==4
	cv::destroyAllWindows(); //zamykamy poprzednie okna
	cv::circle(wektorImg[2], cv::Point(loadedImg.rows / 2, loadedImg.cols / 2), 50, 255, -1); //warto�� -1 w argumencie thickness oznacza okr�g wype�niony
	cv::Mat outImg;
	cv::Mat swap;
	swap = wektorImg[0];
	wektorImg[0] = wektorImg[1];
	wektorImg[1] = swap;
	cv::imshow("Kanal B", wektorImg[0]);
	cv::imshow("Kanal G", wektorImg[1]);
	cv::imshow("Kanal R", wektorImg[2]);
	cv::merge(wektorImg, outImg);
	cv::imshow("Output", outImg);
	cv::waitKey();
#endif
#endif
#if STEP==5 || STEP==6 || STEP==7
	cv::Mat loadedImg = cv::imread(IMG_PATH);
	int64 startTime, stopTime;
	startTime = cv::getTickCount();
	filtr(loadedImg, 300);
	stopTime = cv::getTickCount();
	std::cout << "Czas wykonania " << (stopTime - startTime) * 1000 / cv::getTickFrequency() << " ms" << std::endl;
	cv::imshow("Output", loadedImg);
	cv::waitKey();
#endif
#if STEP==8
	cv::Mat loadedImg = cv::imread(IMG_PATH);
	cv::imshow("Test", loadedImg);
	SDaneObrazy param = { loadedImg, loadedImg.clone()};
	cv::setMouseCallback("Test", onMouse, &param);
	cv::waitKey();
#endif


}