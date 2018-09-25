#ifndef PROTOBUFMSG_H
#define PROTOBUFMSG_H

#include "message/LT.message.pb.h"
#include <string>
#include <list>
#include <vector>
#include <map>


using namespace LTMessage;
using namespace std;


enum{
    ResponseCode_OK = ResponseStatus_StatusCode_OK,
    ResponseCode_Fail = ResponseStatus_StatusCode_FAIL,
    ResponseCode_Terminal_NotFound = ResponseStatus_StatusCode_TERMINAL_NOTFOUND,
    ResponseCode_BaseStation_NotFound = ResponseStatus_StatusCode_BASE_STATION_NOTFOUND,
    ResponseCode_Network_Failtrue = ResponseStatus_StatusCode_NETWORK_FAILURE,
};

enum{
    NO_EVENT = UpdateEvent_Event_NO_EVENT,
    TERMINAL_LIST_UPDATE = UpdateEvent_Event_TERMINAL_LIST_UPDATE,
    BASE_STATION_LIST_UPDATE = UpdateEvent_Event_BASE_STATION_LIST_UPDATE,
    CONFIG_UPDATE = UpdateEvent_Event_CONFIG_UPDATE,
};

enum{
    sensor_raw_type = 1,
    sensor_filter_type = 2,
    robot_filter_type = 4,
    sensor_filter_noimu_type = 8,
    robot_filter_noimu_type = 16,
    sensor_raw_unregistered_type = 32,
    sensor_filter0_type = 64,
    sensor_filter1_type = 128
};

enum{
    multi_base_station = ControllerBroadcast_ControllerType_MULTI,
    singal_base_station = ControllerBroadcast_ControllerType_SINGLE,
    unknown_base_station
};

class ProtoBufMsg
{
public:
    ProtoBufMsg();


    /***************************终端信息请求和响应接口函数*****************************/
    bool TermPropertyReqPacked(string &PackReq);
    bool TermPropertyUpdatePacked(string &PackUpdate);
    bool TermPropertyRespUnPacked(string &UnPackResp);
    /*获取各个终端信息的接口函数*/
    int GetRespCode(void);
    string GetRespDetail(void);
    unsigned int GetTerminalSize(void);
    string GetTerminalName(int index);
    bool GetTerminalName(string &mac,string &name);
    string GetTerminalIp(int index);
    string GetTerminalMac(int index);
    int GetTerminalSensorSize(int index);
    inline bool GetTerminalSensorSize(string &mac, int &size);
    int GetTerminalSensorIndex(int termIndex, int sensorIndex);
    int GetTerminalSensorActive(int termIndex,int sensorIndex);
    double GetTerminalSensorHeight(int termIndex,int sensorIndex);
    double GetTerminalSensorPointX(int termIndex,int sensorIndex);
    double GetTerminalSensorPointY(int termIndex,int sensorIndex);
    double GetTerminalSensorPointZ(int termIndex,int sensorIndex);
    int GetTerminalReporterIpSize(int index);
    string GetTerminalReporterIp(int termIndex, int reporterIndex);
    int GetTerminalReporterPortSize(int index);
    int GetTerminalReporterPort(int termIndex, int reporterIndex);
    bool FindTerminalByMac(string &mac);
    /*设置各个终端信息的接口函数*/
    void ClearTerminalProperty(void);
    void AddTerminalProperty(unsigned int size);
    void SetTerminalName(unsigned int index,string host_name);
    void SetTerminalIp(unsigned int index,string ip_addr);
    void SetTerminalMac(unsigned int index,string mac_addr);
    void ClearTerminalSensor(unsigned int index);
    void RemoveTerminalSensor(unsigned int index,int sensor_index);
    void AddTerminalSensor(unsigned int index, int size);
    void SetTerminalSensorIndex(unsigned int termIndex,int sensorIndex,int index);
    void SetTerminalSensorActive(unsigned int termIndex,int sensorIndex,int active);
    void SetTerminalSensorHeight(unsigned int termIndex,int sensorIndex,double height);
    void SetTerminalSensorPointX(unsigned int termIndex,int sensorIndex,double x);
    void SetTerminalSensorPointY(unsigned int termIndex,int sensorIndex,double y);
    void SetTerminalSensorPointZ(unsigned int termIndex,int sensorIndex,double z);
    void SetTerminalSensorPoint(unsigned int termIndex,int sensorIndex,double &x,double &y,double &z);
    void ClearTerminalReporter(unsigned int index);
    void RemoveTerminalReporter(unsigned int index,int reporter_index);
    void AddTerminalReporterIp(unsigned int index,string ip_addr);
    void SetTerminalReporterIp(unsigned int termIndex, int reporterIndex,string ip_addr);
    void AddTerminalReporterPort(unsigned int index,int port);
    void SetTerminalReporterPort(unsigned int termIndex, int reporterIndex,int port);
    /**********************************************************************************/


    /*************************终端控制请求和响应接口函数*******************************/
    bool TermActionRestartHostPacked(int &sequence, vector<string> &terminal_mac, string &PackAction);
    bool TermActionRestartBinPacked(int &sequence, vector<string> &terminal_mac, string &PackAction);
    bool TermActionUpdateBinPacked(int &sequence, string &url, vector<string> &terminal_mac, string &PackAction);
    bool TermActionStartCalibrationPacked(int &sequence,vector<string> &terminal_mac,
                                          string &PackAction);
    bool TermActionStopCalibrationPacked(int &sequence,vector<string> &terminal_mac,
                                         string &PackAction);
    bool TermActionResponseUnPacked(string & UnpackResp,int &sequence,int &status,string &detail);
    /**********************************************************************************/


    /***************************基站信息请求和响应接口函数*****************************/
    bool BaseStationPropertyReqPacked(string &PackReq);
    bool BaseStationPropertyActiveReqPacked(string &PackReq);
    bool BaseStationPropertyUpdatePacked(string &PackUpdate);
    bool BaseStationPropertyRespUnPacked(string &UnPackResp);
    /*获取各个基站信息的接口函数*/
    int GetBsRespCode(void);
    string GetBsRespDetail(void);
    unsigned int GetBaseStationSize(void);
    string GetBsChipId(unsigned int index);
    string GetBsHostName(unsigned int index);
    int GetBsId(unsigned int index);
    int GetBsIngroupAddr(unsigned int index);
    int GetBsFreq(unsigned int index);
    double GetBsStartAngle(unsigned int index);
    double GetBsCoordinateX(unsigned int index);
    double GetBsCoordinateY(unsigned int index);
    double GetBsCoordinateZ(unsigned int index);
    double GetBsOrientation_pitch(unsigned int index);
    double GetBsOrientation_roll(unsigned int index);
    double GetBsOrientation_yaw(unsigned int index);
    int GetBsLedStatus(unsigned int index);
    double GetBsMotorSpeed(unsigned int index);
    /*设置各个基站信息的接口函数*/
    void ClearBsProperty(void);
    void AddBsProperty(unsigned int size);
    void SetBsChipId(unsigned int index,string &chip_id);
    void SetBsHostName(unsigned int index, string &name);
    void SetBsId(unsigned int ListIndex,int BsId);
    void SetBsIngroupAddr(unsigned int index, int ingroup_addr);
    void SetBsFreq(unsigned int index,int freq);
    void SetBsStartAngle(unsigned int index, double angle);
    void SetBsCoodinate(unsigned int index, double x, double y, double z);
    void SetBsCoodinateX(unsigned int index,double x);
    void SetBsCoodinateY(unsigned int index,double y);
    void SetBsCoodinateZ(unsigned int index,double z);
    void SetBsOrientationPitch(unsigned int index, double pitch);
    void SetBsOrientationRoll(unsigned int index,double roll);
    void SetBsOrientationYaw(unsigned int index,double yaw);
    void SetBsOrientation(unsigned int index,double pitch,double roll,double yaw);
    void SetBsLedStatus(unsigned int index, int led_status);
    void SetMotorSpeed(unsigned int index,double motor_speed);
    /**********************************************************************************/


    /*************************基站控制请求和响应接口函数*******************************/
    bool BsActionPacked(int &sequence, const string &command, string &PackAction);
    bool BsActionResponseUnPacked(string &UnpackResp, int &sequence, int &status,
                                  string &detail, string &info);
    /**********************************************************************************/


    /**************************GUI心跳请求和响应接口函数*******************************/
    bool HeartBeatReqPacked(int &sequence, string &mac, string &PackReq);
    bool HeartBeatRespUnPacked(string &UnpackResp,int &sequence,
                               int &status,string &status_detail,
                               int &event,string &event_detail);
    /**********************************************************************************/


    /*****************************控制器广播信息接口函数*******************************/
    bool ControllerBroadcastUnPacked(string &Unpack, string &ctrl_ip, int &req_port, int &sub_port, int &bs_type);
    bool ControllerBroadcastRespPacked(string &host_name, string &ip, string &mac,
                                       string &PackResp);
    /**********************************************************************************/


    /*****************************解析终端发布的定位信息接口函数***********************/
    bool TermPubLocInfoUnpacked(string &UnpackLoc);
    bool TermPubLocInfoUnpacked(string &UnpackLoc, string &terminal_id, int base_station_index,
                                int type, bool &valid);
    bool TermPubLocInfoUnpacked(string &UnpackLoc,string &terminal_id,int type,bool &valid);
    bool TermPubLocInfoUnpacked(string &UnpackLoc, vector<string> &terminal_id_vector, int filter, bool &valid);
    bool TermPubLocInfoUnpacked(string &UnpackLoc,int type,bool &valid);
    unsigned int GetLocationSize(void);
    unsigned int GetLocationSize(string &index, int type);
    unsigned int GetLocationSize(int type);
    bool CheckLocationSize(void);
    int GetLocationType(unsigned int index);
    int TranslateLocationType(int type);
    void GetLocationCoordinate(unsigned int index,double &x,double &y,double &z);
    bool GetLocationCoordinate(string &index, unsigned int loc_index, int type, double &x, double &y, double &z);
    void GetLocationCoordinate(unsigned int index,int type,double &x,double &y,double &z);
    double GetLocationHeading(unsigned int index);
    bool GetLocationHeading(string &index,unsigned int loc_index,int type,double &heading);
    double GetLocationHeading(unsigned int index,int type);
    double GetLocationGamma(unsigned int index);
    double GetLocationTheta(unsigned int index);
    double GetLocationTs(unsigned int index);
    int GetLocationBsIndex(unsigned int index);
    int GetLocationSensorId(unsigned int index);
    bool GetLocationSensorId(string &index, unsigned int loc_index, int type, int &sensor_id);
    int GetLocationSensorChannelId(unsigned int index);
    string GetLocationTerminalId(unsigned int index);
    unsigned int GetLocateTimes(string &terminal_id, int type);
    void ClearLocateTimes(void);
    void ClearLocation(void);
    void ClearLocation(int type);
    void EraseFrontLocation(int range);
    void EraseFrontLocation(string &index, int type, unsigned int range);
    void AddLocationFilter(string &filter);
    void AddLocationFilters(vector<string> &filters);
    void EraseLocationFilter(string &filter);
    void EraseLocationFilters(vector<string> &filters);
    void EraseLocationFiltersByType(vector<string> &filters,
                                    map<string,vector<Location>> &location);
    /**********************************************************************************/
private:
    /*************************终端信息请求和响应私有变量和函数*******************************/
    /*终端信息请求和响应变量*/
    ResponseStatus_StatusCode StatusCode;
    string StatusDetail;
    vector<TerminalProperty> TermPropertyList;

    /*终端信息请求和响应私有函数*/
    PointXYZ GeneratePoint3DMsg(double x,double y,double z);
    TerminalProperty_Sensor GenerateSensorMsg(int32_t index,
                                              int32_t active, double height, PointXYZ point);
    ResponseStatus GenerateRespStatusMsg(ResponseStatus_StatusCode code,string detail);
    TerminalProperty GenerateTermPropertyMsg(string &host_name,
                                             string &ip_addr, string &mac_addr,
                                             list<TerminalProperty_Sensor> &sensor_list,
                                             list<string> &report_ip_list,
                                             list<int32_t> &report_port_list);
    TerminalPropertyRequest GenerateTermPropertyReqMsg(int type,
                                                       vector<TerminalProperty> &TermPropertyList);
    /******************************************************************************************/


    /***************************基站信息请求和响应私有变量和函数*******************************/
    /*基站信息请求和响应变量*/
    ResponseStatus_StatusCode BsProperty_StatusCode;
    string BsProperty_StatusDetail;
    vector<BaseStationProperty> BsPropertyList;

    /*基站信息请求和响应私有函数*/
    BaseStationPropertyRequest GenerateBSPropertyReqMsg(int type,
                                                        vector<BaseStationProperty> &BSPropertyList);
    /******************************************************************************************/


    /*****************************解析终端发布的定位信息私有变量*******************************/
    vector<Location> publish_location;
    map<string,vector<Location>> sensor_raw_location;
    map<string,vector<Location>> sensor_filter_location;
    map<string,vector<Location>> robot_filter_location;
    map<string,vector<Location>> sensor_filter_noimu_location;
    map<string,vector<Location>> robot_filter_noimu_location;
    map<string,vector<Location>> sensor_raw_unregistered_location;
    map<string,vector<Location>> sensor_raw_filter0_location;
    map<string,vector<Location>> sensor_raw_filter1_location;
    vector<Location> sensor_raw_location_;
    vector<Location> sensor_filter_location_;
    vector<Location> robot_filter_location_;
    map<string,unsigned int> sensor_raw_locate_times_;
    map<string,unsigned int> sensor_filter_locate_times_;
    map<string,unsigned int> robot_filter_locate_times_;
    /******************************************************************************************/
};


/***************************终端信息请求和响应接口函数*******************************/
inline int ProtoBufMsg::GetRespCode(void)
{
    return static_cast<int>(StatusCode);
}

inline string ProtoBufMsg::GetRespDetail(void)
{
    return StatusDetail;
}

inline unsigned int ProtoBufMsg::GetTerminalSize(void)
{
    return TermPropertyList.size();
}

inline string ProtoBufMsg::GetTerminalName(int index)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + index;
    return (i->host_name());
}

inline bool ProtoBufMsg::GetTerminalName(string &mac, string &name)
{
    for(unsigned int i = 0;i < TermPropertyList.size();i ++)
    {
        if(TermPropertyList[i].mac_addr() == mac)
        {
            name = TermPropertyList[i].host_name();
            return true;
        }
    }

    return false;
}

inline string ProtoBufMsg::GetTerminalIp(int index)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + index;
    return (i->ip_addr());
}

inline string ProtoBufMsg::GetTerminalMac(int index)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + index;
    return (i->mac_addr());
}

inline int ProtoBufMsg::GetTerminalSensorSize(int index)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + index;
    return ((i->sensor_list()).size());
}

inline bool ProtoBufMsg::GetTerminalSensorSize(string &mac, int &size)
{
    vector<TerminalProperty>::iterator it;

    for(unsigned int i = 0;i < TermPropertyList.size();i ++)
    {
        if(TermPropertyList[i].mac_addr() == mac)
        {
            it = TermPropertyList.begin() + static_cast<int>(i);
            size = ((it->sensor_list()).size());
            return true;
        }
    }

    return false;
}

inline int ProtoBufMsg::GetTerminalSensorIndex(int termIndex, int sensorIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return ((i->sensor_list(sensorIndex)).index());
}

inline int ProtoBufMsg::GetTerminalSensorActive(int termIndex,int sensorIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return ((i->sensor_list(sensorIndex)).active());
}

inline double ProtoBufMsg::GetTerminalSensorHeight(int termIndex,int sensorIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return ((i->sensor_list(sensorIndex)).height());
}

inline double ProtoBufMsg::GetTerminalSensorPointX(int termIndex,int sensorIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return ((i->sensor_list(sensorIndex)).coordinates().x());
}

inline double ProtoBufMsg::GetTerminalSensorPointY(int termIndex,int sensorIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return ((i->sensor_list(sensorIndex)).coordinates().y());
}

inline double ProtoBufMsg::GetTerminalSensorPointZ(int termIndex,int sensorIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return ((i->sensor_list(sensorIndex)).coordinates().z());
}

inline int ProtoBufMsg::GetTerminalReporterIpSize(int index)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + index;
    return (i->report_ip_addr_size());
}

inline string ProtoBufMsg::GetTerminalReporterIp(int termIndex,int reporterIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return (i->report_ip_addr(reporterIndex));
}

inline int ProtoBufMsg::GetTerminalReporterPortSize(int index)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + index;
    return (i->report_tcp_port_size());
}

inline int ProtoBufMsg::GetTerminalReporterPort(int termIndex,int reporterIndex)
{
    vector<TerminalProperty>::iterator i = TermPropertyList.begin() + termIndex;
    return (i->report_tcp_port(reporterIndex));
}

inline bool ProtoBufMsg::FindTerminalByMac(string &mac)
{
    for(unsigned int i = 0;i < TermPropertyList.size();i ++)
    {
        if(TermPropertyList[i].mac_addr() == mac)
        {
            return true;
        }
    }

    return false;
}

inline void ProtoBufMsg::ClearTerminalProperty(void)
{
    for(unsigned int i = 0;i < TermPropertyList.size();i ++)
    {
        TermPropertyList[i].Clear();
    }
    TermPropertyList.clear();
}

inline void ProtoBufMsg::AddTerminalProperty(unsigned int size)
{
    TerminalProperty TermProperty;
    vector<TerminalProperty>::iterator it = TermPropertyList.end();
    TermPropertyList.insert(it,size,TermProperty);
}

inline void ProtoBufMsg::SetTerminalName(unsigned int index,string host_name)
{
    TermPropertyList[index].set_host_name(host_name);
}

inline void ProtoBufMsg::SetTerminalIp(unsigned int index,string ip_addr)
{
    TermPropertyList[index].set_ip_addr(ip_addr);
}

inline void ProtoBufMsg::SetTerminalMac(unsigned int index,string mac_addr)
{
    TermPropertyList[index].set_mac_addr(mac_addr);
}

inline void ProtoBufMsg::ClearTerminalSensor(unsigned int index)
{
    TermPropertyList[index].clear_sensor_list();
}

inline void ProtoBufMsg::RemoveTerminalSensor(unsigned int index,int sensor_index)
{
    ::google::protobuf::RepeatedPtrField< ::LTMessage::TerminalProperty_Sensor > *repeated;
    repeated = TermPropertyList[index].mutable_sensor_list();
    repeated->erase(repeated->begin() + sensor_index);
}

inline void ProtoBufMsg::AddTerminalSensor(unsigned int index,int size)
{
    for(int i = 0;i < size;i ++)
    {
        TermPropertyList[index].add_sensor_list();
    }
}

inline void ProtoBufMsg::SetTerminalSensorIndex(unsigned int termIndex,int sensorIndex,int index)
{
    TerminalProperty_Sensor *sensor = TermPropertyList[termIndex].mutable_sensor_list(sensorIndex);
    sensor->set_index(index);
}

inline void ProtoBufMsg::SetTerminalSensorActive(unsigned int termIndex,int sensorIndex,int active)
{
    TerminalProperty_Sensor *sensor = TermPropertyList[termIndex].mutable_sensor_list(sensorIndex);
    sensor->set_active(active);
}

inline void ProtoBufMsg::SetTerminalSensorHeight(unsigned int termIndex,int sensorIndex,double height)
{
    TerminalProperty_Sensor *sensor = TermPropertyList[termIndex].mutable_sensor_list(sensorIndex);
    sensor->set_height(height);
}

inline void ProtoBufMsg::SetTerminalSensorPointX(unsigned int termIndex,int sensorIndex,double x)
{
    TerminalProperty_Sensor *sensor = TermPropertyList[termIndex].mutable_sensor_list(sensorIndex);
    PointXYZ *point = sensor->mutable_coordinates();
    point->set_x(x);
}

inline void ProtoBufMsg::SetTerminalSensorPointY(unsigned int termIndex,int sensorIndex,double y)
{
    TerminalProperty_Sensor *sensor = TermPropertyList[termIndex].mutable_sensor_list(sensorIndex);
    PointXYZ *point = sensor->mutable_coordinates();
    point->set_y(y);
}

inline void ProtoBufMsg::SetTerminalSensorPointZ(unsigned int termIndex,int sensorIndex,double z)
{
    TerminalProperty_Sensor *sensor = TermPropertyList[termIndex].mutable_sensor_list(sensorIndex);
    PointXYZ *point = sensor->mutable_coordinates();
    point->set_z(z);
}

inline void ProtoBufMsg::SetTerminalSensorPoint(unsigned int termIndex,int sensorIndex,double &x,double &y,double &z)
{
    TerminalProperty_Sensor *sensor = TermPropertyList[termIndex].mutable_sensor_list(sensorIndex);
    PointXYZ *point = sensor->mutable_coordinates();
    point->set_x(x);
    point->set_y(y);
    point->set_z(z);
}

inline void ProtoBufMsg::ClearTerminalReporter(unsigned int index)
{
    TermPropertyList[index].clear_report_ip_addr();
    TermPropertyList[index].clear_report_tcp_port();
}

inline void ProtoBufMsg::RemoveTerminalReporter(unsigned int index,int reporter_index)
{
    ::google::protobuf::RepeatedPtrField< ::std::string> *repeated_ip;
    repeated_ip = TermPropertyList[index].mutable_report_ip_addr();
    repeated_ip->erase(repeated_ip->begin() + reporter_index);

    ::google::protobuf::RepeatedField< ::google::protobuf::int32 >* repeated_port;
    repeated_port = TermPropertyList[index].mutable_report_tcp_port();
    repeated_port->erase(repeated_port->begin() + reporter_index);
}

inline void ProtoBufMsg::AddTerminalReporterIp(unsigned int index,string ip_addr)
{
    string *reporter_ip = TermPropertyList[index].add_report_ip_addr();
    *reporter_ip = ip_addr;
}

inline void ProtoBufMsg::SetTerminalReporterIp(unsigned int termIndex, int reporterIndex,string ip_addr)
{
    string *reporter_ip = TermPropertyList[termIndex].mutable_report_ip_addr(reporterIndex);
    *reporter_ip = ip_addr;
}

inline void ProtoBufMsg::AddTerminalReporterPort(unsigned int index,int port)
{
    TermPropertyList[index].add_report_tcp_port(port);
}

inline void ProtoBufMsg::SetTerminalReporterPort(unsigned int termIndex, int reporterIndex,int port)
{
    TermPropertyList[termIndex].set_report_tcp_port(reporterIndex,port);
}
/**********************************************************************************/


/***************************基站信息请求和响应接口函数*****************************/
inline int ProtoBufMsg::GetBsRespCode(void)
{
    return static_cast<int>(BsProperty_StatusCode);
}

inline string ProtoBufMsg::GetBsRespDetail(void)
{
    return BsProperty_StatusDetail;
}

inline unsigned int ProtoBufMsg::GetBaseStationSize(void)
{
    return BsPropertyList.size();
}

inline string ProtoBufMsg::GetBsChipId(unsigned int index)
{
    return BsPropertyList[index].chip_id();
}

inline string ProtoBufMsg::GetBsHostName(unsigned int index)
{
    return BsPropertyList[index].host_name();
}

inline int ProtoBufMsg::GetBsId(unsigned int index)
{
    return BsPropertyList[index].bs_id();
}

inline int ProtoBufMsg::GetBsIngroupAddr(unsigned int index)
{
    return BsPropertyList[index].ingroup_addr();
}

inline int ProtoBufMsg::GetBsFreq(unsigned int index)
{
    return BsPropertyList[index].frequency();
}

inline double ProtoBufMsg::GetBsStartAngle(unsigned int index)
{
    return BsPropertyList[index].start_angle();
}

inline double ProtoBufMsg::GetBsCoordinateX(unsigned int index)
{
    return BsPropertyList[index].coordinates().x();
}

inline double ProtoBufMsg::GetBsCoordinateY(unsigned int index)
{
    return BsPropertyList[index].coordinates().y();
}

inline double ProtoBufMsg::GetBsCoordinateZ(unsigned int index)
{
    return BsPropertyList[index].coordinates().z();
}

inline double ProtoBufMsg::GetBsOrientation_pitch(unsigned int index)
{
    return BsPropertyList[index].orientation().pitch();
}

inline double ProtoBufMsg::GetBsOrientation_roll(unsigned int index)
{
    return BsPropertyList[index].orientation().roll();
}

inline double ProtoBufMsg::GetBsOrientation_yaw(unsigned int index)
{
    return BsPropertyList[index].orientation().yaw();
}

inline int ProtoBufMsg::GetBsLedStatus(unsigned int index)
{
    return BsPropertyList[index].led_status();
}

inline double ProtoBufMsg::GetBsMotorSpeed(unsigned int index)
{
    return BsPropertyList[index].motor_speed();
}

inline void ProtoBufMsg::ClearBsProperty(void)
{
    for(unsigned int i = 0;i < BsPropertyList.size();i ++)
    {
        BsPropertyList[i].Clear();
    }
    BsPropertyList.clear();
}

inline void ProtoBufMsg::AddBsProperty(unsigned int size)
{
    BaseStationProperty BsProperty;
    vector<BaseStationProperty>::iterator it = BsPropertyList.end();
    BsPropertyList.insert(it,size,BsProperty);
}

inline void ProtoBufMsg::SetBsChipId(unsigned int index,string &chip_id)
{
    BsPropertyList[index].set_chip_id(chip_id);
}

inline void ProtoBufMsg::SetBsHostName(unsigned int index,string &name)
{
    BsPropertyList[index].set_host_name(name);
}

inline void ProtoBufMsg::SetBsId(unsigned int ListIndex, int BsId)
{
    BsPropertyList[ListIndex].set_bs_id(BsId);
}

inline void ProtoBufMsg::SetBsIngroupAddr(unsigned int index,int ingroup_addr)
{
    BsPropertyList[index].set_ingroup_addr(ingroup_addr);
}

inline void ProtoBufMsg::SetBsFreq(unsigned int index, int freq)
{
    BsPropertyList[index].set_frequency(freq);
}

inline void ProtoBufMsg::SetBsStartAngle(unsigned int index,double angle)
{
    BsPropertyList[index].set_start_angle(angle);
}

inline void ProtoBufMsg::SetBsCoodinate(unsigned int index,double x,double y,double z)
{
    PointXYZ *point = BsPropertyList[index].mutable_coordinates();
    point->set_x(x);
    point->set_y(y);
    point->set_z(z);
}

inline void ProtoBufMsg::SetBsCoodinateX(unsigned int index,double x)
{
    PointXYZ *point = BsPropertyList[index].mutable_coordinates();
    point->set_x(x);
}

inline void ProtoBufMsg::SetBsCoodinateY(unsigned int index, double y)
{
    PointXYZ *point = BsPropertyList[index].mutable_coordinates();
    point->set_y(y);
}

inline void ProtoBufMsg::SetBsCoodinateZ(unsigned int index,double z)
{
    PointXYZ *point = BsPropertyList[index].mutable_coordinates();
    point->set_z(z);
}

inline void ProtoBufMsg::SetBsOrientationPitch(unsigned int index,double pitch)
{
    BaseStationProperty_Pose *pose = BsPropertyList[index].mutable_orientation();
    pose->set_pitch(pitch);
}

inline void ProtoBufMsg::SetBsOrientationRoll(unsigned int index,double roll)
{
    BaseStationProperty_Pose *pose = BsPropertyList[index].mutable_orientation();
    pose->set_roll(roll);
}

inline void ProtoBufMsg::SetBsOrientationYaw(unsigned int index, double yaw)
{
    BaseStationProperty_Pose *pose = BsPropertyList[index].mutable_orientation();
    pose->set_yaw(yaw);
}

inline void ProtoBufMsg::SetBsOrientation(unsigned int index,double pitch,double roll,double yaw)
{
    BaseStationProperty_Pose *pose = BsPropertyList[index].mutable_orientation();
    pose->set_pitch(pitch);
    pose->set_roll(roll);
    pose->set_yaw(yaw);
}

inline void ProtoBufMsg::SetBsLedStatus(unsigned int index,int led_status)
{
    BsPropertyList[index].set_led_status(led_status);
}

inline void ProtoBufMsg::SetMotorSpeed(unsigned int index,double motor_speed)
{
    BsPropertyList[index].set_motor_speed(motor_speed);
}
/**********************************************************************************/


/*****************************解析终端发布的定位信息接口函数***********************/
inline unsigned int ProtoBufMsg::GetLocationSize(void)
{
    return publish_location.size();
}

inline unsigned int ProtoBufMsg::GetLocationSize(string &index,int type)
{
    switch(type)
    {
    case sensor_raw_type:
    {
        if(sensor_raw_location.find(index) == sensor_raw_location.end())
        {
            return 0;
        }
        return sensor_raw_location[index].size();
    }
    case sensor_filter_type:
    {
        if(sensor_filter_location.find(index) == sensor_filter_location.end())
        {
            return 0;
        }
        return sensor_filter_location[index].size();
    }
    case robot_filter_type:
    {
        if(robot_filter_location.find(index) == robot_filter_location.end())
        {
            return 0;
        }
        return robot_filter_location[index].size();
    }
    case sensor_filter_noimu_type:
    {
        if(sensor_filter_noimu_location.find(index) == sensor_filter_noimu_location.end())
        {
            return 0;
        }
        return sensor_filter_noimu_location[index].size();
    }
    case robot_filter_noimu_type:
    {
        if(robot_filter_noimu_location.find(index) == robot_filter_noimu_location.end())
        {
            return 0;
        }
        return robot_filter_noimu_location[index].size();
    }
    case sensor_raw_unregistered_type:
    {
        if(sensor_raw_unregistered_location.find(index) == sensor_raw_unregistered_location.end())
        {
            return 0;
        }
        return sensor_raw_unregistered_location[index].size();
    }
    case sensor_filter0_type:
    {
        if(sensor_raw_filter0_location.find(index) == sensor_raw_filter0_location.end())
        {
            return 0;
        }
        return sensor_raw_filter0_location[index].size();
    }
    case sensor_filter1_type:
    {
        if(sensor_raw_filter1_location.find(index) == sensor_raw_filter1_location.end())
        {
            return 0;
        }
        return sensor_raw_filter1_location[index].size();
    }
    default:
    {
        return 0;
    }
    }
}

inline unsigned int ProtoBufMsg::GetLocationSize(int type)
{
    switch(type)
    {
    case sensor_raw_type:
    case sensor_raw_unregistered_type:
    {
        return sensor_raw_location_.size();
    }
    case sensor_filter_type:
    case sensor_filter_noimu_type:
    case sensor_filter0_type:
    case sensor_filter1_type:
    {
        return sensor_filter_location_.size();
    }
    case robot_filter_type:
    case robot_filter_noimu_type:
    {
        return robot_filter_location_.size();
    }
    default:
    {
        return 0;
    }
    }
}

inline bool ProtoBufMsg::CheckLocationSize(void)
{
    if(sensor_raw_location.size() > 0)
    {
        return true;
    }

    if(sensor_filter_location.size() > 0)
    {
        return true;
    }

    if(robot_filter_location.size() > 0)
    {
        return true;
    }

    if(sensor_filter_noimu_location.size() > 0)
    {
        return true;
    }

    if(robot_filter_noimu_location.size() > 0)
    {
        return true;
    }

    if(sensor_raw_unregistered_location.size() > 0)
    {
        return true;
    }

    if(sensor_raw_filter0_location.size() > 0)
    {
        return true;
    }

    if(sensor_raw_filter1_location.size() > 0)
    {
        return true;
    }

    return false;
}

inline int ProtoBufMsg::GetLocationType(unsigned int index)
{
    return publish_location[index].type();
}

inline int ProtoBufMsg::TranslateLocationType(int type)
{
    switch(static_cast<Location_LocationType>(type))
    {
    case Location_LocationType_LOC_SENSOR_RAW:
    {
        return sensor_raw_type;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED:
    {
        return sensor_filter_type;
    }
    case Location_LocationType_LOC_ROBOT_FILTERED:
    {
        return robot_filter_type;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED_NOIMU:
    {
        return sensor_filter_noimu_type;
    }
    case Location_LocationType_LOC_ROBOT_FILTERED_NOIMU:
    {
        return robot_filter_noimu_type;
    }
    case Location_LocationType_LOC_SENSOR_RAW_UNREGISTERED_BS:
    {
        return sensor_raw_unregistered_type;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED0:
    {
        return sensor_filter0_type;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED1:
    {
        return sensor_filter1_type;
    }
    default:
    {
        return 0;
    }
    }
}

inline void ProtoBufMsg::GetLocationCoordinate(unsigned int index,double &x,double &y,double &z)
{
    x = publish_location[index].coordinates().x();
    y = publish_location[index].coordinates().y();
    z = publish_location[index].coordinates().z();
}

inline bool ProtoBufMsg::GetLocationCoordinate(string &index,unsigned int loc_index,int type,
                                               double &x,double &y,double &z)
{
    switch(type)
    {
    case sensor_raw_type:
    {
        if(sensor_raw_location.find(index) == sensor_raw_location.end())
        {
            return false;
        }
        x = sensor_raw_location[index][loc_index].coordinates().x();
        y = sensor_raw_location[index][loc_index].coordinates().y();
        z = sensor_raw_location[index][loc_index].coordinates().z();
        break;
    }
    case sensor_filter_type:
    {
        if(sensor_filter_location.find(index) == sensor_filter_location.end())
        {
            return false;
        }
        x = sensor_filter_location[index][loc_index].coordinates().x();
        y = sensor_filter_location[index][loc_index].coordinates().y();
        z = sensor_filter_location[index][loc_index].coordinates().z();
        break;
    }
    case robot_filter_type:
    {
        if(robot_filter_location.find(index) == robot_filter_location.end())
        {
            return false;
        }
        x = robot_filter_location[index][loc_index].coordinates().x();
        y = robot_filter_location[index][loc_index].coordinates().y();
        z = robot_filter_location[index][loc_index].coordinates().z();
        break;
    }
    case sensor_filter_noimu_type:
    {
        if(sensor_filter_noimu_location.find(index) == sensor_filter_noimu_location.end())
        {
            return false;
        }
        x = sensor_filter_noimu_location[index][loc_index].coordinates().x();
        y = sensor_filter_noimu_location[index][loc_index].coordinates().y();
        z = sensor_filter_noimu_location[index][loc_index].coordinates().z();
        break;
    }
    case robot_filter_noimu_type:
    {
        if(robot_filter_noimu_location.find(index) == robot_filter_noimu_location.end())
        {
            return false;
        }
        x = robot_filter_noimu_location[index][loc_index].coordinates().x();
        y = robot_filter_noimu_location[index][loc_index].coordinates().y();
        z = robot_filter_noimu_location[index][loc_index].coordinates().z();
        break;
    }
    case sensor_raw_unregistered_type:
    {
        if(sensor_raw_unregistered_location.find(index) == sensor_raw_unregistered_location.end())
        {
            return false;
        }
        x = sensor_raw_unregistered_location[index][loc_index].coordinates().x();
        y = sensor_raw_unregistered_location[index][loc_index].coordinates().y();
        z = sensor_raw_unregistered_location[index][loc_index].coordinates().z();
        break;
    }
    case sensor_filter0_type:
    {
        if(sensor_raw_filter0_location.find(index) == sensor_raw_filter0_location.end())
        {
            return false;
        }
        x = sensor_raw_filter0_location[index][loc_index].coordinates().x();
        y = sensor_raw_filter0_location[index][loc_index].coordinates().y();
        z = sensor_raw_filter0_location[index][loc_index].coordinates().z();
        break;
    }
    case sensor_filter1_type:
    {
        if(sensor_raw_filter1_location.find(index) == sensor_raw_filter1_location.end())
        {
            return false;
        }
        x = sensor_raw_filter1_location[index][loc_index].coordinates().x();
        y = sensor_raw_filter1_location[index][loc_index].coordinates().y();
        z = sensor_raw_filter1_location[index][loc_index].coordinates().z();
        break;
    }
    default:
    {
        return false;
    }
    }

    return true;
}

inline void ProtoBufMsg::GetLocationCoordinate(unsigned int index,int type,double &x,double &y,double &z)
{
    switch(type)
    {
    case sensor_raw_type:
    case sensor_raw_unregistered_type:
    {
        x = sensor_raw_location_[index].coordinates().x();
        y = sensor_raw_location_[index].coordinates().y();
        z = sensor_raw_location_[index].coordinates().z();
        break;
    }
    case sensor_filter_type:
    case sensor_filter_noimu_type:
    case sensor_filter0_type:
    case sensor_filter1_type:
    {
        x = sensor_filter_location_[index].coordinates().x();
        y = sensor_filter_location_[index].coordinates().y();
        z = sensor_filter_location_[index].coordinates().z();
        break;
    }
    case robot_filter_type:
    case robot_filter_noimu_type:
    {
        x = robot_filter_location_[index].coordinates().x();
        y = robot_filter_location_[index].coordinates().y();
        z = robot_filter_location_[index].coordinates().z();
        break;
    }
    default:
    {
        break;
    }
    }
}

inline double ProtoBufMsg::GetLocationHeading(unsigned int index)
{
    return publish_location[index].heading();
}

inline bool ProtoBufMsg::GetLocationHeading(string &index,unsigned int loc_index,int type,double &heading)
{
    switch(type)
    {
    case sensor_raw_type:
    {
        if(sensor_raw_location.find(index) == sensor_raw_location.end())
        {
            return false;
        }
        heading = sensor_raw_location[index][loc_index].heading();
        break;
    }
    case sensor_filter_type:
    {
        if(sensor_filter_location.find(index) == sensor_filter_location.end())
        {
            return false;
        }
        heading = sensor_filter_location[index][loc_index].heading();
        break;
    }
    case robot_filter_type:
    {
        if(robot_filter_location.find(index) == robot_filter_location.end())
        {
            return false;
        }
        heading = robot_filter_location[index][loc_index].heading();
        break;
    }
    case sensor_filter_noimu_type:
    {
        if(sensor_filter_noimu_location.find(index) == sensor_filter_noimu_location.end())
        {
            return false;
        }
        heading = sensor_filter_noimu_location[index][loc_index].heading();
        break;
    }
    case robot_filter_noimu_type:
    {
        if(robot_filter_noimu_location.find(index) == robot_filter_noimu_location.end())
        {
            return false;
        }
        heading = robot_filter_noimu_location[index][loc_index].heading();
        break;
    }
    case sensor_raw_unregistered_type:
    {
        if(sensor_raw_unregistered_location.find(index) == sensor_raw_unregistered_location.end())
        {
            return false;
        }
        heading = sensor_raw_unregistered_location[index][loc_index].heading();
        break;
    }
    case sensor_filter0_type:
    {
        if(sensor_raw_filter0_location.find(index) == sensor_raw_filter0_location.end())
        {
            return false;
        }
        heading = sensor_raw_filter0_location[index][loc_index].heading();
        break;
    }
    case sensor_filter1_type:
    {
        if(sensor_raw_filter1_location.find(index) == sensor_raw_filter1_location.end())
        {
            return false;
        }
        heading = sensor_raw_filter1_location[index][loc_index].heading();
        break;
    }
    default:
    {
        return false;
    }
    }

    return true;
}

inline double ProtoBufMsg::GetLocationHeading(unsigned int index,int type)
{
    switch(type)
    {
    case sensor_raw_type:
    case sensor_raw_unregistered_type:
    {
        return sensor_raw_location_[index].heading();
    }
    case sensor_filter_type:
    case sensor_filter_noimu_type:
    case sensor_filter0_type:
    case sensor_filter1_type:
    {
        return sensor_filter_location_[index].heading();
    }
    case robot_filter_type:
    case robot_filter_noimu_type:
    {
        return robot_filter_location_[index].heading();
    }
    default:
    {
        return 0;
    }
    }
}

inline double ProtoBufMsg::GetLocationGamma(unsigned int index)
{
    return publish_location[index].gamma();
}

inline double ProtoBufMsg::GetLocationTheta(unsigned int index)
{
    return publish_location[index].theta();
}

inline double ProtoBufMsg::GetLocationTs(unsigned int index)
{
    return publish_location[index].ts();
}

inline int ProtoBufMsg::GetLocationBsIndex(unsigned int index)
{
    return publish_location[index].bs_index();
}

inline int ProtoBufMsg::GetLocationSensorId(unsigned int index)
{
    return publish_location[index].sensor_id();
}

inline bool ProtoBufMsg::GetLocationSensorId(string &index,unsigned int loc_index,int type,int &sensor_id)
{
    switch(type)
    {
    case sensor_raw_type:
    {
        if(sensor_raw_location.find(index) == sensor_raw_location.end())
        {
            return false;
        }
        sensor_id = sensor_raw_location[index][loc_index].sensor_id();
        break;
    }
    case sensor_filter_type:
    {
        if(sensor_filter_location.find(index) == sensor_filter_location.end())
        {
            return false;
        }
        sensor_id = sensor_filter_location[index][loc_index].sensor_id();
        break;
    }
    case robot_filter_type:
    {
        if(robot_filter_location.find(index) == robot_filter_location.end())
        {
            return false;
        }
        sensor_id = robot_filter_location[index][loc_index].sensor_id();
        break;
    }
    case sensor_filter_noimu_type:
    {
        if(sensor_filter_noimu_location.find(index) == sensor_filter_noimu_location.end())
        {
            return false;
        }
        sensor_id = sensor_filter_noimu_location[index][loc_index].sensor_id();
        break;
    }
    case robot_filter_noimu_type:
    {
        if(robot_filter_noimu_location.find(index) == robot_filter_noimu_location.end())
        {
            return false;
        }
        sensor_id = robot_filter_noimu_location[index][loc_index].sensor_id();
        break;
    }
    case sensor_raw_unregistered_type:
    {
        if(sensor_raw_unregistered_location.find(index) == sensor_raw_unregistered_location.end())
        {
            return false;
        }
        sensor_id = sensor_raw_unregistered_location[index][loc_index].sensor_id();
        break;
    }
    case sensor_filter0_type:
    {
        if(sensor_raw_filter0_location.find(index) == sensor_raw_filter0_location.end())
        {
            return false;
        }
        sensor_id = sensor_raw_filter0_location[index][loc_index].sensor_id();
        break;
    }
    case sensor_filter1_type:
    {
        if(sensor_raw_filter1_location.find(index) == sensor_raw_filter1_location.end())
        {
            return false;
        }
        sensor_id = sensor_raw_filter1_location[index][loc_index].sensor_id();
        break;
    }
    default:
    {
        return false;
    }
    }

    return true;
}

inline int ProtoBufMsg::GetLocationSensorChannelId(unsigned int index)
{
    return publish_location[index].sensor_channel_id();
}

inline string ProtoBufMsg::GetLocationTerminalId(unsigned int index)
{
    return publish_location[index].terminal_id();
}

inline unsigned int ProtoBufMsg::GetLocateTimes(string &terminal_id,int type)
{
    switch(type)
    {
    case sensor_raw_type:
    case sensor_raw_unregistered_type:
    {
        if(sensor_raw_locate_times_.find(terminal_id) != sensor_raw_locate_times_.end())
        {
            return sensor_raw_locate_times_[terminal_id];
        }
        else
        {
            return 0;
        }
    }
    case sensor_filter_type:
    case sensor_filter_noimu_type:
    case sensor_filter0_type:
    case sensor_filter1_type:
    {
        if(sensor_filter_locate_times_.find(terminal_id) != sensor_filter_locate_times_.end())
        {
            return sensor_filter_locate_times_[terminal_id];
        }
        else
        {
            return 0;
        }
    }
    case robot_filter_type:
    case robot_filter_noimu_type:
    {
        if(robot_filter_locate_times_.find(terminal_id) != robot_filter_locate_times_.end())
        {
            return robot_filter_locate_times_[terminal_id];
        }
        else
        {
            return 0;
        }
    }
    default:
    {
        return 0;
    }
    }
}

inline void ProtoBufMsg::ClearLocateTimes(void)
{
    sensor_raw_locate_times_.clear();
    sensor_filter_locate_times_.clear();
    robot_filter_locate_times_.clear();
}

inline void ProtoBufMsg::ClearLocation(void)
{
    publish_location.clear();
    sensor_raw_location.clear();
    sensor_filter_location.clear();
    robot_filter_location.clear();
    sensor_filter_noimu_location.clear();
    robot_filter_noimu_location.clear();
    sensor_raw_unregistered_location.clear();
    sensor_raw_filter0_location.clear();
    sensor_raw_filter1_location.clear();
    sensor_raw_location_.clear();
    sensor_filter_location_.clear();
    robot_filter_location_.clear();
}

inline void ProtoBufMsg::ClearLocation(int type)
{
    map<string,vector<Location>>::iterator it;

    publish_location.clear();

    if(type & sensor_raw_type)
    {
        for(it = sensor_raw_location.begin();it != sensor_raw_location.end();it ++)
        {
            sensor_raw_location[it->first].clear();
        }
    }
    if(type & sensor_filter_type)
    {
        for(it = sensor_filter_location.begin();it != sensor_filter_location.end();it ++)
        {
            sensor_filter_location[it->first].clear();
        }
    }
    if(type & robot_filter_type)
    {
        for(it = robot_filter_location.begin();it != robot_filter_location.end();it ++)
        {
            robot_filter_location[it->first].clear();
        }
    }
    if(type & sensor_filter_noimu_type)
    {
        for(it = sensor_filter_noimu_location.begin();it != sensor_filter_noimu_location.end();it ++)
        {
            sensor_filter_noimu_location[it->first].clear();
        }
    }
    if(type & robot_filter_noimu_type)
    {
        for(it = robot_filter_noimu_location.begin();it != robot_filter_noimu_location.end();it ++)
        {
            robot_filter_noimu_location[it->first].clear();
        }
    }
    if(type & sensor_raw_unregistered_type)
    {
        for(it = sensor_raw_unregistered_location.begin();it != sensor_raw_unregistered_location.end();it ++)
        {
            sensor_raw_unregistered_location[it->first].clear();
        }
    }
    if(type & sensor_filter0_type)
    {
        for(it = sensor_raw_filter0_location.begin();it != sensor_raw_filter0_location.end();it ++)
        {
            sensor_raw_filter0_location[it->first].clear();
        }
    }
    if(type & sensor_filter1_type)
    {
        for(it = sensor_raw_filter1_location.begin();it != sensor_raw_filter1_location.end();it ++)
        {
            sensor_raw_filter1_location[it->first].clear();
        }
    }
}

inline void ProtoBufMsg::EraseFrontLocation(int range)
{
    publish_location.erase(publish_location.begin(),publish_location.begin() + range);
}

inline void ProtoBufMsg::EraseFrontLocation(string &index,int type,unsigned int range)
{
    vector<Location>::iterator it;

    switch(type)
    {
    case sensor_raw_type:
    {
        if(sensor_raw_location.find(index) == sensor_raw_location.end())
        {
            return;
        }
        it = sensor_raw_location[index].begin();
        sensor_raw_location[index].erase(it,it + range);
        break;
    }
    case sensor_filter_type:
    {
        if(sensor_filter_location.find(index) == sensor_filter_location.end())
        {
            return;
        }
        it = sensor_filter_location[index].begin();
        sensor_filter_location[index].erase(it,it + range);
        break;
    }
    case robot_filter_type:
    {
        if(robot_filter_location.find(index) == robot_filter_location.end())
        {
            return;
        }
        it = robot_filter_location[index].begin();
        robot_filter_location[index].erase(it,it + range);
        break;
    }
    case sensor_filter_noimu_type:
    {
        if(sensor_filter_noimu_location.find(index) == sensor_filter_noimu_location.end())
        {
            return;
        }
        it = sensor_filter_noimu_location[index].begin();
        sensor_filter_noimu_location[index].erase(it,it + range);
        break;
    }
    case robot_filter_noimu_type:
    {
        if(robot_filter_noimu_location.find(index) == robot_filter_noimu_location.end())
        {
            return;
        }
        it = robot_filter_noimu_location[index].begin();
        robot_filter_noimu_location[index].erase(it,it + range);
        break;
    }
    case sensor_raw_unregistered_type:
    {
        if(sensor_raw_unregistered_location.find(index) == sensor_raw_unregistered_location.end())
        {
            return;
        }
        it = sensor_raw_unregistered_location[index].begin();
        sensor_raw_unregistered_location[index].erase(it,it + range);
        break;
    }
    case sensor_filter0_type:
    {
        if(sensor_raw_filter0_location.find(index) == sensor_raw_filter0_location.end())
        {
            return;
        }
        it = sensor_raw_filter0_location[index].begin();
        sensor_raw_filter0_location[index].erase(it,it + range);
        break;
    }
    case sensor_filter1_type:
    {
        if(sensor_raw_filter1_location.find(index) == sensor_raw_filter1_location.end())
        {
            return;
        }
        it = sensor_raw_filter1_location[index].begin();
        sensor_raw_filter1_location[index].erase(it,it + range);
        break;
    }
    default:
    {
        break;
    }
    }
}

inline void ProtoBufMsg::AddLocationFilter(string &filter)
{
    if(sensor_raw_location.find(filter) != sensor_raw_location.end())
    {
        sensor_raw_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
    if(sensor_filter_location.find(filter) != sensor_filter_location.end())
    {
        sensor_filter_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
    if(robot_filter_location.find(filter) != robot_filter_location.end())
    {
        robot_filter_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
    if(sensor_filter_noimu_location.find(filter) != sensor_filter_noimu_location.end())
    {
        sensor_filter_noimu_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
    if(robot_filter_noimu_location.find(filter) != robot_filter_noimu_location.end())
    {
        robot_filter_noimu_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
    if(sensor_raw_unregistered_location.find(filter) != sensor_raw_unregistered_location.end())
    {
        sensor_raw_unregistered_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
    if(sensor_raw_filter0_location.find(filter) != sensor_raw_filter0_location.end())
    {
        sensor_raw_filter0_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
    if(sensor_raw_filter1_location.find(filter) != sensor_raw_filter1_location.end())
    {
        sensor_raw_filter1_location.insert(pair<string,vector<Location>>(filter,vector<Location>()));
    }
}

inline void ProtoBufMsg::AddLocationFilters(vector<string> &filters)
{
    for(unsigned int i = 0;i < filters.size();i ++)
    {
        if(sensor_raw_location.find(filters[i]) == sensor_raw_location.end())
        {
            sensor_raw_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
        if(sensor_filter_location.find(filters[i]) == sensor_filter_location.end())
        {
            sensor_filter_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
        if(robot_filter_location.find(filters[i]) == robot_filter_location.end())
        {
            robot_filter_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
        if(sensor_filter_noimu_location.find(filters[i]) == sensor_filter_noimu_location.end())
        {
            sensor_filter_noimu_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
        if(robot_filter_noimu_location.find(filters[i]) == robot_filter_noimu_location.end())
        {
            robot_filter_noimu_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
        if(sensor_raw_unregistered_location.find(filters[i]) == sensor_raw_unregistered_location.end())
        {
            sensor_raw_unregistered_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
        if(sensor_raw_filter0_location.find(filters[i]) == sensor_raw_filter0_location.end())
        {
            sensor_raw_filter0_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
        if(sensor_raw_filter1_location.find(filters[i]) == sensor_raw_filter1_location.end())
        {
            sensor_raw_filter1_location.insert(pair<string,vector<Location>>(filters[i],vector<Location>()));
        }
    }
}

inline void ProtoBufMsg::EraseLocationFilter(string &filter)
{
    map<string,vector<Location>>::iterator it;

    it = sensor_raw_location.find(filter);
    if(it != sensor_raw_location.end())
    {
        sensor_raw_location.erase(it);
    }

    it = sensor_filter_location.find(filter);
    if(it != sensor_filter_location.end())
    {
        sensor_filter_location.erase(it);
    }

    it = robot_filter_location.find(filter);
    if(it != robot_filter_location.end())
    {
        robot_filter_location.erase(it);
    }

    it = sensor_filter_noimu_location.find(filter);
    if(it != sensor_filter_noimu_location.end())
    {
        sensor_filter_noimu_location.erase(it);
    }

    it = robot_filter_noimu_location.find(filter);
    if(it != robot_filter_noimu_location.end())
    {
        robot_filter_noimu_location.erase(it);
    }

    it = sensor_raw_unregistered_location.find(filter);
    if(it != sensor_raw_unregistered_location.end())
    {
        sensor_raw_unregistered_location.erase(it);
    }

    it = sensor_raw_filter0_location.find(filter);
    if(it != sensor_raw_filter0_location.end())
    {
        sensor_raw_filter0_location.erase(it);
    }

    it = sensor_raw_filter1_location.find(filter);
    if(it != sensor_raw_filter1_location.end())
    {
        sensor_raw_filter1_location.erase(it);
    }
}

inline void ProtoBufMsg::EraseLocationFiltersByType(vector<string> &filters,
                                                    map<string,vector<Location>> &location)
{
    map<string,vector<Location>>::iterator it_map;
    vector<string>::iterator it_vector;
    vector<string> filters_erased;

    it_map = location.begin();
    while(it_map != location.end())
    {
        it_vector = std::find(filters.begin(),filters.end(),it_map->first);
        if(it_vector == filters.end())
        {
            filters_erased.push_back(it_map->first);
        }
        it_map ++;
    }
    for(unsigned int i = 0;i < filters_erased.size();i ++)
    {
        location.erase(filters_erased[i]);
    }
}

inline void ProtoBufMsg::EraseLocationFilters(vector<string> &filters)
{
    EraseLocationFiltersByType(filters,sensor_raw_location);
    EraseLocationFiltersByType(filters,sensor_filter_location);
    EraseLocationFiltersByType(filters,robot_filter_location);
    EraseLocationFiltersByType(filters,sensor_filter_noimu_location);
    EraseLocationFiltersByType(filters,robot_filter_noimu_location);
    EraseLocationFiltersByType(filters,sensor_raw_unregistered_location);
    EraseLocationFiltersByType(filters,sensor_raw_filter0_location);
    EraseLocationFiltersByType(filters,sensor_raw_filter1_location);
}
/**********************************************************************************/

#endif // PROTOBUFMSG_H
