/*
* Title      : User Application for tuning Camera drivers ( ISC and ov426 [v4l2 subdevice] )
* Author     : Jithin <jithinthomas0011@gmail.com>
* Date       : 26/06/2022
* Copyright  : jithinthomas0011
*/

#include "../includes/cam_tuner.h"

using namespace std;

/*
* usage(usage/help)
* @param void
* return void
*/
void usage(){
	cout<<UNDERLINE_TEXT BOLDWHITE_TEXT<<"\t\t*********APPLICATION TO TUNE ov426 AND ISC . Help :- **********"<<RESET_TEXT CYAN_TEXT<<"\
		\n\n\t\tOPTIONS:\n\
		-d         : Device select for tuning (eg: ./Camctl -d 426/ISC ...)\n \
		-G         : Get Tune Parameter value (eg: ./Camctl -d 426 -G contr)\n\
		-S         : Set Tune Parameter value (eg: ./Camctl -d ISC -S gamma=2)\n\
		-L         : List Tune Parameters     (eg: ./Camctl -d 426 -L)\n\
		-R         : Read 426 Register value  (eg: ./Camctl -d 426 -R 0x3503)\n\
		-W         : Write 426 Register       (eg: ./Camctl -d 426 -W 0x3503 3)\n\
		\n\
		TUNE PARAMETERS:\n\
		bright     : Brightness Control             (ISC only)[-1024 <-> 1023]\n\
		contr      : Contrast Control               (ISC only)[-1024 <-> 1023]\n\
		gamma      : Gamma Control                  (ISC only)[0 <-> 2]\n\
		gain       : Gain Control                   (ISC/426 )[1 <-> 4]\n\
		awb        : Automatic White Balance        (ISC/426 )[0 <-> 1]\n\
		dowb       : Do-White-Balance               (ISC only)[1 <-> 1]\n\
		redg       : Red Component Gain             (ISC/426 )[0 <-> 255]\n\
		blueg      : Blue Component Gain            (ISC/426 )[0 <-> 255]\n\
		greeng     : Green Component Gain           (426 only)[0 <-> 255]\n\
		greenredg  : Green-Red Component Gain       (ISC only)[0 <-> 8191]\n\
		greenblueg : Green-Blue Component Gain      (ISC only)[0 <-> 8191]\n\
		redofst    : Red Component Offset           (ISC only)[-4095 <-> 4095]\n\
		blueofst   : Blue Component Offset          (ISC only)[-4095 <-> 4095]\n\
		grofst     : Green-Red Offset               (ISC only)[-4095 <-> 4095]\n\
		gbofst     : Green-Blue Offset              (ISC only)[-4095 <-> 4095]\n\
		agc        : Auto Gain Control              (426 only)[0 <-> 1]\n\
		aec        : Auto Exposure Control          (426 only)[0 <-> 1]\n\
		expo       : Exposure Control               (426 only)[0 <-> 33]\n\
		blc        : Black Level Control            (426 only)[0 <-> 255]\n\n\
		"<<YELLOW_TEXT<<"*Note:\n\t\t1)To adjust chroma component's gain/offset, disable AWB first.\n\
		2)Use prefix \"0x\" for hex address/values. No prefix for decimal address/values.\n\n"<<RESET_TEXT<<endl;
}

/*
* ReadReg(Register read function for ov426)
* @param sRegAddr(Register address)
* @return bool(Register read status)
*/
bool ReadReg(string sRegAddr){
	uint16_t RegAddr = 0;
	cout<<__func__<<":: Reg = "<<sRegAddr<<endl;
	try{
		if(sRegAddr.find("0x") != string::npos){
			cout<<"Reg Address provided in hex!"<<endl;
			RegAddr =stol(sRegAddr, nullptr, 16);
		}else{
			cout<<"Not a hex address"<<endl;
			RegAddr =stol(sRegAddr, nullptr, 10);
		}
	}catch(exception e){
		cout<<"Regsiter address provided ["<<sRegAddr<<"] is invalid!!  "<<e.what()<<endl;
		return false;
		}
	
	cout << "Reg address (in dec) = " << RegAddr << "\n";
	/* Open Device */
	int fd = open(ISC_PATH, O_RDWR);
	if (fd == -1) {
		cout<<"Error in opening file\n";
		return false;
	}
	struct v4l2_dbg_register reg = {0}; 
	/* Set Subdevice-0 */
	reg.match.type = V4L2_CHIP_MATCH_SUBDEV;
	reg.match.addr = 0;

	/* Initialize register addr */
	reg.reg = RegAddr;
	reg.size = 2;
	reg.val = 0;

	/* Read register */
	if (ioctl(fd, VIDIOC_DBG_G_REGISTER, &reg) < 0) {
		perror("Read error");
		close(fd);
		fd=-1;
		return false;
	}
	cout<<"Reg "<<reg.reg<<"(dec) read value  = "<<BOLDCYAN_TEXT<< reg.val<<"(dec)."<<RESET_TEXT<<endl;
	close(fd);
	fd=-1;
	return true;
}

/*
* WriteReg(Register write function for ov426)
* @param sRegAddr(Register address) , 
*        sRegVal(Value to write)
* @return bool(Register write status)
*/
bool WriteReg(string sRegAddr, string sRegVal){
	uint16_t RegAddr = 0 ,RegVal =0;
	cout<<__func__<<":: Reg = "<<sRegAddr<<endl;
	try{
		if(sRegAddr.find("0x") != string::npos){
			cout<<"Reg Address provided in hex!"<<endl;
			RegAddr =stol(sRegAddr, nullptr, 16);
		}else{
			cout<<"Not a hex address"<<endl;
			RegAddr =stol(sRegAddr, nullptr, 10);
		}
		if(sRegVal.find("0x") != string::npos){
			cout<<"Reg Val provided in hex!"<<endl;
			RegVal =stol(sRegVal, nullptr, 16);
		}else{
			cout<<"Not a hex val"<<endl;
			RegVal =stol(sRegVal, nullptr, 10);
		}
	}catch(exception e){
		cout<<"Regsiter address ["<<sRegAddr<<"]/value ["<<sRegVal<<"] provided is invalid!!  "<<e.what()<<endl;
		return false;
		}
	cout<<__func__<<":: Reg Add (in dec) = "<<RegAddr<<", RegVal (in dec)= "<<RegVal<<endl;
	int fd = open(ISC_PATH, O_RDWR);
	if (fd == -1) {
		cout<<"Error in opening file\n";
		return false;
	}
	struct v4l2_dbg_register reg = {0}; 
	/* Set Subdevice-0 */
	reg.match.type = V4L2_CHIP_MATCH_SUBDEV;
	reg.match.addr = 0;
	/* Initialize register addr */
	reg.reg = RegAddr;
	reg.size = 2;
	reg.val = RegVal;
	/* Read register */
	if (ioctl(fd, VIDIOC_DBG_S_REGISTER, &reg) < 0) {
		perror("Read error");
		close(fd);
		fd=-1;
		return false;
	}
	cout<<"Reg "<<reg.reg<<"(dec) read value  = "<< reg.val<<"(dec)."<<endl;
	close(fd);
	fd=-1;
	return true;
}

/*
* enumerate_menu(Enumerate user controls for tuning)
* @param fd(file descriptor of v4l2 device) , 
* @return void
*/
void enumerate_menu(int fd){
	memset(&Camctl::querymenu, 0, sizeof(Camctl::querymenu));
	Camctl::querymenu.id = Camctl::queryctrl.id;
	for (Camctl::querymenu.index = Camctl::queryctrl.minimum;
			Camctl::querymenu.index <= Camctl::queryctrl.maximum;
			Camctl::querymenu.index++) {
		if (0 == ioctl(fd, VIDIOC_QUERYMENU, &Camctl::querymenu)) {
			cout<<Camctl::querymenu.name<<endl;
		}
	}
}

/*
* ListParams(List user controls for tuning)
* @param DevPath(v4l2 device path)
* @return bool(ListParams status)
*/
bool ListParams(string DevPath){
	cout<<__func__<<endl;
	int fd = open(DevPath.c_str(), O_RDWR);
	if(fd < 0)
	{	struct v4l2_dbg_register reg = {0}; 
		perror("open dev error");
		return false;
	}
	memset(&Camctl::query_ext_ctrl, 0, sizeof(Camctl::query_ext_ctrl));
	Camctl::query_ext_ctrl.id = V4L2_CTRL_FLAG_NEXT_CTRL | V4L2_CTRL_FLAG_NEXT_COMPOUND;
	while (0 == ioctl(fd, VIDIOC_QUERY_EXT_CTRL, &Camctl::query_ext_ctrl)) {
		if (!(Camctl::query_ext_ctrl.flags & V4L2_CTRL_FLAG_DISABLED)) {
			cout<<CYAN_TEXT<< Camctl::query_ext_ctrl.name<<RESET_TEXT<<endl;

			if (Camctl::query_ext_ctrl.type == V4L2_CTRL_TYPE_MENU)
				enumerate_menu(Camctl::query_ext_ctrl.id);
		}
		Camctl::query_ext_ctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL | V4L2_CTRL_FLAG_NEXT_COMPOUND;
	}
	if (errno != EINVAL) {
		perror("VIDIOC_QUERY_EXT_CTRL");
		close(fd);
		fd=-1;
		return false;
	}
	close(fd);
	fd= -1;
	return true;
}

/*
* GetParam(Get value of tuning parameter)
* @param DevPath(v4l2 device path) , 
*        ctrl(v4l2_control instance)
* @return bool(GetParam status)
*/
bool GetParam(string DevPath, struct v4l2_control ctrl){
	int fd = open(DevPath.c_str(), O_RDWR);
	if(fd < 0){
		perror("open dev error");
		return false;
	}
	int ret = ioctl(fd, VIDIOC_G_CTRL,&ctrl);
	if(ret < 0) {
		perror("Get Param failed");
		close(fd);
		fd=-1;
		return false;
	} else {
		cout<<BOLDCYAN_TEXT<<"Value =  "<<ctrl.value<<RESET_TEXT<<endl;
		if(ctrl.id == V4L2_CID_EXPOSURE ){
			float out = ((ctrl.value *1000)/(30*462))+1;
			cout<<BOLDCYAN_TEXT<<"Exposure mapped Value = "<<out<<RESET_TEXT<<endl;
		}
	}
	close(fd);
	fd=-1;
	return true;
}

/*
* GetTuneValToSet(Get actual value to set for tuning parameter)
* @param ctrl(v4l2_control instance) , 
*        inVal(user input)
* @return bool(GetTuneValToSet status)
*/
bool GetTuneValToSet(struct v4l2_control &ctrl,int inVal ){
	bool retVal=true;
	switch (ctrl.id){
		case V4L2_CID_EXPOSURE:{
			if(inVal<0){
				retVal = false;
			}
			float fps =30, vts=462;
			float expl =( inVal *  fps  * (vts/1000));
			if(expl>458){
				cout<<"Provide exposure <= (1/(fps))*1000"<<endl;
				retVal = false;
			}
			ctrl.value = expl;
		}
		break;
		case V4L2_CID_GAIN:
			if (inVal <= 1 || inVal > 4 ){
				cout<< "Invalid gain"<<endl;
				retVal =false;
			}
			else
				ctrl.value =inVal;
		break;
		case V4L2_CID_RED_BALANCE:
		case V4L2_CID_BLUE_BALANCE:
		case V4L2_CID_AUDIO_BASS://Green_balance
		   if (inVal< 1 || inVal > 4 ){
   			cout<<"Invalid chroma gain"<<endl;
			   retVal=false;
		   }
		   else{
			   ctrl.value = inVal;
		   }
		break;
		case V4L2_CID_BLACK_LEVEL:
			  if (inVal< 0 || inVal > 255){
   				cout<<"Invalid blc"<<endl;
				   retVal =false;
			  }
			  else{
				  ctrl.value =inVal;
			  }
		break;
		case V4L2_CID_DO_WHITE_BALANCE:
			if(1!= inVal){
				cout<<"Invalide Do WB."<<endl;
				retVal = false;
			}
			else{
				ctrl.value =inVal;
			}
		break;
		case V4L2_CID_AUTO_WHITE_BALANCE:
		case V4L2_CID_AUTOGAIN:
		case V4L2_CID_EXPOSURE_AUTO:
			if(1!= inVal && 0!= inVal){
				cout<<"Invalid Auto Exposure/ Gain/ WB ."<<endl;
				retVal =false;
			}
			else{
				ctrl.value =inVal;
			}
		break;
		case V4L2_CID_GAMMA:
			if(inVal< 0 || inVal > 2){
				cout<<"Invalid Gamma."<<endl;
				retVal =false;
			}
			else{
				ctrl.value =inVal;
			}		
		break;
		default:
			ctrl.value = inVal;
		break;
	}
	return retVal;
}

/*
* SetParam(set/update tuning parameter value)
* @param DevPath(v4l2 device path),
*        ctrl(v4l2_control instance) , 
*        sVal(user input)
* @return bool(SetParam status)
*/
bool SetParam(string DevPath, struct v4l2_control &ctrl , string sVal ){
	cout<<__func__<<endl;
	int Val = -1;
	try{
		if(sVal.find("0x") != string::npos){
			cout<<"Value provided in hex!"<<endl;
			Val =stol(sVal, nullptr, 16);
			cout << "Set Val (in dec) = " << Val << "\n";
		}
		else{
			cout<<"Not a hex value"<<endl;
			Val =stol(sVal, nullptr, 10);
			cout << "Set Val (in dec) = " << Val << "\n";
		}
	}catch(exception e){
		cout<<"Set Value provided ["<<sVal<<"] is invalid!!  "<<e.what()<<endl;
		return false;
		}
	int fd = open(DevPath.c_str(), O_RDWR);
	if(fd < 0){
		perror("open dev error");
		return false;
	}
	if(false == GetTuneValToSet(ctrl, Val) ){
		cout<<"Invalid Set value !"<<endl;
    close(fd);
    fd=-1;
		return false;
	}
	cout<<"Final set param val = "<<ctrl.value<<endl;
	int ret1 = ioctl(fd, VIDIOC_S_CTRL,&ctrl);
	if(ret1 < 0) {
		perror("Set Param ioctl failed ");
		close(fd);
		fd=-1;
		return false;
	} else {
		cout<<" Value =  "<<ctrl.value<<endl;
	}
	close(fd);
	fd =-1;
	return true;
}

/*
* main(Entry function for Camctl tuning application)
* @param argc(command line argument count),
*        argv(pointer to array of command line arguments)
* @return int(main() status)
*/
int main(int argc , char *argv[]){
	vector<string>optn(argc);
	for(int it = 1;it<argc;++it){
		optn[it-1]= argv[it];
	}
	if( argc <= 3){
		cout<<"Invalid arguments!!"<<endl;
		usage();
	}else if(optn[0] != "-d"){
		cout<<"Invalid arguments!![Device not specified]"<<endl;
		usage();
	}else{
		unordered_map<string,string>::const_iterator itrD = Camctl::DEV_MAP.find(optn[1]);
		if (itrD != Camctl::DEV_MAP.end()){
			unordered_map<string,Camctl::TuneArgs>::const_iterator itrO = Camctl::OPTN_MAP.find(optn[2]);
			if (itrO != Camctl::OPTN_MAP.end()){
				switch(itrO->second){
					case Camctl::eGETPARAM:{
						unordered_map<string,int>::const_iterator itrT = Camctl::TUNE_MAP.find(optn[3]);
						if(itrT!=Camctl::TUNE_MAP.end() ){
							memset(&Camctl::control, 0, sizeof(Camctl::control));
							Camctl::control.id = itrT->second;
							GetParam(itrD->second ,Camctl::control );
						}
						else{
							cout<<"Invalid Tuning Parameter!"<<endl;
							usage();
						}
						break; 
					}
					case Camctl::eSETPARAM:{
						 if(argc < 6){
							cout<<"Parameter/ value to set not provided!"<<endl;
						}else{
							unordered_map<string,int>::const_iterator itrT = Camctl::TUNE_MAP.find(optn[3]);
							if(itrT!=Camctl::TUNE_MAP.end() ){
								memset(&Camctl::control, 0, sizeof(Camctl::control));
								Camctl::control.id = itrT->second;
								SetParam(itrD->second ,Camctl::control, optn[4] );
							}
							else{
								cout<<"Invalid Tuning Parameter!"<<endl;
								usage();
							}
						}
						break; 
					}
					case Camctl::eLISTPARAMS:
							ListParams(itrD->second);
						break;
					case Camctl::eREADREG:
						if("426" != itrD->first){
							cout<<"Read Register and Write Register are defined only for 426"<<endl;
						}else if(argc < 5){
							cout<<"Register address not provided!"<<endl;
							usage();
						}else{
							ReadReg(optn[3]);
						}
						break;
					case Camctl::eWRITEREG:
						if("426" != itrD->first){
							cout<<"Read Register and Write Register are defined only for 426"<<endl;
						}else if(argc < 6){
							cout<<"Register address/ value to set not provided!"<<endl;
						}else{
							WriteReg(optn[3] ,optn[4] );
						}
						break;
					default:
						break;
				}
			}
			else{
				cout<<"Invalid Tuning Parameter!"<<endl;
				usage();
			}
		}
		else {
			cout<<"Invalid device path"<<endl;
			usage();
		}
	}
	return 0;
}
