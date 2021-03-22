#define _WIN32_WINNT 0x0500
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include<atltime.h>
#include <ctime>
#include "json.hpp"
#include <Windows.h>

using json = nlohmann::json;
using namespace cv;
using namespace std;


void WriteLog(const char * szLog)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	char filename[] = "log.txt";
	fstream fp;
	fp.open(filename, ios::app);
	CTime logtime = CTime::GetCurrentTime();
	CString t = logtime.Format("%Y-%m-%d-%H-%M-%S");
	fp <<"[Info]: " << t << endl;
	fp << szLog << endl;
	fp.close();
	
}
//320*320 R15fps W15fps/2
int writer(string camera, string fileName, string windowName, int opentime, int closetime) {
	bool cameraOn = false;
	WriteLog("[Event]: Ready to open the camera...\n");
	while (true){
		CTime ot = CTime::GetCurrentTime();
		CString open = ot.Format("%H%M");
		string o = (string)open;
		int now = atoi(o.c_str());
		if (closetime > opentime) {
			if (now >= opentime && now < closetime) {
				cameraOn = true;
				break;
			}
		}else if (closetime < opentime) {
			if (!(now >= closetime && now < opentime)) {
				cameraOn = true;
				break;
			}
		}
	}
	WriteLog("[Event]: Open the camera...\n");
	//初始化攝影機
	while (cameraOn) {
		Mat image;
		Mat src;
		Mat err;
		VideoCapture capture;
		VideoCapture error;
		error.open("test.jpg");
		VideoWriter writer;
		int codec = VideoWriter::fourcc('M', 'P', '4', 'V');
		double fps = 15.0;
		int cnt = 0;
		Size sizeFrame(320, 320);
		string cacheName = "cache/" + fileName + ".mp4";
		writer.open(cacheName, codec, fps, sizeFrame);

		clock_t start, now;
		start = clock();
		//錄影
		bool record = true;
		while (record) {
			capture.open(camera);
			while (true)
			{
				now = clock();
				double stop = (now - start) / (double)CLOCKS_PER_SEC;
				if (stop >= 1800.0) {
					record = false;
					break;
				}
				capture >> image;
				if (!image.empty()) {
					Mat xframe;
					resize(image, xframe, sizeFrame);
					cnt += 1;
					//每2frames寫入一次
					if (cnt % 2 == 0)
						writer.write(xframe);
					imshow(windowName, xframe);
					char c = (char)waitKey(1);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
				}
				else {
					capture.release();
					error >> err;
					cnt += 1;
					//每2frames寫入一次
					//if (cnt % 2 == 0)
					writer.write(err);
					imshow(windowName, err);
					char c = (char)waitKey(66);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
					break;
				}

			}

		}
		capture.release();
		error.release();
		writer.release();
		WriteLog("[Event]: Save the video file...\n");
		//取得目前時間
		CTime endtime = CTime::GetCurrentTime();
		CString date = endtime.Format("%Y-%m-%d-%H-%M-%S");
		string outputName = "output/" + fileName + "-" + (string)date + ".mp4";
		//檔案重新命名及搬移
		rename(cacheName.c_str(), outputName.c_str());
		WriteLog("[Event]: Move the video file completed\n");
	}

	return 0;
}
//640*480 R20fps W20fps/2
int writer_20fps(string camera, string fileName, string windowName, int opentime, int closetime) {
	bool cameraOn = false;
	WriteLog("[Event]: Ready to open the camera...\n");
	while (true) {
		CTime ot = CTime::GetCurrentTime();
		CString open = ot.Format("%H%M");
		string o = (string)open;
		int now = atoi(o.c_str());
		if (closetime > opentime) {
			if (now >= opentime && now < closetime) {
				cameraOn = true;
				break;
			}
		}
		else if (closetime < opentime) {
			if (!(now >= closetime && now < opentime)) {
				cameraOn = true;
				break;
			}
		}
	}
	WriteLog("[Event]: Open the camera...\n");
	//初始化攝影機
	while (cameraOn) {
		Mat image;
		Mat src;
		Mat err;
		VideoCapture capture;
		VideoCapture error;
		error.open("test.jpg");
		VideoWriter writer;
		int codec = VideoWriter::fourcc('M', 'P', '4', 'V');
		double fps = 20.0;
		int cnt = 0;
		Size sizeFrame(640, 480);
		string cacheName = "cache/" + fileName + ".mp4";
		writer.open(cacheName, codec, fps, sizeFrame);

		clock_t start, now;
		start = clock();
		//錄影
		bool record = true;
		while (record) {
			capture.open(camera);
			while (true)
			{
				now = clock();
				double stop = (now - start) / (double)CLOCKS_PER_SEC;
				if (stop >= 1800.0) {
					record = false;
					break;
				}
				capture >> image;
				if (!image.empty()) {
					Mat xframe;
					resize(image, xframe, sizeFrame);
					cnt += 1;
					if (cnt % 2 == 0)
						writer.write(xframe);
					imshow(windowName, xframe);
					char c = (char)waitKey(1);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
				}
				else {
					capture.release();
					error >> err;
					cnt += 1;
					writer.write(err);
					imshow(windowName, err);
					char c = (char)waitKey(66);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
					break;
				}
			}
		}
		capture.release();
		error.release();
		writer.release();
		WriteLog("[Event]: Save the video file...\n");
		//取得目前時間
		CTime endtime = CTime::GetCurrentTime();
		CString date = endtime.Format("%Y-%m-%d-%H-%M-%S");
		string outputName = "output/" + fileName + "-" + (string)date + ".mp4";
		//檔案重新命名及搬移
		rename(cacheName.c_str(), outputName.c_str());
		WriteLog("[Event]: Move the video file completed\n");
	}

	return 0;
}
//640*480 R10fps W20fps
int writer_10fps(string camera, string fileName, string windowName, int opentime, int closetime) {
	bool cameraOn = false;
	WriteLog("[Event]: Ready to open the camera...\n");
	while (true) {
		CTime ot = CTime::GetCurrentTime();
		CString open = ot.Format("%H%M");
		string o = (string)open;
		int now = atoi(o.c_str());
		if (closetime > opentime) {
			if (now >= opentime && now < closetime) {
				cameraOn = true;
				break;
			}
		}
		else if (closetime < opentime) {
			if (!(now >= closetime && now < opentime)) {
				cameraOn = true;
				break;
			}
		}
	}
	WriteLog("[Event]: Open the camera...\n");
	//初始化攝影機
	while (cameraOn) {
		Mat image;
		Mat src;
		Mat err;
		VideoCapture capture;
		VideoCapture error;
		error.open("test.jpg");
		VideoWriter writer;
		int codec = VideoWriter::fourcc('M', 'P', '4', 'V');
		double fps = 20.0;
		int cnt = 0;
		Size sizeFrame(640, 480);
		string cacheName = "cache/" + fileName + ".mp4";
		writer.open(cacheName, codec, fps, sizeFrame);

		clock_t start, now;
		start = clock();
		//錄影
		bool record = true;
		while (record) {
			capture.open(camera);
			while (true)
			{
				now = clock();
				double stop = (now - start) / (double)CLOCKS_PER_SEC;
				if (stop >= 1800.0) {
					record = false;
					break;
				}
				capture >> image;
				if (!image.empty()) {
					Mat xframe;
					resize(image, xframe, sizeFrame);
					cnt += 1;
					
					writer.write(xframe);
					imshow(windowName, xframe);
					char c = (char)waitKey(1);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
				}
				else {
					capture.release();
					error >> err;
					cnt += 1;
					writer.write(err);
					imshow(windowName, err);
					char c = (char)waitKey(66);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
					break;
				}
			}
		}
		capture.release();
		error.release();
		writer.release();
		WriteLog("[Event]: Save the video file...\n");
		//取得目前時間
		CTime endtime = CTime::GetCurrentTime();
		CString date = endtime.Format("%Y-%m-%d-%H-%M-%S");
		string outputName = "output/" + fileName + "-" + (string)date + ".mp4";
		//檔案重新命名及搬移
		rename(cacheName.c_str(), outputName.c_str());
		WriteLog("[Event]: Move the video file completed\n");
	}

	return 0;
}
//640*480 R15fps W15fps
int writer_15fps(string camera, string fileName, string windowName, int opentime, int closetime) {
	bool cameraOn = false;
	WriteLog("[Event]: Ready to open the camera...\n");
	while (true) {
		CTime ot = CTime::GetCurrentTime();
		CString open = ot.Format("%H%M");
		string o = (string)open;
		int now = atoi(o.c_str());
		if (closetime > opentime) {
			if (now >= opentime && now < closetime) {
				cameraOn = true;
				break;
			}
		}
		else if (closetime < opentime) {
			if (!(now >= closetime && now < opentime)) {
				cameraOn = true;
				break;
			}
		}
	}
	WriteLog("[Event]: Open the camera...\n");
	//初始化攝影機
	while (cameraOn) {
		Mat image;
		Mat src;
		Mat err;
		VideoCapture capture;
		VideoCapture error;
		error.open("test.jpg");
		VideoWriter writer;
		int codec = VideoWriter::fourcc('H', '2', '6', '4');
		double fps = 15.0;
		int cnt = 0;
		Size sizeFrame(640, 480);
		string cacheName = "cache/" + fileName + ".mp4";
		writer.open(cacheName, codec, fps, sizeFrame);

		clock_t start, now;
		start = clock();
		//錄影
		bool record = true;
		while (record) {
			capture.open(camera);
			while (true)
			{
				now = clock();
				double stop = (now - start) / (double)CLOCKS_PER_SEC;
				if (stop >= 1800.0) {
					record = false;
					break;
				}
				capture >> image;
				if (!image.empty()) {
					Mat xframe;
					resize(image, xframe, sizeFrame);
					cnt += 1;
					writer.write(xframe);
					imshow(windowName, xframe);
					char c = (char)waitKey(1);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
				}
				else {
					capture.release();
					error >> err;
					cnt += 1;
					writer.write(err);
					imshow(windowName, err);
					char c = (char)waitKey(66);
					if (c == 27) {
						record = false;
						cameraOn = false;
						break;
					}
					break;
				}
			}
		}
		capture.release();
		error.release();
		writer.release();
		WriteLog("[Event]: Save the video file...\n");
		//取得目前時間
		CTime endtime = CTime::GetCurrentTime();
		CString date = endtime.Format("%Y-%m-%d-%H-%M-%S");
		string outputName = "output/" + fileName + "-" + (string)date + ".mp4";
		//檔案重新命名及搬移
		rename(cacheName.c_str(), outputName.c_str());
		WriteLog("[Event]: Move the video file completed\n");
	}

	return 0;
}

/*
int test(string camera, string filename) {
	//cout << "Built with OpenCV " << CV_VERSION << endl;
	Mat image;
	Mat src;
	VideoCapture capture;
	capture.open(camera);
	capture >> src;
	bool isColor = (src.type() == CV_8UC3);
	VideoWriter writer;
	int codec = VideoWriter::fourcc('M', 'P', '4', 'V');
	double fps = 15.0;

	Size sizeFrame(320, 320);
	writer.open(filename, codec, fps, sizeFrame, isColor);
	cout << "Started writing video... " << endl;
	clock_t start, now;

	if (!capture.isOpened()) {
		cout << "camera is unconnect" << endl;
		return -1;
	}
	start = clock();
	int cnt = 0;
	for (;;)
	{
		now = clock();
		double stop = (now - start) / (double)CLOCKS_PER_SEC;
		if (stop >= 1800.0) break;

		capture >> image;
		Mat xframe;
		resize(image, xframe, sizeFrame);
		cnt += 1;
		//每2frames寫入一次
		if (cnt % 2 == 0)
			writer.write(xframe);
		imshow("Sample", xframe);
		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	cout << "Write complete !" << endl;
	capture.release();
	writer.release();
	return 0;
}*/

int main()
{
	HWND hWnd = GetConsoleWindow(); //隱藏視窗1
	ShowWindow(hWnd, SW_HIDE); //隱藏視窗2
	ifstream fin("json/json_test.json");   
	json j;
	fin >> j;
	fin.close();
	/*"fCid": 10,
        "fSid": 4,
        "fTaxID": "06112131",
        "fComputerID": null,
        "fName": "test01",
        "fNumber": "admin",
        "fPwd": "123",
        "fIpAddress": "192.168.0.188:554",
        "StartTime": "08:00",
        "EndTime": "18:00",
        "fCreateDate": "20200204133633",
        "fEditDate": "20200204153747",
        "fEditUser": "Admin",
        "fIsPublish": 0*/
	
	WriteLog("[Event]: Loading camera imformation...\n");
	string taxID = j["fTaxID"];
	string cAcc = j["fNumber"];			//camera account
	string cPwd = j["fPwd"];			//camera password
	string ipAddress = j["fIpAddress"];	//camera ip address
	string opt = j["StartTime"];	//time to open the camera
	string clt = j["EndTime"];	//time to close the camera
	int opentime = atoi(opt.c_str());
	int closetime = atoi(clt.c_str());
	
	string camera = "rtsp://"+cAcc+":"+cPwd+"@"+ipAddress+"/profile2";
	int finsh = writer_15fps(camera, cAcc, taxID, opentime, closetime);
	return finsh;
	//return 0;
}