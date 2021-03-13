#include<iostream>
#include<opencv2/opencv.hpp>
#include<io.h>
#include<vector>
#include<cmath>
#include <windows.h>
#include <fstream>
cv::Mat img;
int num = 0;
int n = 0;
int v = 21;
void getFiles(std::string path,std::vector<std::string>& files)
{
    _finddata_t fileInfo;
    std::string p;
    p.assign(path);
    intptr_t handle = _findfirst((p+"//*.jpg").c_str(), &fileInfo);
    do
    {
        std::string name = p + fileInfo.name;
        //std::cout << name<< std::endl;
        files.push_back(name);
    } while (_findnext(handle, &fileInfo) == 0);
    _findclose(handle);
}
void saveMat(cv::Mat &img1)
{
    std::string folderPath = "保存路径";
    std::string png = "jpg";
    folderPath = folderPath + std::to_string(num);
    folderPath.push_back('_');
    folderPath = folderPath + std::to_string(n);
    folderPath.push_back('.');
    folderPath = folderPath + png;
    cv::imwrite(folderPath, img1);
    std::cout <<folderPath<< std::endl;
    n++;
}
void writefile(int a, int b)
{
    std::ofstream outfile;
    outfile.open("保存路径", std::ios::out | std::ios::app);
    outfile <<num<<" "<<n<<"("<<a<<","<<b<<")"<< std::endl;
    outfile.close();
}
void MouseEvent(int event, int x, int y, int flags, void* data)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        cv::Mat hm = img;
        double x1 = x, y1 = y;
        circle(img, cv::Point(x, y), 1, cv::Scalar(0, 0, 255));
        cv::cvtColor(hm, hm, cv::COLOR_RGB2GRAY);
        cv::Mat M(hm.rows, hm.cols, CV_64FC1, cv::Scalar(0, 0, 255));
        for (auto i = 0; i < hm.rows; i++)
        {
            for (auto j = 0; j < hm.cols; j++)
            {
                double a1 = i, b1 = j;
                double u = (a1-x1)*(a1-x1)+(b1-y1)*(b1-y1);
                u = -u / 2 / v / v;
                double e = std::exp(u);
                M.at<double>(i, j) = e*100;
                /*std::cout << u << "******" << e;
                return;*/
            }
        }
        saveMat(M);
        writefile(x, y);
    }
}
int main()
{
	std::vector<std::string>filename;
	std::string imgpath = "图片路径";
	getFiles(imgpath, filename);
    for (auto i : filename)
    {
        std::cout << i << std::endl;
        std::cout << num << std::endl;
        img = cv::imread(i);
        cv::resize(img, img, cv::Size(1008,756));
        cv::namedWindow("img");
        cv::imshow("img", img);
        cv::setMouseCallback("img", MouseEvent);
        while (1) 
        { if (cv::waitKey(0) == 27)
            break; 
        }
        cv::destroyAllWindows();
        num++;
        n = 0;
    }
    return 0;
}