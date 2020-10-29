/* -*-c++-*- Copyright (C) 2020. All rights reserved
* Author	: Ethan Li <ethan.li.whu@gmail.com>
* github£º	https://github.com/ethan-li-coding
* blog:		https://ethanli.blog.csdn.net/
* zhihu:	https://www.zhihu.com/people/yingsongli
* bilibili: https://space.bilibili.com/328453794
* Describe	: implement of FasterStereoCuda
*/

#include <io.h>
#include <chrono>
using namespace std::chrono;

#include <opencv.hpp>
using namespace cv;

#include <vector>
using namespace std;

#include "FasterStereoCuda.h"
#ifdef _DEBUG
#pragma comment(lib,"opencv_world320d.lib")
#pragma comment(lib, "FasterStereoCudad.lib")
#else
#pragma comment(lib,"opencv_world320.lib")
#pragma comment(lib, "FasterStereoCuda.lib")
#endif

#include "option_manager.h"

// using page locked memory
#define USING_PAGE_LOCKED_MEMORY

void LoadDatas(const string& path_left,const string& path_right,const string& file_type, vector<Mat>& left_mats,vector<Mat>& right_mats)
{
	//Load all image pairs in specified folder
	for (int i = 0; i < 2; i++) {
		auto& path = (i == 0) ? path_left : path_right;
		auto& mats = (i == 0) ? left_mats : right_mats;
		vector<string> str_paths;
		_finddata_t file;
		std::string str_root = path;
		if (str_root[str_root.length() - 1] != '\\')
		{
			str_root.append("\\");
		}
		std::string search_name = str_root;
		search_name.append("*." + file_type);
		intptr_t k;
		intptr_t HANDLE;
		k = HANDLE = _findfirst(search_name.c_str(), &file);
		while (k != -1)
		{
			str_paths.push_back(str_root + file.name);
			k = _findnext(HANDLE, &file);
		}
		for (int n = 0; n < str_paths.size(); n++) {
			mats.emplace_back(imread(str_paths[n], IMREAD_GRAYSCALE));
		}
	}
}

void LoadDatas(const string& path_left, const string& path_right, vector<Mat>& left_mats, vector<Mat>& right_mats)
{
	//Load single image pairs
	left_mats.emplace_back(imread(path_left, IMREAD_GRAYSCALE));
	right_mats.emplace_back(imread(path_right, IMREAD_GRAYSCALE));
}

/**
 * \brief mode1(single pairs): im2.png im6.png option.xml
 *		  mode2(multiple pairs): folder1 folder2 png option.xml
 *		  mode3: opt
 */
int main(int argc, char** argv)
{
	if (argc < 2){
		cout << "insufficient parameters!" << endl;
		return 0;
	}

	string data_type;
	string	path_left,
			path_right;
	string opt_path;
	string file_type;

	if (argc < 4) {
		cout << "insufficient parameters!" << endl;
		return 0;
	}

	if (argc == 4) {
		// mode 1 : single pairs
		path_left = argv[1];
		path_right = argv[2];
		opt_path = argv[3];
		data_type = "S";
	}
	else if(argc == 5) {
		// mode 2 : multiple pairs
		path_left = argv[1];
		path_right = argv[2];
		file_type = argv[3];
		opt_path = argv[4];
		data_type = "M";
	}
	else if(argc == 2 && argv[1] == "opt"){
		// mode 3 :initialize parameter file for algorithm
		FasterStereoCuda::StereoOption1 opt1;
		FasterStereoCuda::StereoOption2 opt2;
		OptionManager opt_mag;
		opt_mag.Load("option.xml", opt1);
		opt_mag.Load("option2.xml", opt2);
		cout << "initialize parameter file of algorithm done!" << endl;
		return 0;
	}
	else {
		cout << "parameter type error!" << endl;
		return 0;
	}


	/********************************************************/
	/*loading parameters of algorithm from xml file*/
	FasterStereoCuda::StereoOption1* ste_opt1 = nullptr;
	FasterStereoCuda::StereoOption2* ste_opt2 = nullptr;
	OptionManager opt_mag;
	OptionManager::OptionType opt_type = opt_mag.GaussType(opt_path);
	if(opt_type == OptionManager::OPTION_1) {
		ste_opt1 = new FasterStereoCuda::StereoOption1;
		if(!opt_mag.Load(opt_path, *ste_opt1)) {
			cout << "loading parameters of algorithm succeed!" << endl;
			delete ste_opt1; ste_opt1 = nullptr;
			return 0;
		}
	}
	else if(opt_type == OptionManager::OPTION_2) {
		ste_opt2 = new FasterStereoCuda::StereoOption2;
		if(!opt_mag.Load(opt_path, *ste_opt2)) {
			cout << "loading parameters of algorithm failed!" << endl;
			delete ste_opt2; ste_opt2 = nullptr;
			return 0;
		}
	}
	else {
		cout << "loading parameters of algorithm failed!" << endl;
		return 0;
	}
	
	/********************************************************/
	/*data*/
	vector<Mat> left_mats;
	vector<Mat> right_mats;
	if (data_type == "S") {
		// loading single image pair
		LoadDatas(path_left, path_right, left_mats, right_mats);
	}
	else if (data_type == "M") {
		// loading multiple image pairs from folder
		LoadDatas(path_left, path_right, file_type, left_mats, right_mats);
	}
	if(left_mats.empty()||right_mats.empty()||(left_mats.size()!=right_mats.size())) {
		cout << "data error!" << endl;
		return 0;
	}

	/********************************************************/
	/*initialize algorithm*/
	const int width = left_mats[0].cols;
	const int height = right_mats[0].rows;
	FasterStereoCuda stereo;
	if (ste_opt1 != nullptr) {
		// interface 1 : for generating disparity map
		if (!stereo.Init(*ste_opt1)) {
			return 0;
		}
	}
	else if(ste_opt2 != nullptr) {
		// interface 1 : for generating depth map
		if (!stereo.Init2(*ste_opt2)) {
			return 0;
		}
	}
	cout << "initialize algorithm done!" << endl;

	/********************************************************/
	/*allocate memory of data*/
#ifdef USING_PAGE_LOCKED_MEMORY
	// using page locked memory
	unsigned char* img_left = nullptr;
	unsigned char* img_right = nullptr;
	float* d_data = nullptr;
	FasterStereoCuda::MallocPageLockedPtr(reinterpret_cast<void**>(&img_left), width * height * sizeof(unsigned char));
	FasterStereoCuda::MallocPageLockedPtr(reinterpret_cast<void**>(&img_right), width * height * sizeof(unsigned char));
	FasterStereoCuda::MallocPageLockedPtr(reinterpret_cast<void**>(&d_data), width * height * sizeof(float));
#else
	unsigned char* img_left = new unsigned char[width*height];
	unsigned char* img_right = new unsigned char[width*height];
	float* d_data = new float[width*height];
#endif

	/********************************************************/
	/*set windows of results*/
	namedWindow("left", WINDOW_NORMAL);
	namedWindow("right", WINDOW_NORMAL);
	namedWindow("d_map", WINDOW_NORMAL);
	namedWindow("d_map_color", WINDOW_NORMAL); 
	double scale = width / 640.0;
	resizeWindow("left", int(width / scale), int(height / scale));
	resizeWindow("right", int(width / scale), int(height / scale));
	resizeWindow("d_map", int(width / scale), int(height / scale));
	resizeWindow("d_map_color", int(width / scale), int(height / scale));

	/********************************************************/
	/*matching(for loop)*/
	int time_count = 0;
	double time_all = 0.0, time_avg = 0.0, time_first = 0.0;
	for (int i = 0; i < left_mats.size(); ) {
		auto& mat_left = left_mats[i];
		auto& mat_right = right_mats[i];

		for (int i = 0; i < height;i++) {
			for (int j = 0; j < width;j++) {
				img_left[i*width + j] = mat_left.at<unsigned char>(i, j);
				img_right[i*width + j] = mat_right.at<unsigned char>(i, j);
			}
		}

		int validCount = 0;
		auto start = std::chrono::steady_clock::now();

		/********************************************************/
		/*do matching*/
		if (ste_opt1 != nullptr) {
			// interface 1 : generating disparity map
			stereo.Match(img_left, img_right, d_data);
		}
		else if(ste_opt2 != nullptr) {
			// interface 2 : generating depth map
			stereo.Match2(img_left, img_right, d_data);
		}

		/********************************************************/
		/*compute time consuming and frame*/
		auto end = std::chrono::steady_clock::now();
		auto tt = duration_cast<std::chrono::microseconds>(end - start);
		double time_level = tt.count()/1000.0;
		time_all += time_level;
		if (time_count==0)
			time_first = time_level;
		time_count++;
		if (time_count > 1) {
			time_avg = (time_all - time_first) / (time_count - 1);
		}
		else {
			time_avg = time_level;
		}
		printf("frame %d, timing: %6.4lf ms",i, time_level);

		/********************************************************/
		/*results visualization*/
		Mat disp_mat = Mat(height, width, CV_8UC1);
		Mat disp_color;
		float min_d = 999999.0f, max_d = -999999.0f;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				const float d = d_data[i * width + j];
				if (d != -999999.0f) {
					min_d = min(min_d, d);
					max_d = max(max_d, d);
				}
			}
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				const float d = d_data[i * width + j];
				if (d == -999999.0f) {
					disp_mat.data[i * width + j] = 0;
				}
				else {
					disp_mat.data[i * width + j] = static_cast<uchar>((d - min_d) / (max_d - min_d) * 255);
				}
			}
		}
		double fps = int(1000.0 / time_avg * 10 + 0.5) / 10.0;
		printf(" fps = %6.4lf\n", fps);
		applyColorMap(disp_mat, disp_color, COLORMAP_JET);
		imshow("left", mat_left);
		imshow("right", mat_right);
		imshow("d_map", disp_mat);
		imshow("d_map_color", disp_color);
		cvWaitKey(1);

#if 0
		// save the disparity(depth) map if necessary
		imwrite("d.bmp", disp_mat);
#endif

		// loop
		i = ++i % left_mats.size();
	}

	/********************************************************/
	/*release the memory*/
	stereo.Release();
	if(ste_opt1) {
		delete ste_opt1; ste_opt1 = nullptr;
	}
	if(ste_opt2) {
		delete ste_opt2; ste_opt2 = nullptr;
	}
#ifdef USING_PAGE_LOCKED_MEMORY
	FasterStereoCuda::FreePageLockedPtr(img_left);
	FasterStereoCuda::FreePageLockedPtr(img_right);
	FasterStereoCuda::FreePageLockedPtr(d_data);
#else
	delete[] d_data; d_data = nullptr;
	delete[] img_left; img_left = nullptr;
	delete[] img_right; img_right = nullptr;
#endif

	system("pause");
	return 0;
}

