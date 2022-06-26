/*
* Title      : User Application for tuning Camera drivers ( ISC and ov426 [v4l2 subdevice] )
* Author     : Jithin <jithinthomas0011@gmail.com>
* Date       : 26/06/2022
* Copyright  : jithinthomas0011
*/

#ifndef CAM_TUNER_H
#define CAM_TUNER_H

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <linux/videodev2.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <memory.h>


/*dev paths*/
#define ISC_PATH "/dev/video0"
#define OV426_PATH "/dev/v4l-subdev0"

/*Some additional v4l2 macros for ISC tuning.*/
#define   ISC_GREEN_RED_GAIN        0x009819c2
#define   ISC_GREEN_BLUE_GAIN       0x009819c3
#define   ISC_RED_OFFSET            0x009819c4
#define   ISC_BLUE_OFFSET           0x009819c5
#define   ISC_GREEN_RED_OFFSET      0x009819c6
#define   ISC_GREEN_BLUE_OFFSET     0x009819c7

/*For text formatting*/
#define RESET_TEXT       "\033[0m"
#define BLACK_TEXT       "\033[30m"             /* Black */
#define RED_TEXT         "\033[31m"             /* Red */
#define GREEN_TEXT       "\033[32m"             /* Green */
#define YELLOW_TEXT      "\033[33m"             /* Yellow */
#define BLUE_TEXT        "\033[34m"             /* Blue */
#define MAGENTA_TEXT     "\033[35m"             /* Magenta */
#define CYAN_TEXT        "\033[36m"             /* Cyan */
#define WHITE_TEXT       "\033[37m"             /* White */
#define BOLDBLACK_TEXT   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED_TEXT     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN_TEXT   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW_TEXT  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE_TEXT    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA_TEXT "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN_TEXT    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE_TEXT   "\033[1m\033[37m"      /* Bold White */
#define UNDERLINE_TEXT   "\033[4m"

using namespace std;

namespace Camctl{

	enum TuneArgs{
		eGETPARAM =0, 
		eSETPARAM, 
		eLISTPARAMS,
		eREADREG,
		eWRITEREG

	};
	const unordered_map<string,string>DEV_MAP{
		{ "426", OV426_PATH}, 	
		{ "ISC" , ISC_PATH}};

	const unordered_map<string, TuneArgs>OPTN_MAP{
		{"-G", eGETPARAM},
		{"-S", eSETPARAM},
		{"-L", eLISTPARAMS},
		{"-R", eREADREG},
		{"-W", eWRITEREG}};

	const unordered_map<string,int>TUNE_MAP{
		{"bright",         V4L2_CID_BRIGHTNESS},
		{"contr",          V4L2_CID_CONTRAST},
		{"gamma",          V4L2_CID_GAMMA},
		{"blc",            V4L2_CID_BLACK_LEVEL},
		{"agc",            V4L2_CID_AUTOGAIN},
		{"aec",            V4L2_CID_AUDIO_BALANCE},
		{"gain",           V4L2_CID_GAIN},
		{"expo",           V4L2_CID_EXPOSURE},
		{"awb",            V4L2_CID_AUTO_WHITE_BALANCE},
		{"dowb",           V4L2_CID_DO_WHITE_BALANCE},
		{"redg",           V4L2_CID_RED_BALANCE},
		{"greeng",         V4L2_CID_AUDIO_BASS},
		{"blueg",          V4L2_CID_BLUE_BALANCE},
		{"greenredg",      ISC_GREEN_RED_GAIN},
		{"greenblueg",     ISC_GREEN_BLUE_GAIN},
		{"redofst",        ISC_RED_OFFSET},
		{"blueofst",       ISC_BLUE_OFFSET},
		{"grofst",         ISC_GREEN_RED_OFFSET},
		{"gbofst",         ISC_GREEN_BLUE_OFFSET},

	};	
	struct v4l2_queryctrl queryctrl;
	struct v4l2_querymenu querymenu;
	struct v4l2_control control;
	struct v4l2_query_ext_ctrl query_ext_ctrl;
}; //namesoace Camctl

#endif //CAM_TUNER_H
