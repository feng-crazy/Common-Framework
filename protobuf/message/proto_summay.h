syntax = "proto3";

package LTMessage; 

//三维坐标，单位m
message PointXYZ {
	double x = 1;
	double y = 2;
	double z = 3;
}

//终端Publish定位结果
message Location {
  enum LocationType {
		LOC_SENSOR_RAW = 0;      //传感器原始位置
		LOC_SENSOR_FILTERED = 1; //传感器滤波位置
		LOC_ROBOT_FILTERED = 2;  //机器人滤波位置
		LOC_SENSOR_FILTERED_NOIMU = 3; //传感器滤波位置（无预测）
		LOC_ROBOT_FILTERED_NOIMU = 4;  //机器人滤波位置（无预测）
		LOC_SENSOR_RAW_UNREGISTERED_BS = 5;  //接收没有注册基站解析的位置
		LOC_SENSOR_FILTERED0 = 6; //传感器头0滤波位置
		LOC_SENSOR_FILTERED1 = 7; //传感器头1滤波位置

	}

	LocationType type = 1;  //位置类型
	PointXYZ coordinates = 2;
	double heading = 3; //单位radian
	double gamma = 4;   //单位radian
	double theta = 5;   //单位radian
	double ts = 6;      //单位s
	int32 bs_index = 7;
	int32 sensor_id = 8; //传感器号
	int32 sensor_channel_id = 9; //传感器通道号，一个通道可能对应多个sensor
	string terminal_id = 10; //终端的全局唯一id，比如接收机的mac地址
}

//对于请求/命令的响应状态
message ResponseStatus {
	enum StatusCode {
		OK = 0;
		FAIL = 1; 
		TERMINAL_NOTFOUND = 2;
		BASE_STATION_NOTFOUND = 3;
		NETWORK_FAILURE = 4;
		//其他错误码待扩展
	}
	StatusCode code = 1;
	string detail = 2;  //错误则包含错误描述，正确则包含响应内容。具体解析取决于上层约定
}

//系统状态更新事件 注意这里虽然是enum但是有一些事件是会同时存在的
message UpdateEvent {
	enum Event {
		NO_EVENT = 0;
		TERMINAL_LIST_UPDATE = 1; //通知GUI有终端的变动，让GUI重新来请求TerminalProperty list
		BASE_STATION_LIST_UPDATE = 2;  //通知GUI有基站的变动，让GUI重新来请求BaseStationProperty list
		CONFIG_UPDATE = 3; //detail为转化为string的config文件
		//其他事件码待扩展
	}
	Event event = 1;
	string detail = 2; //事件进一步信息，比如新的config信息
}

//终端的异常状态,多种异常有同时存在的情况，所以不能用enum
message TerminalExceptStatus{
	string mac_addr = 1;  // 终端的MAC地址 终端唯一标识
	bool data_hub_is_dead = 2; // except_detail为异常信息，比如core dump或者死亡堆栈
	bool loc_engine_is_dead = 3;
	bool config_error = 4;  // 错误的详细信息
	bool sensor_change = 5; //传感器有插拔的动作，可能是异常断开
	repeated string except_detail = 6; // 异常的详细说明，如果同时存在多个异常，以这个序列列表排列
	string data_hub_check_info = 7;  // 终端datahub运行时检查信息
	string loc_engine_check_info = 8;  // 终端LocEngine运行时检查信息
}

//基站的异常状态 todo:基站异常待扩展
message BasesExceptStatus{
	// 基站的芯片id 刚接入的基站可能是和当前基站重复的频率，
	//组地址，和hostname，所以需要芯片id来识别唯一的标识
	string chip_id = 1;  
	repeated string except_detail = 2; // 异常的详细说明，如果同时存在多个异常，以这个序列列表排列
}

//终端属性相关消息，用于请求和回应
message TerminalProperty {

	message Sensor {
	  int32 index = 1;
	  int32 active = 2;            // 0: inactive; 1: active
	  double height = 3;           //离地高度，单位m
	  PointXYZ coordinates = 4;          //在机器人本体坐标系的位置
	}

	string host_name = 1;
	string mac_addr = 2;
	string ip_addr = 3;
	repeated Sensor sensor_list = 4; //所有传感器
	repeated string report_ip_addr = 5; //利用stream_socket类型，使用传统tcp连接汇报位置给远程主机
	repeated int32 report_tcp_port = 6; //远程主机端口
}

//基站属性相关消息，在LTManager获取基站信息，基站列表的时候使用
//或者在manager修改基站信息的时候，将发送发送到控制器
message BaseStationProperty {
	// 基站的芯片id 刚接入的基站可能是和当前基站重复的频率，
	//组地址，和hostname，所以需要芯片id来识别唯一的标识
	string chip_id = 1;
	int32 bs_id = 2;
	string host_name = 3;
	int32 ingroup_addr = 4;
	int32 frequency = 5;
	
	message Pose {
	  double pitch = 1;
	  double roll = 2;
	  double yaw = 3;
	}
	
	double start_angle = 6;
	PointXYZ coordinates = 7;
	Pose orientation = 8;
	
	int32  led_status = 9; //0: off; 1: on
	double  motor_speed = 10; //0：off; otherwise motor speed in rpm
}
	
//---------------------终端信息请求和响应--------------------------

//请求终端信息, LTManager GUI --> 终端控制器
message TerminalPropertyRequest {
	enum RequestType {
		UPDATE = 0;
		GET = 1;
	}
	RequestType type = 1;
	repeated TerminalProperty terminal_list = 2; // 控制器返回的当前的终端属性列表
}

//回复终端信息，获取的终端列表，或者update失败的列表 终端控制器 --> LTManager GUI
message TerminalPropertyResponse {
	ResponseStatus status = 1;      
	repeated TerminalProperty terminal_list = 2; //如果get 控制器返回的当前的终端属性列表，如果update返回失败的终端
}

//---------------------终端控制请求和响应-----------------------------------------

//请求终端执行某种举动（即命令）; LTManager GUI --> 终端控制器
message TerminalActionRequest {
	enum RequestType {
		RESTART_HOST = 0;
		RESTART_BIN = 1;
		UPDATE_BIN = 2;
		START_CALIBRATION = 3;
		STOP_CALIBRATION = 4;
		RUNTIME_CHECK = 5;
	}

	int32 sequence = 1;
	RequestType type = 2;
	string update_url = 3; //the binary is updated from this web address (e.g.: "username:passwd@git.lingtrack.com/xx/xx")
	repeated TerminalProperty terminal_list = 4; //指定哪一些终端执行该动作
}

//控制器表示正确收到命令（不代表终端执行完毕），终端控制器 --> LTManager GUI
message TerminalActionResponse {
	int32 sequence = 1;
	ResponseStatus status = 2;
}


//--------------------基站信息请求和响应--------------------------------------

//请求基站信息，可以获取或者更新；LTManager GUI --> 基站控制器
message BaseStationPropertyRequest {
	enum RequestType {
		UPDATE_ACTIVE = 0;  // 更新基站属性，目前不支持离线操作，所以只能修改已经连接上的基站的属性
		GET_ACTIVE = 1;  // 获取当前已经连接上的基站属性
		GET_ALL = 2;  // 获取所有的基站属性，包括曾经配置过，现在没有连接上的基站
		REMOVE = 3;  // 移除某一个属性，但是不能移除当前已经连接的基站的属性
	}
	RequestType type = 1;
	repeated BaseStationProperty base_station_list = 2;
}
	
//回复基站信息，基站控制器 --> LTManager GUI
message BaseStationPropertyResponse {
	ResponseStatus status = 1;
	repeated BaseStationProperty base_station_list = 2;
}


//--------------------基站控制请求和响应------------------------------------------

//请求基站执行某种举动（即命令）; LTManager GUI --> 基站控制器
message BaseStationActionRequest {    //基站命令，目前只考虑字符串命令行的消息格式
	int32 sequence = 1;
	string command = 2; //e.g., "set all led off"
}

//控制器表示正确收到命令,并且执行的返回结果，基站控制器 --> GUI
message BaseStationActionResponse {
	int32 sequence = 1;
	ResponseStatus status = 2;      
	string bs_info = 3; //bs_control的返回的其他信息
}

//-------------------LTManager GUI心跳请求和响应-------------------------------------------

//Gui和终端控制器的心跳消息；LTManager GUI --> 终端控制器
message GuiHeartbeatRequest {
	string mac_addr = 1;
	int32 sequence = 2;
}

//终端控制器对LTManager GUI心跳包的回复，终端控制器 --> LTManager GUI
message GuiHeartbeatResponse {
	int32 sequence = 1;
	ResponseStatus status = 2;
	UpdateEvent event = 3;     //系统状态更新
	repeated TerminalExceptStatus term_except_status = 4; // 可能同时存在多个终端的异常
	repeated BasesExceptStatus bases_except_status = 5; // 可能同时存在多个基站的异常
}

//-------------------终端心跳请求和响应-------------------------------------------

//终端和控制器的心跳消息，终端 --> 终端控制器
message TerminalHeartbeatRequest {
	int32 sequence = 1;
	bool ok_status = 2;
	TerminalExceptStatus except_status_detail = 3;
}

//控制器对终端心跳包的回复，终端控制器 --> 终端
message TerminalHeartbeatResponse {
	int32 sequence = 1;
	ResponseStatus status = 2;
	UpdateEvent event = 3;     //系统状态更新 
	TerminalActionRequest action_request_from_gui = 4;  //GUI的指令转发
}

//--------------------------------------------------------------

//监控区域的更新。将来会包括地图属性，用于定位优化，目前只考虑坐标最大最小值。GUI --> 控制器
message FieldPropertyUpdateRequest {
	
	//监控区域，比如最大最小坐标，障碍区域等等，用于异常位置排除或者地图匹配
	message FieldScope {
	  double min_x = 1;
	  double min_y = 2;
	  double min_z = 3;
	  
	  double max_x = 4;
	  double max_y = 5;
	  double max_z = 6;
	}
	
	FieldScope field_scope = 1;
}

//控制器 --> GUI
message FieldPropertyUpdateResponse { 
	ResponseStatus status = 1;
} 

//--------------------------------------------------------------

message ControllerBroadcast {
	string ip_addr = 1;		// Controller ip addr 并不可靠
	int32  terminal_port = 2;		//与终端交互信令端口
	int32  manager_port = 3;		//与manager交互信令端口
	int32  loc_xsub_port = 4;		//接收位置的xsub端口
	int32  loc_xpub_port = 5;		//发布位置的xpub端口
	enum ControllerType {
		MULTI = 0;  // 多基站基站
		SINGLE = 1;  // 单基站类型
	}
	ControllerType type = 6;
}

message BroadcastResponse {
	enum ResponseType { 	// 说明当前消息是终端响应还是管理器响应
		TERMINAL_TYPE = 0;
		MANAGER_TYPE = 1;
	}
	ResponseType type = 1;
	string ip_addr = 2;  // ip addr 并不可靠
	string mac_addr = 3;
	string host_name = 4;
}



