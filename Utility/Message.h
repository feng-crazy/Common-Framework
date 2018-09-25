/******************************************************************************
版权所有：	深圳市灵踪科技有限公司
项目名称：	LTManager
版本号：	1.0
文件名：	Message.h
生成日期：	2018.5.22
作者：		何登锋
功能说明：
    消息与其载体结构的定义。

=============
   注意事项
=============
1	编辑该文件前请仔细阅读 Frame/MsgDefine.h 文件中的“注意事项”
2	消息的订阅者受CPU等因素的影响，在200MHz的CPU情况下，订阅者受到限制，一般情况
    下，订阅在超过5个，系统就会出现问题，CPU的占用率也会上升。
******************************************************************************/
#ifndef _PUBLIC_MESSAGE_H
#define _PUBLIC_MESSAGE_H





// MESSAGE ID  <<====>>  MESSAGE ENTITY FORMAT
// ID RANGE: 0 - 99
enum PublishMessage
{
    MSG_NONE,
    MSG_SYSTEM_TIME0, // 0.02秒发送一次，
    MSG_SYSTEM_TIME1, // 0.1秒一次
    MSG_SYSTEM_TIME2, // 0.5秒一次，
    MSG_SYSTEM_TIME3, // 每秒一次，
    MSG_WAVE_UPDATE_TIME, // 0.25秒一次，
    MSG_LOC_UPDATE_TIME, // 0.1秒一次，
    MSG_DATA_STATUS_UPDATE_TIME, // 0.25秒一次，

    MSG_SYSTEM_RESET,//系统重置消息,一些清空功能.

};



#endif

