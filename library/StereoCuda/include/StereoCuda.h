#pragma once
#include "sgm_types.h"

#ifdef STEREO_CUDA_EXPORT
#define STEREO_CUDA_DLL __declspec(dllexport)
#else
#define STEREO_CUDA_DLL /*__declspec(dllimport)*/
#endif

class STEREO_CUDA_DLL StereoCuda
{
public:
	StereoCuda();
	~StereoCuda();

public:
	/**
	 * \brief 初始化
	 * \param width				影像宽
	 * \param height			影像高
	 * \param min_disparity		最小视差值
	 * \param disp_range		视差范围
	 * \param sgm_option		SGM参数
	 * \param print_log			是否打印log信息
	 * \return true-成功 false-失败
	 */
	bool Init(sint32 width, sint32 height, sint32 min_disparity, sint32 disp_range, CuSGMOption sgm_option,bool print_log = false) const;
	
	/**
	 * \brief 匹配
	 * \param img_left			左影像数据
	 * \param img_right			右影像数据
	 * \param disp_left			输出左视差图，尺寸与影像尺寸一致，需预先分配内存
	 * \param disp_right		输出右视差图，尺寸与影像尺寸一致，需预先分配内存
	 * \param init_disp_left	初始视差值数组，若为nullptr，则不采用初始视差值
	 * \param ste_roi_left		左影像ROI
	 * \param ste_roi_right		右影像ROI
	 * \return 
	 */
	bool Match(uint8* img_left, uint8* img_right, float32* disp_left, float32* disp_right = nullptr, sint16* init_disp_left = nullptr, StereoROI_T* ste_roi_left = nullptr, StereoROI_T* ste_roi_right = nullptr) const;

	/**
	 * \brief 初始化2
	 * \param width				影像宽			
	 * \param height 			影像高			
	 * \param min_disparity 	最小视差值
	 * \param disp_range 		视差范围		
	 * \param sgm_option 		SGM参数		
	 * \param cam_param 		相机参数		
	 * \param print_log 		是否打印log信息
	 * \return 
	 */
	bool Init2(sint32 width, sint32 height, sint32 min_disparity, sint32 disp_range, CuSGMOption sgm_option, CamParam_T cam_param, bool print_log = false) const;

	/**
	 * \brief 匹配2
	 * \param img_left			左影像数据		
	 * \param img_right 		右影像数据		
	 * \param depth_left 		左影像深度图，尺寸与影像尺寸一致，需预先分配内存
	 * \param init_disp_left 	左影像初始视差值数组，若为nullptr，则不采用初始视差值
	 * \param ste_roi_left		左影像ROI
	 * \param ste_roi_right		右影像ROI
	 * \return true-成功 false-失败
	 */
	bool Match2(uint8* img_left, uint8* img_right, float32* depth_left, sint16* init_disp_left = nullptr, StereoROI_T* ste_roi_left = nullptr, StereoROI_T* ste_roi_right = nullptr) const;

	/**\brief 释放内存 */
	void Release() const;

	/**
	 * \brief 获取视差图的ROI区
	 * \param disp_ptr			视差图指针
	 * \param width				视差图宽
	 * \param height			视差图高
	 * \param ste_roi			输出的ROI数据
	 */
	static void GetRoiFromDispMap(float32* disp_ptr, sint32 width, sint32 height, StereoROI_T& ste_roi);

	/**\brief 获取无效值 */
	static float32 get_invad_float();
	static sint16 get_invad_short();

	/**\brief 获取金字塔匹配模式下除最高层之外的所有层的视差搜索范围 */
	static sint16 get_level_range();

	/**\brief 在主机端分配锁页内存（传输更快） */
	static bool MallocPageLockedPtr(void** ptr, size_t size);

	/**\brief 释放主机端锁页内存 */
	static bool FreePageLockedPtr(void* ptr);
private:
	void* impl_;
};