#ifndef _COMMON_H_
#define _COMMON_H_

#define SQUARE(x) ((x) * (x))

//引入所需使用的標頭檔
#include <QPoint>
#include <QString>
#include <QDebug>
#include <QPointer>

#include "Shapes/Shapes.h"

#define PI 3.14159265359

/*程式中繪圖相關的參數設定*/
//調整用原點之半徑
#define MODIFY_POINT_RADIUS (5)
//平常狀態繪出區域範圍使用的顏色
#define NOMAL_REGION_COLOR (Qt::green)
//調整狀態繪出區域範圍使用的顏色
#define MODIFYING_REGION_COLOR (Qt::red)
//提示使用者目前所用的區域形狀之顏色
#define USING_SHAPE_COLOR (Qt::red)
//繪出區域範圍的框線粗度
#define REGION_LINE_THICKNESS (1)
#define NORMAL_MODIFY_POINT_COLOR (QBrush(Qt::green))
#define MODIFYING_MODIFY_POINT_COLOR (QBrush(Qt::red))

/*程式中所使用到的按鍵定義*/
//按鍵_上
#define KEY_UP 'w'
//按鍵_下
#define KEY_DOWN 's'
//按鍵_左
#define KEY_LEFT 'a'
//按鍵_右
#define KEY_RIGHT 'd'
//按鍵_前一畫面
#define KEY_PREVIOUS_FRAME 'q'
//按鍵_下一畫面
#define KEY_NEXT_FRAME 'e'
//按鍵_播放_暫停
#define KEY_PLAY_PAUSE ' '
//按鍵_離開程式
#define KEY_ESC 27
//按鍵_取消選取
#define KEY_CANCLE 'c'
//按鍵_重新播放
#define KEY_REPLAY 'r'
//按鍵_沿用畫面上的框選區域
#define KEY_FORCE_USE 'f'
//按鍵_框選形狀1
#define KEY_SHAPE_1 '1'
//按鍵_框選形狀2
#define KEY_SHAPE_2 '2'
//按鍵_框選形狀3
#define KEY_SHAPE_3 '3'

//設定FPS
#define FRAME_RATE (30)

//顯示圖片的預設等待時間預設
const int defaultWaitingTime = (1000 / FRAME_RATE);

#define ROOT_TAG                "Root"
#define FILE_DESCRIPTION_TAG    "FileDescription"
#define VIDEO_FILE_NAME_TAG     "VideoFileName"
#define CREATE_DATE_TAG         "CreateDate"
#define LATEST_MODIFY_DATE_TAG  "LatestModifyDate"
#define DATA_SET_TAG            "DataSet"
#define HEADER_TAG              "Header"
#define RECORD_TAG              "Record"
#define SHAPE_TAG               "Shape"
#define START_POINT_TAG         "StartPoint"
#define HEIGHT_TAG              "Height"
#define WIDTH_TAG               "Width"
#define FRAME_NUMBER_ATTRIBUTE  "frameNumber"
#define TOTAL_RECORD_ATTRIBUTE  "totalRecord"
#define SERIAL_NUMBER_ATTRIBUTE "serialNumber"

#define DATE_FORMAT "yyyy/MM/dd"

const int VIDEO_VIEEWER_WIDTH = 640;
const int VIDEO_VIEEWER_HEIGHT = 480;

#endif
