/* -*-c++-*- Copyright (C) 2020. All rights reserved
* Author	: Ethan Li <ethan.li.whu@gmail.com>
* github£º	https://github.com/ethan-li-coding
* blog:		https://ethanli.blog.csdn.net/
* zhihu:	https://www.zhihu.com/people/yingsongli
* bilibili: https://space.bilibili.com/328453794
* Describe	: implement of FasterStereoCuda
*/


#include "option_manager.h"
#include "tinyxml2.h"
#include <corecrt_io.h>

OptionManager::OptionManager() {}
OptionManager::~OptionManager() {}

OptionManager::OptionType OptionManager::GaussType(const string& path)
{
	if (_access(path.c_str(), 0) == -1) {
		return OPTION_NO;
	}

	tinyxml2::XMLDocument opt_doc;
	if (opt_doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		return OPTION_NO;
	}

	tinyxml2::XMLElement* opt_proj = opt_doc.RootElement();
	if (opt_proj == nullptr) {
		return OPTION_NO;
	}

	const string text = opt_proj->Name();
	if (text == "Option1") {
		return OPTION_1;
	}
	else if(text == "Option2"){
		return OPTION_2;
	}
	else {
		return OPTION_NO;
	}
}

bool OptionManager::Load(const string& path, FasterStereoCuda::StereoOption1& opt)
{
	if (_access(path.c_str(), 0) == -1) {
		Save(path, opt);
		return false;
	}

	tinyxml2::XMLDocument opt_doc;
	if (opt_doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		if (Save(path, opt)) {
			printf("%s%s\n", "option file was created in ", path.c_str());
			return false;
		}
	}

	tinyxml2::XMLElement* opt_proj = opt_doc.RootElement();
	if(opt_proj == nullptr) {
		return false;
	}

	// load options of view
	tinyxml2::XMLElement* opt_view = opt_proj->FirstChildElement("View");
	if (opt_view) {
		tinyxml2::XMLElement* arg_key = NULL;
		arg_key = opt_view->FirstChildElement("width");
		if (arg_key) {
			opt.width = atoi(arg_key->GetText());
		}
		arg_key = opt_view->FirstChildElement("height");
		if (arg_key) {
			opt.height = atoi(arg_key->GetText());
		}
	}

	// load options of stereo algorithm
	tinyxml2::XMLElement* opt_ste = opt_proj->FirstChildElement("Stereo");
	if (opt_ste) {
		tinyxml2::XMLElement* arg_key = NULL;
		arg_key = opt_ste->FirstChildElement("disp_min");
		if (arg_key) {
			opt.min_disp = atoi(arg_key->GetText());
		}
		arg_key = opt_ste->FirstChildElement("disp_max");
		if (arg_key) {
			opt.max_disp = atoi(arg_key->GetText());
		}
		arg_key = opt_ste->FirstChildElement("layers_num");
		if (arg_key) {
			opt.num_layers = atoi(arg_key->GetText());
		}
		arg_key = opt_ste->FirstChildElement("do_consistency_check");
		if (arg_key) {
			opt.do_lr_check = static_cast<bool>(atoi(arg_key->GetText()));
		}
		arg_key = opt_ste->FirstChildElement("do_remove_peaks");
		if (arg_key) {
			opt.do_rm_peaks = static_cast<bool>(atoi(arg_key->GetText()));
		}
		arg_key = opt_ste->FirstChildElement("do_smooth");
		if (arg_key) {
			opt.do_smooth = static_cast<bool>(atoi(arg_key->GetText()));
		}
	}

	return true;
}

bool OptionManager::Load(const string& path, FasterStereoCuda::StereoOption2& opt)
{
	if (_access(path.c_str(), 0) == -1) {
		Save(path, opt);
		return false;
	}

	tinyxml2::XMLDocument opt_doc;
	if (opt_doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		if (Save(path, opt)) {
			printf("%s%s\n", "option file was created in ", path.c_str());
			return false;
		}
	}

	tinyxml2::XMLElement* opt_proj = opt_doc.RootElement();
	if(opt_proj == nullptr) {
		return false;
	}

	// load options of view
	tinyxml2::XMLElement* opt_view = opt_proj->FirstChildElement("View");
	if (opt_view) {
		tinyxml2::XMLElement* arg_key = NULL;
		arg_key = opt_view->FirstChildElement("width");
		if (arg_key) {
			opt.width = atoi(arg_key->GetText());
		}
		arg_key = opt_view->FirstChildElement("height");
		if (arg_key) {
			opt.height = atoi(arg_key->GetText());
		}
	}

	// load options of stereo algorithm
	tinyxml2::XMLElement* opt_ste = opt_proj->FirstChildElement("Stereo");
	if (opt_ste) {
		tinyxml2::XMLElement* arg_key = NULL;
		arg_key = opt_ste->FirstChildElement("depth_min");
		if (arg_key) {
			opt.min_depth = atoi(arg_key->GetText());
		}
		arg_key = opt_ste->FirstChildElement("depth_max");
		if (arg_key) {
			opt.max_depth = atoi(arg_key->GetText());
		}
		arg_key = opt_ste->FirstChildElement("layers_num");
		if (arg_key) {
			opt.num_layers = atoi(arg_key->GetText());
		}
		arg_key = opt_ste->FirstChildElement("do_consistency_check");
		if (arg_key) {
			opt.do_lr_check = static_cast<bool>(atoi(arg_key->GetText()));
		}
		arg_key = opt_ste->FirstChildElement("do_remove_peaks");
		if (arg_key) {
			opt.do_rm_peaks = static_cast<bool>(atoi(arg_key->GetText()));
		}
		arg_key = opt_ste->FirstChildElement("do_smooth");
		if (arg_key) {
			opt.do_smooth = static_cast<bool>(atoi(arg_key->GetText()));
		}
	}

	// load options of cameras
	tinyxml2::XMLElement* opt_cam = opt_proj->FirstChildElement("Camera");
	if (opt_cam) {
		tinyxml2::XMLElement* arg_key = NULL;
		arg_key = opt_cam->FirstChildElement("x0_left_pix");
		if (arg_key) {
			opt.epi.x0_left = atof(arg_key->GetText());
		}
		arg_key = opt_cam->FirstChildElement("y0_left_pix");
		if (arg_key) {
			opt.epi.y0_left = atof(arg_key->GetText());
		}
		arg_key = opt_cam->FirstChildElement("x0_right_pix");
		if (arg_key) {
			opt.epi.x0_right = atof(arg_key->GetText());
		}
		arg_key = opt_cam->FirstChildElement("y0_right_pix");
		if (arg_key) {
			opt.epi.y0_right = atof(arg_key->GetText());
		}
		arg_key = opt_cam->FirstChildElement("baseline_mm");
		if (arg_key) {
			opt.epi.baseline = atof(arg_key->GetText());
		}
		arg_key = opt_cam->FirstChildElement("focus_pix");
		if (arg_key) {
			opt.epi.focus = atof(arg_key->GetText());
		}
	}

	return true;
}

bool OptionManager::Save(const string& path, const FasterStereoCuda::StereoOption1& opt)
{
	tinyxml2::XMLDocument opt_doc;
	const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
	opt_doc.Parse(declaration);

	//project root
	tinyxml2::XMLElement* opt_proj = opt_doc.NewElement("Option1");
	opt_doc.InsertEndChild(opt_proj);
	if (opt_proj == nullptr) {
		return false;
	}

	// save options of view
	tinyxml2::XMLElement* opt_view = opt_doc.NewElement("View");
	opt_proj->InsertEndChild(opt_view);
	tinyxml2::XMLElement* arg_key = nullptr;
	tinyxml2::XMLText* arg_value = nullptr;
	arg_key = opt_doc.NewElement("width");
	arg_value = opt_doc.NewText(std::to_string(opt.width).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_view->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("height");
	arg_value = opt_doc.NewText(std::to_string(opt.height).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_view->InsertEndChild(arg_key);

	// save options of stereo algorithm
	tinyxml2::XMLElement* opt_ste = opt_doc.NewElement("Stereo");
	opt_proj->InsertEndChild(opt_ste);
	arg_key = opt_doc.NewElement("disp_min");
	arg_value = opt_doc.NewText(std::to_string(opt.min_disp).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("disp_max");
	arg_value = opt_doc.NewText(std::to_string(opt.max_disp).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("layers_num");
	arg_value = opt_doc.NewText(std::to_string(opt.num_layers).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("do_consistency_check");
	arg_value = opt_doc.NewText(std::to_string(opt.do_lr_check).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("do_remove_peaks");
	arg_value = opt_doc.NewText(std::to_string(opt.do_rm_peaks).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("do_smooth");
	arg_value = opt_doc.NewText(std::to_string(opt.do_smooth).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);

	if (opt_doc.SaveFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		printf("%s\n", "save option files failed!");
		return false;
	}
	return true;
}
bool OptionManager::Save(const string& path, const FasterStereoCuda::StereoOption2& opt)
{
	tinyxml2::XMLDocument opt_doc;
	const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
	opt_doc.Parse(declaration);

	//project root
	tinyxml2::XMLElement* opt_proj = opt_doc.NewElement("Option2");
	opt_doc.InsertEndChild(opt_proj);

	// save options of view
	tinyxml2::XMLElement* opt_view = opt_doc.NewElement("View");
	opt_proj->InsertEndChild(opt_view);
	tinyxml2::XMLElement* arg_key = nullptr;
	tinyxml2::XMLText* arg_value = nullptr;
	arg_key = opt_doc.NewElement("width");
	arg_value = opt_doc.NewText(std::to_string(opt.width).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_view->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("height");
	arg_value = opt_doc.NewText(std::to_string(opt.height).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_view->InsertEndChild(arg_key);

	// save options of stereo algorithm
	tinyxml2::XMLElement* opt_ste = opt_doc.NewElement("Stereo");
	opt_proj->InsertEndChild(opt_ste);
	arg_key = opt_doc.NewElement("depth_min");
	arg_value = opt_doc.NewText(std::to_string(opt.min_depth).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("depth_max");
	arg_value = opt_doc.NewText(std::to_string(opt.max_depth).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("layers_num");
	arg_value = opt_doc.NewText(std::to_string(opt.num_layers).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("do_consistency_check");
	arg_value = opt_doc.NewText(std::to_string(opt.do_lr_check).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("do_remove_peaks");
	arg_value = opt_doc.NewText(std::to_string(opt.do_rm_peaks).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("do_smooth");
	arg_value = opt_doc.NewText(std::to_string(opt.do_smooth).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_ste->InsertEndChild(arg_key);

	// save options of cameras
	tinyxml2::XMLElement* opt_cam = opt_doc.NewElement("Camera");
	opt_proj->InsertEndChild(opt_cam);
	arg_key = opt_doc.NewElement("x0_left_pix");
	arg_value = opt_doc.NewText(std::to_string(opt.epi.x0_left).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_cam->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("y0_left_pix");
	arg_value = opt_doc.NewText(std::to_string(opt.epi.y0_left).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_cam->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("x0_right_pix");
	arg_value = opt_doc.NewText(std::to_string(opt.epi.x0_right).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_cam->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("y0_right_pix");
	arg_value = opt_doc.NewText(std::to_string(opt.epi.y0_right).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_cam->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("baseline_mm");
	arg_value = opt_doc.NewText(std::to_string(opt.epi.baseline).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_cam->InsertEndChild(arg_key);
	arg_key = opt_doc.NewElement("focus_pix");
	arg_value = opt_doc.NewText(std::to_string(opt.epi.focus).c_str());
	arg_key->InsertEndChild(arg_value);
	opt_cam->InsertEndChild(arg_key);

	if (opt_doc.SaveFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		printf("%s\n", "save option2 files failed!");
		return false;
	}
	return true;
}
