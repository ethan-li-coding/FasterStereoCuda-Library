/* -*-c++-*- Copyright (C) 2020. All rights reserved
* Author	: Ethan Li <ethan.li.whu@gmail.com>
* github£º	https://github.com/ethan-li-coding
* blog:		https://ethanli.blog.csdn.net/
* zhihu:	https://www.zhihu.com/people/yingsongli
* bilibili: https://space.bilibili.com/328453794
* Describe	: implement of FasterStereoCuda
*/

#ifndef FASTERSTEREOCUDA_STEREO_H_
#define FASTERSTEREOCUDA_STEREO_H_

#ifndef FASTERSTEREOCUDA_EXPORTS 
#define FASTERSTEREOCUDA_DLL __declspec(dllimport)
#else
#define FASTERSTEREOCUDA_DLL __declspec(dllexport)
#endif


/**
 * \brief faster stereo algorithm based on cuda
 * Besides cuda, other optimization strategies are used, such as hierarchical optimization
 */
class FASTERSTEREOCUDA_DLL FasterStereoCuda
{
public:
	/**\brief parameters of polar image pairs*/
	typedef struct EpipolarStereoParam{
		double x0_left, y0_left;	// image coordinates of principal point(left view)
		double x0_right, y0_right;	// image coordinates of principal point(right view)
		double baseline;			// baseline(mm)
		double focus;				// focal length in pixel units 
		EpipolarStereoParam(){
			x0_left = y0_left = x0_right = y0_right = focus = baseline = 0;
		}
	}epi_ste_t;

	/**\brief parameters of stereo algorithm (for generating disparity map)*/
	typedef struct StereoOption1 {
		int			width;			// width of image
		int			height;			// height of image
		int			min_disp;		// minimum disparity
		int			max_disp;		// maximum disparity
		int			num_layers;		// matching layers(the more layers the faster)

		bool		do_lr_check;	// whether to doing consistency check
		bool		do_rm_peaks;	// whether to removing peaks
		bool		do_smooth;		// whether to smoothing disparity map
		StereoOption1() : width(0), height(0), min_disp(0), max_disp(0), num_layers(1), 
						 do_lr_check(true),do_rm_peaks(true), do_smooth(false) {};
	}ste_opt1_t;

	/**\brief  parameters of stereo algorithm (for generating depth map)*/
	typedef struct StereoOption2 {
		int			width;			// width of image
		int			height;			// height of image
		int			min_depth;		// minimum disparity
		int			max_depth;		// maximum disparity
		int			num_layers;		// matching layers(the more layers the faster)

		bool		do_lr_check;	// whether to doing consistency check
		bool		do_rm_peaks;	// whether to removing peaks
		bool		do_smooth;		// whether to smoothing disparity map

		epi_ste_t	epi;			// parameters of polar image pairs

		StereoOption2(): width(0), height(0), min_depth(0), max_depth(0), num_layers(1),
						 do_lr_check(true), do_rm_peaks(true), do_smooth(false){} ;
	}ste_opt2_t;
public:
	FasterStereoCuda();					
	~FasterStereoCuda();					

	/**
	 * \brief alloc page-locked memory on the host
	 *		  by this way, you can get memory on the host with faster transmission speed between host(CPU) and device(GPU) 
	 * \param ptr	device pointer to allocated memory
	 * \param size	requested allocation size in bytes
	 * \return true: succeed false: filed
	 */
	static bool MallocPageLockedPtr(void** ptr, size_t size);

	/**
	 * \brief frees page-locked memory
	 * \param ptr	pointer to memory to free
	 * \return true: succeed false: filed
	 */
	static bool FreePageLockedPtr(void* ptr);
public:
	/**
	 * \brief initialization 1, for generating disparity map
	 * \param option		in, parameters of algorithm
	 * \return true: succeed false: filed
	 */
	bool Init(const ste_opt1_t& option) const;

	/**
	 * \brief initialization 2, for generating depth map
	 * \param option		in, parameters of algorithm
	 * \return true: succeed false: filed
	 */
	bool Init2(const ste_opt2_t& option) const;

	/**\brief release*/
	void Release() const;

	/**
	 * \brief interface 1 : doing stereo matching to generate disparity map
	 * \param bytes_left		in£¬image data of left view
	 * \param bytes_right 		in£¬image data of right view
	 * \param disparity_data 	out£¬disparity map, the same size as image£¬you mast malloc memory in advance. invalid value : -999999.0f
	 * \return true: succeed false: filed
	 */
	bool Match(const unsigned char* bytes_left, const unsigned char* bytes_right, float* disparity_data) const;

	/**
	 * \brief interface 2 : doing stereo matching to generate depth map
	 * \param bytes_left		in£¬image data of left view
	 * \param bytes_right 		in£¬image data of right view
	 * \param depth_data 		out£¬depth map, the same size as image£¬you mast malloc memory in advance. invalid value : -999999.0f
	 * \return true: succeed false: filed
	 */
	bool Match2(const unsigned char* bytes_left, const unsigned char* bytes_right, float* depth_data) const;

private:
	void* impl_;
};

#endif // FASTERSTEREOCUDA_STEREO_H_