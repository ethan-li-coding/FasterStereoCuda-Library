#pragma once

#include <vector>
using std::vector;

#define INVALID_VALUE		-999999.0f

typedef float	 float32;	//32位单精度浮点数
typedef double   float64;	//64位双精度浮点数
typedef uint8_t	 uint8;		//无符号8位整数
typedef uint16_t uint16;	//无符号16位整数
typedef uint32_t uint32;	//无符号32位整数
typedef uint64_t uint64;	//无符号64位整数
typedef int8_t   sint8;		//有符号8位整数
typedef int16_t  sint16;	//有符号16位整数
typedef int32_t  sint32;	//有符号32位整数
typedef int64_t  sint64;	//有符号64位整数

#ifndef STEREO_SGM_PARAM_T
#define STEREO_SGM_PARAM_T
class CuSGMOption {
public:
	CuSGMOption() : p1(8), p2_init(32), p2_max(0), cs_type(CS_5x5), num_paths(0), do_median_filter(false), do_lr_check(false),
		lr_threshold(0), unique_threshold(0), using_constant_p2(false), do_remove_peaks(false),
		peaks_ratio_threshold(0), post_filter_type(PF_NONE), morphology_type(MP_NONE) {};

	~CuSGMOption() = default;

	// census transform type
	enum CS_Type {
		CS_5x5 = 0,		// 0-5x5
		CS_9x7			// 1-9x7
	};

	// post filter type
	enum PF_Type {
		PF_NONE = 0,	// 0-NONE
		PF_GAUSS,		// 1-gaussian filter
		PF_BILATERAL	// 2-bilateral filter
	};

	// morphology type
	enum MP_Type {
		MP_NONE = 0,	// 0-NONE
		MP_EROSION,		// 1-Erosion 
		MP_DILATION,	// 2-Dilation
		MP_OPEN,		// 3-Opening 
		MP_CLOSE,		// 4-Closing
	};

	sint32		p1;
	sint32		p2_init;
	sint32		p2_max;
	CS_Type		cs_type;
	sint32		num_paths;
	bool		do_median_filter;
	bool		do_lr_check;
	float32		lr_threshold;
	float32		unique_threshold;
	bool		using_constant_p2;
	bool		do_remove_peaks;
	float32		peaks_ratio_threshold;
	PF_Type		post_filter_type;
	MP_Type		morphology_type;
};
#endif

#ifndef STEREO_CAMERA_PARAM_T
#define STEREO_CAMERA_PARAM_T
typedef struct CameraParam_T {
	float64 x0_left, y0_left;
	float64 x0_right, y0_right;
	float64 focus;
	float64 baseline;

	CameraParam_T Scale(const int& s) const {
		CameraParam_T cam{};
		cam.x0_left = x0_left / s;		cam.y0_left = y0_left / s;
		cam.x0_right = x0_right / s;	cam.y0_right = y0_right / s;
		cam.focus = focus / s;
		cam.baseline = baseline;
		return cam;
	}
}CamParam_T;
#endif

#ifndef STEREO_ROI_T
#define STEREO_ROI_T
struct StereoROI_T
{
	sint32 x, y, w, h;
};
#endif