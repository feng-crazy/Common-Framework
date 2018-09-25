#include "ProtoBufMsg.h"
#include <list>
#include <sstream>
#include <QDebug>



ProtoBufMsg::ProtoBufMsg()
{

}

/***************************终端信息请求和响应函数*****************************/
PointXYZ ProtoBufMsg::GeneratePoint3DMsg(double x,double y,double z)
{
    PointXYZ point;
    point.set_x(x);
    point.set_y(y);
    point.set_z(z);

    return point;
}

TerminalProperty_Sensor ProtoBufMsg::GenerateSensorMsg(int32_t index,
                                                       int32_t active, double height, PointXYZ point)
{
    TerminalProperty_Sensor sensor;
    sensor.set_index(index);
    sensor.set_active(active);
    sensor.set_height(height);
    PointXYZ *allocated_point = sensor.mutable_coordinates();
    *allocated_point = point;

    return sensor;
}

ResponseStatus ProtoBufMsg::GenerateRespStatusMsg(ResponseStatus_StatusCode code,string detail)
{
    ResponseStatus respStatus;

    respStatus.set_code(code);
    respStatus.set_detail(detail);

    return respStatus;
}

TerminalProperty ProtoBufMsg::GenerateTermPropertyMsg(string &host_name,
                                                      string &ip_addr, string &mac_addr,
                                                      list<TerminalProperty_Sensor> &sensor_list, list<string> &report_ip_list,
                                                      list<int32_t> &report_port_list)
{
    TerminalProperty TermProperty;

    TermProperty.set_host_name(host_name);
    TermProperty.set_ip_addr(ip_addr);
    TermProperty.set_mac_addr(mac_addr);

    list<TerminalProperty_Sensor>::iterator i;
    for(i = sensor_list.begin();i != sensor_list.end();i ++)
    {
        TerminalProperty_Sensor *sensor;
        sensor = TermProperty.add_sensor_list();
        *sensor = *i;
    }

    list<string>::iterator j;
    for(j = report_ip_list.begin();j != report_ip_list.end();j ++)
    {
        const string report_ip = *j;
        TermProperty.add_report_ip_addr(report_ip);
    }

    list<int32_t>::iterator k;
    for(k = report_port_list.begin();k != report_port_list.end();k ++)
    {
        TermProperty.add_report_tcp_port(*k);
    }

    return TermProperty;
}

TerminalPropertyRequest ProtoBufMsg::GenerateTermPropertyReqMsg(int type,
                                                                vector<TerminalProperty> &TermPropertyList)
{
    TerminalPropertyRequest TermPropertyReq;

    TermPropertyReq.set_type(TerminalPropertyRequest_RequestType(type));
    vector<TerminalProperty>::iterator i;
    for(i = TermPropertyList.begin();i != TermPropertyList.end();i ++)
    {
        TerminalProperty *TermPropertyAdded = TermPropertyReq.add_terminal_list();
        *TermPropertyAdded = *i;
    }

    return TermPropertyReq;
}

bool ProtoBufMsg::TermPropertyReqPacked(string &PackReq)
{ 
    vector<TerminalProperty> terminal_property_list;
    TerminalPropertyRequest terminal_property_req;
    terminal_property_req = GenerateTermPropertyReqMsg(TerminalPropertyRequest_RequestType_GET,
                                                       terminal_property_list);

    return terminal_property_req.SerializeToString(&PackReq);
}

bool ProtoBufMsg::TermPropertyUpdatePacked(string &PackUpdate)
{
    TerminalPropertyRequest TermPropertyReq;
    TermPropertyReq = GenerateTermPropertyReqMsg(TerminalPropertyRequest_RequestType_UPDATE,TermPropertyList);

    return TermPropertyReq.SerializeToString(&PackUpdate);
}

bool ProtoBufMsg::TermPropertyRespUnPacked(string &UnPackResp)
{
    TerminalPropertyResponse TermPropertyResp;
    if(false == TermPropertyResp.ParseFromString(UnPackResp))
    {
        return false;
    }

    ResponseStatus RespStatus = TermPropertyResp.status();
    StatusCode = RespStatus.code();
    StatusDetail = RespStatus.detail();
    //    cout << "Terminal Property Get Resp : " << endl;
    //    cout << "code = " << StatusCode << " detail = " << StatusDetail << endl;

    TermPropertyList.clear();
    for(int i = 0;i < TermPropertyResp.terminal_list_size();i ++)
    {
        TerminalProperty term_property = TermPropertyResp.terminal_list(i);
        TermPropertyList.push_back(term_property);
        //        cout << "host name = " << term_property.host_name() << " ip = " << term_property.ip_addr() <<
        //                " mac = " << term_property.mac_addr() << endl;
        //        cout << " sensor index = " << term_property.sensor_list(0).index() <<
        //                " sensor active = " << term_property.sensor_list(0).active() <<
        //                " sensor height = " << term_property.sensor_list(0).height() <<
        //                " sensor point = " << term_property.sensor_list(0).coordinates().x() << " " <<
        //                term_property.sensor_list(0).coordinates().y() << " " <<
        //                term_property.sensor_list(0).coordinates().z() << endl;
        //        cout << " report ip = " << term_property.report_ip_addr(0) <<
        //                " report port = " << term_property.report_tcp_port(0) << endl;
    }

    return true;
}

/*************************终端控制请求和响应接口函数*******************************/
bool ProtoBufMsg::TermActionRestartHostPacked(int &sequence,vector<string> &terminal_mac,string &PackAction)
{
    TerminalActionRequest TermActionReq;
    TermActionReq.set_type(TerminalActionRequest_RequestType_RESTART_HOST);
    TermActionReq.set_sequence(sequence);
    for(unsigned int i = 0;i < terminal_mac.size();i ++)
    {
        TerminalProperty *p_terminal_property = TermActionReq.add_terminal_list();
        p_terminal_property->set_mac_addr(terminal_mac[i]);
    }
    return TermActionReq.SerializeToString(&PackAction);
}

bool ProtoBufMsg::TermActionRestartBinPacked(int &sequence,vector<string> &terminal_mac,string &PackAction)
{
    TerminalActionRequest TermActionReq;
    TermActionReq.set_type(TerminalActionRequest_RequestType_RESTART_BIN);
    TermActionReq.set_sequence(sequence);
    for(unsigned int i = 0;i < terminal_mac.size();i ++)
    {
        TerminalProperty *p_terminal_property = TermActionReq.add_terminal_list();
        p_terminal_property->set_mac_addr(terminal_mac[i]);
    }
    return TermActionReq.SerializeToString(&PackAction);
}

bool ProtoBufMsg::TermActionUpdateBinPacked(int &sequence,string &url,
                                            vector<string> &terminal_mac,string &PackAction)
{
    TerminalActionRequest TermActionReq;
    TermActionReq.set_type(TerminalActionRequest_RequestType_UPDATE_BIN);
    TermActionReq.set_sequence(sequence);
    TermActionReq.set_update_url(url);
    for(unsigned int i = 0;i < terminal_mac.size();i ++)
    {
        TerminalProperty *p_terminal_property = TermActionReq.add_terminal_list();
        p_terminal_property->set_mac_addr(terminal_mac[i]);
    }
    return TermActionReq.SerializeToString(&PackAction);
}

bool ProtoBufMsg::TermActionStartCalibrationPacked(int &sequence,vector<string> &terminal_mac,
                                                   string &PackAction)
{
    TerminalActionRequest TermActionReq;
    TermActionReq.set_type(TerminalActionRequest_RequestType_START_CALIBRATION);
    TermActionReq.set_sequence(sequence);
    for(unsigned int i = 0;i < terminal_mac.size();i ++)
    {
        TerminalProperty *p_terminal_property = TermActionReq.add_terminal_list();
        p_terminal_property->set_mac_addr(terminal_mac[i]);
    }
    return TermActionReq.SerializeToString(&PackAction);
}

bool ProtoBufMsg::TermActionStopCalibrationPacked(int &sequence,vector<string> &terminal_mac,
                                                  string &PackAction)
{
    TerminalActionRequest TermActionReq;
    TermActionReq.set_type(TerminalActionRequest_RequestType_STOP_CALIBRATION);
    TermActionReq.set_sequence(sequence);
    for(unsigned int i = 0;i < terminal_mac.size();i ++)
    {
        TerminalProperty *p_terminal_property = TermActionReq.add_terminal_list();
        p_terminal_property->set_mac_addr(terminal_mac[i]);
    }
    return TermActionReq.SerializeToString(&PackAction);
}

bool ProtoBufMsg::TermActionResponseUnPacked(string &UnpackResp,int &sequence,int &status,string &detail)
{
    TerminalActionResponse TermActionResp;
    if(false == TermActionResp.ParseFromString(UnpackResp))
    {
        return false;
    }

    sequence = TermActionResp.sequence();
    ResponseStatus respStatus = TermActionResp.status();
    status = respStatus.code();
    detail = respStatus.detail();
    cout << "Term Action : " << "sequence = " << sequence << " status = " << detail << endl;
    return true;
}

/***************************基站信息请求和响应函数*****************************/
BaseStationPropertyRequest ProtoBufMsg::GenerateBSPropertyReqMsg(int type,
                                                                 vector<BaseStationProperty> &BSPropertyList)
{
    BaseStationPropertyRequest BSPropertyReq;

    BSPropertyReq.set_type(BaseStationPropertyRequest_RequestType(type));
    vector<BaseStationProperty>::iterator i;
    for(i = BSPropertyList.begin();i != BSPropertyList.end();i ++)
    {
        BaseStationProperty *BSPropertyAdded = BSPropertyReq.add_base_station_list();
        *BSPropertyAdded = *i;
    }

    return BSPropertyReq;
}

bool ProtoBufMsg::BaseStationPropertyReqPacked(string &PackReq)
{
    vector<BaseStationProperty> base_station_property_list;
    BaseStationPropertyRequest base_station_property_req;
    base_station_property_req = GenerateBSPropertyReqMsg(BaseStationPropertyRequest_RequestType_GET_ALL,
                                                         base_station_property_list);

    return base_station_property_req.SerializeToString(&PackReq);
}

bool ProtoBufMsg::BaseStationPropertyActiveReqPacked(string &PackReq)
{
    vector<BaseStationProperty> base_station_property_list;
    BaseStationPropertyRequest base_station_property_req;
    base_station_property_req = GenerateBSPropertyReqMsg(BaseStationPropertyRequest_RequestType_GET_ACTIVE,
                                                         base_station_property_list);

    return base_station_property_req.SerializeToString(&PackReq);
}

bool ProtoBufMsg::BaseStationPropertyUpdatePacked(string &PackUpdate)
{
    BaseStationPropertyRequest BSPropertyReq;
    BSPropertyReq = GenerateBSPropertyReqMsg(BaseStationPropertyRequest_RequestType_UPDATE_ACTIVE,BsPropertyList);

    return BSPropertyReq.SerializeToString(&PackUpdate);
}

bool ProtoBufMsg::BaseStationPropertyRespUnPacked(string &UnPackResp)
{
    BaseStationPropertyResponse BsPropertyResp;
    if(false == BsPropertyResp.ParseFromString(UnPackResp))
    {
        return false;
    }

    ResponseStatus RespStatus = BsPropertyResp.status();
    BsProperty_StatusCode = RespStatus.code();
    BsProperty_StatusDetail = RespStatus.detail();
    //    cout << "code = " << BsProperty_StatusCode << " detail = " << BsProperty_StatusDetail << endl;

    BsPropertyList.clear();
    for(int i = 0;i < BsPropertyResp.base_station_list_size();i ++)
    {
        BaseStationProperty bs_property = BsPropertyResp.base_station_list(i);
        BsPropertyList.push_back(bs_property);
        //        cout << "host name = " << bs_property.host_name() << " group_ip = " << bs_property.ingroup_addr() <<
        //                " bs id = " << bs_property.bs_id() << " freq = " << bs_property.frequency() << endl;
        //        cout << " angle = " << bs_property.start_angle() <<
        //                " led status = " << bs_property.led_status() <<
        //                " motor speed = " << bs_property.motor_speed() << endl;
        //        cout << "coordinates = " << bs_property.coordinates().x() <<
        //                " " << bs_property.coordinates().y() <<
        //                " " << bs_property.coordinates().z() << endl;
        //        cout << "Pos = " << bs_property.orientation().pitch() <<
        //                " " << bs_property.orientation().roll() <<
        //                " " << bs_property.orientation().yaw() << endl;
    }

    return true;
}

/*************************基站控制请求和响应接口函数*******************************/
bool ProtoBufMsg::BsActionPacked(int &sequence, const string &command, string &PackAction)
{
    BaseStationActionRequest BsActionReq;
    BsActionReq.set_command(command);
    BsActionReq.set_sequence(sequence);

    return BsActionReq.SerializeToString(&PackAction);
}

bool ProtoBufMsg::BsActionResponseUnPacked(string &UnpackResp,int &sequence,int &status,
                                           string &detail,string &info)
{
    BaseStationActionResponse BsActionResp;
    if(false == BsActionResp.ParseFromString(UnpackResp))
    {
        return false;
    }

    sequence = BsActionResp.sequence();
    status = BsActionResp.status().code();
    detail = BsActionResp.status().detail();
    info = BsActionResp.bs_info();

    return true;
}

/**************************GUI心跳请求和响应接口函数*******************************/
bool ProtoBufMsg::HeartBeatReqPacked(int &sequence,string &mac,string &PackReq)
{
    GuiHeartbeatRequest HeartBeatReq;
    HeartBeatReq.set_mac_addr(mac);
    HeartBeatReq.set_sequence(sequence);
    return HeartBeatReq.SerializeToString(&PackReq);
}

bool ProtoBufMsg::HeartBeatRespUnPacked(string &UnpackResp, int &sequence,
                                        int &status, string &status_detail,
                                        int &event, string &event_detail)
{
    GuiHeartbeatResponse HeartBeatResp;
    if(false == HeartBeatResp.ParseFromString(UnpackResp))
    {
        return false;
    }

    sequence = HeartBeatResp.sequence();
    status = HeartBeatResp.status().code();
    status_detail = HeartBeatResp.status().detail();
    event = HeartBeatResp.event().event();
    event_detail = HeartBeatResp.event().detail();
    //    cout << "Heart Beat : " << " sequence = " << sequence << " status = " << status <<
    //            " statu detail = " << status_detail << " event = " << event <<
    //            " event detail = " << event_detail << endl;
    return true;
}

/**************************控制器广播信息接口函数*******************************/
bool ProtoBufMsg::ControllerBroadcastUnPacked(string &Unpack,string &ctrl_ip,
                                              int &req_port,int &sub_port,int &bs_type)
{
    ControllerBroadcast CtrlBroadcast;

    if(false == CtrlBroadcast.ParseFromString(Unpack))
    {
        return false;
    }

    ctrl_ip = CtrlBroadcast.ip_addr();
    req_port = CtrlBroadcast.manager_port();
    sub_port = CtrlBroadcast.loc_xpub_port();
    if(CtrlBroadcast.type() == ControllerBroadcast_ControllerType_MULTI)
    {
        bs_type = multi_base_station;
    }
    else if(CtrlBroadcast.type() == ControllerBroadcast_ControllerType_SINGLE)
    {
        bs_type = ControllerBroadcast_ControllerType_SINGLE;
    }
    else
    {
        bs_type = unknown_base_station;
    }
    return true;
}

bool ProtoBufMsg::ControllerBroadcastRespPacked(string &host_name,string &ip,string &mac,
                                                string &PackResp)
{
    BroadcastResponse TermBroadResp;
    TermBroadResp.set_type(BroadcastResponse_ResponseType_MANAGER_TYPE);
    TermBroadResp.set_host_name(host_name);
    TermBroadResp.set_ip_addr(ip);
    TermBroadResp.set_mac_addr(mac);

    if(false == TermBroadResp.SerializeToString(&PackResp))
    {
        return false;
    }

    return true;
}

/*****************************解析终端发布的定位信息接口函数***********************/
bool ProtoBufMsg::TermPubLocInfoUnpacked(string &UnpackLoc)
{
    Location location;
    if(false == location.ParseFromString(UnpackLoc))
    {
        return false;
    }
    publish_location.push_back(location);

    //    cout << "Location : " << "type = " << PubLoc.type() << " x = " << PubLoc.coordinates().x() <<
    //            " y = " << PubLoc.coordinates().y() << " z = " << PubLoc.coordinates().z() <<
    //            " heading = " << PubLoc.heading() << " gamma = " << PubLoc.gamma() <<
    //            " theth = " << PubLoc.theta() << " ts = " << PubLoc.ts() << " bs id = " << PubLoc.bs_id() <<
    //            " sensor id = " << PubLoc.sensor_id() << " sensor channel = " << PubLoc.sensor_channel_id() <<
    //            " terminal id = " << PubLoc.terminal_id() << endl;

    return true;
}

bool ProtoBufMsg::TermPubLocInfoUnpacked(string &UnpackLoc,string &terminal_id,int base_station_index,
                                         int type,bool &valid)
{
    Location location;
    if(false == location.ParseFromString(UnpackLoc))
    {
        return false;
    }

    if(location.terminal_id().compare(terminal_id) != 0)
    {
        valid = false;
        return true;
    }

    if(location.bs_index() != base_station_index)
    {
        valid = false;
        return true;
    }

    if(!(TranslateLocationType(location.type()) & type))
    {
        valid = false;
        return true;
    }
    publish_location.push_back(location);

    valid = true;
    return true;
}

bool ProtoBufMsg::TermPubLocInfoUnpacked(string &UnpackLoc,string &terminal_id,int type,bool &valid)
{
    Location location;
    if(false == location.ParseFromString(UnpackLoc))
    {
        return false;
    }

    if(location.terminal_id().compare(terminal_id) != 0)
    {
        valid = false;
        return true;
    }

    if(!(TranslateLocationType(location.type()) & type))
    {
        valid = false;
        return true;
    }
    switch(location.type())
    {
    case Location_LocationType_LOC_SENSOR_RAW:
    case Location_LocationType_LOC_SENSOR_RAW_UNREGISTERED_BS:
    {
        sensor_raw_location_.push_back(location);

        map<string,unsigned int>::iterator it_locate = sensor_raw_locate_times_.begin();
        it_locate = sensor_raw_locate_times_.find(location.terminal_id());
        if(it_locate != sensor_raw_locate_times_.end())
        {
            it_locate->second = it_locate->second + 1;
        }
        else
        {
            sensor_raw_locate_times_.insert(pair<string,unsigned int>(location.terminal_id(),1));
        }
        break;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED:
    case Location_LocationType_LOC_SENSOR_FILTERED_NOIMU:
    case Location_LocationType_LOC_SENSOR_FILTERED0:
    case Location_LocationType_LOC_SENSOR_FILTERED1:
    {
        sensor_filter_location_.push_back(location);

        map<string,unsigned int>::iterator it_locate = sensor_filter_locate_times_.begin();
        it_locate = sensor_filter_locate_times_.find(location.terminal_id());
        if(it_locate != sensor_filter_locate_times_.end())
        {
            it_locate->second = it_locate->second + 1;
        }
        else
        {
            sensor_filter_locate_times_.insert(pair<string,unsigned int>(location.terminal_id(),1));
        }
        break;
    }
    case Location_LocationType_LOC_ROBOT_FILTERED:
    case Location_LocationType_LOC_ROBOT_FILTERED_NOIMU:
    {
        robot_filter_location_.push_back(location);

        map<string,unsigned int>::iterator it_locate = robot_filter_locate_times_.begin();
        it_locate = robot_filter_locate_times_.find(location.terminal_id());
        if(it_locate != robot_filter_locate_times_.end())
        {
            it_locate->second = it_locate->second + 1;
        }
        else
        {
            robot_filter_locate_times_.insert(pair<string,unsigned int>(location.terminal_id(),1));
        }
        break;
    }
    default:
    {
        break;
    }
    }

    valid = true;
    return true;
}

bool ProtoBufMsg::TermPubLocInfoUnpacked(string &UnpackLoc,vector<string> &terminal_id_vector,
                                         int filter,bool &valid)
{
    Location location;
    if(false == location.ParseFromString(UnpackLoc))
    {
        return false;
    }
    vector<string>::iterator it;
    it = std::find(terminal_id_vector.begin(),terminal_id_vector.end(),location.terminal_id());
    if(it == terminal_id_vector.end())
    {
        valid = false;
        return true;
    }

    if(!(TranslateLocationType(location.type()) & filter))
    {
        valid = false;
        return true;
    }
    publish_location.push_back(location);

    valid = true;
    return true;
}

bool ProtoBufMsg::TermPubLocInfoUnpacked(string &UnpackLoc,int type,bool &valid)
{
    Location location;
    if(false == location.ParseFromString(UnpackLoc))
    {
        return false;
    }

    switch(location.type())
    {
    case Location_LocationType_LOC_SENSOR_RAW:
    {
        if(!(type & sensor_raw_type))
        {
            valid = false;
            return true;
        }
        if(sensor_raw_location.find(location.terminal_id()) == sensor_raw_location.end())
        {
            valid = false;
            return true;
        }
        sensor_raw_location[location.terminal_id()].push_back(location);
        break;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED:
    {
        if(!(type & sensor_filter_type))
        {
            valid = false;
            return true;
        }
        if(sensor_filter_location.find(location.terminal_id()) == sensor_filter_location.end())
        {
            valid = false;
            return true;
        }
        sensor_filter_location[location.terminal_id()].push_back(location);
        break;
    }
    case Location_LocationType_LOC_ROBOT_FILTERED:
    {
        if(!(type & robot_filter_type))
        {
            valid = false;
            return true;
        }
        if(robot_filter_location.find(location.terminal_id()) == robot_filter_location.end())
        {
            valid = false;
            return true;
        }
        robot_filter_location[location.terminal_id()].push_back(location);
        break;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED_NOIMU:
    {
        if(!(type & sensor_filter_noimu_type))
        {
            valid = false;
            return true;
        }
        if(sensor_filter_noimu_location.find(location.terminal_id()) == sensor_filter_noimu_location.end())
        {
            valid = false;
            return true;
        }
        sensor_filter_noimu_location[location.terminal_id()].push_back(location);
        break;
    }
    case Location_LocationType_LOC_ROBOT_FILTERED_NOIMU:
    {
        if(!(type & robot_filter_noimu_type))
        {
            valid = false;
            return true;
        }
        if(robot_filter_noimu_location.find(location.terminal_id()) == robot_filter_noimu_location.end())
        {
            valid = false;
            return true;
        }
        robot_filter_noimu_location[location.terminal_id()].push_back(location);
        break;
    }
    case Location_LocationType_LOC_SENSOR_RAW_UNREGISTERED_BS:
    {
        if(!(type & sensor_raw_unregistered_type))
        {
            valid = false;
            return true;
        }

        if(sensor_raw_unregistered_location.find(location.terminal_id()) == sensor_raw_unregistered_location.end())
        {
            valid = false;
            return true;
        }
        sensor_raw_unregistered_location[location.terminal_id()].push_back(location);
        break;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED0:
    {
        if(!(type & sensor_filter0_type))
        {
            valid = false;
            return true;
        }
        if(sensor_raw_filter0_location.find(location.terminal_id()) == sensor_raw_filter0_location.end())
        {
            valid = false;
            return true;
        }
        sensor_raw_filter0_location[location.terminal_id()].push_back(location);
        break;
    }
    case Location_LocationType_LOC_SENSOR_FILTERED1:
    {
        if(!(type & sensor_filter1_type))
        {
            valid = false;
            return true;
        }
        if(sensor_raw_filter1_location.find(location.terminal_id()) == sensor_raw_filter1_location.end())
        {
            valid = false;
            return true;
        }
        sensor_raw_filter1_location[location.terminal_id()].push_back(location);
        break;
    }
    default:
    {
        valid = false;
        return true;
    }
    }

    valid = true;
    return true;
}
