/* -*-c++-*- Copyright (C) 2020. All rights reserved
* Author	: Ethan Li <ethan.li.whu@gmail.com>
* github£º	https://github.com/ethan-li-coding
* blog:		https://ethanli.blog.csdn.net/
* zhihu:	https://www.zhihu.com/people/yingsongli
* bilibili: https://space.bilibili.com/328453794
* Describe	: implement of FasterStereoCuda
*/


#pragma once
#include "FasterStereoCuda.h"
#include <string>
using std::string;

/**\brief class of option manager */
class OptionManager {
public:
	OptionManager();
	~OptionManager();


	/**
	 * \brief type of option
	 *	OPTION_1£ºoption of disparity space
	 *	OPTION_2£ºoption of depth space
	 */ 
	enum OptionType {
		OPTION_1 = 0,
		OPTION_2,
		OPTION_NO
	};

	/**\brief gauss the type of option */
	OptionType GaussType(const string& path);

	/**
	 * \brief loading option from file
	 * \param path	in, option file path
	 * \param opt	out, option
	 * \return true£ºsucceed false£ºfailed
	 */
	bool Load(const string& path, FasterStereoCuda::StereoOption1& opt);

	/**
	 * \brief loading option from file
	 * \param path	in, option file path
	 * \param opt	out, option
	 * \return true£ºsucceed false£ºfailed
	 */
	bool Load(const string& path, FasterStereoCuda::StereoOption2& opt);

private:
	/**\brief save the option */
	bool Save(const string& path, const FasterStereoCuda::StereoOption1& opt);
	/**\brief save the option */
	bool Save(const string& path, const FasterStereoCuda::StereoOption2& opt);
};
