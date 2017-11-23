

#include "D_menu_content.h"

//angle,xsd,len,content..,
const unchar Dis_YC_Content[][7]={
	{0,3,4,YC_MIA , YC_MIB , YC_MIC},
	{0,3,3,YC_I1,YC_I2},
	{0,3,3,YC_UAB1,YC_UBC1},
	{0,3,3,YC_UAB2,YC_UBC2},
	{0,3,3,YC_UL1,YC_UL2},
};

const unchar  mainmenu[16][7] = //测量菜单[0123456789.]11I 12A 13B 14C 15U 16V 17F 18H 19Z 20P 21W 22K 23M 24Q 25V 26a 27r 28= 29o 30s 31
{
    {4, 15, 50, 31, 48}, //Iabc
    {2, 11, 12},
    {2, 11, 13},
    {2, 11, 14},
    {4, 46, 47, 31, 48}, //I1 I2
    {2, 11, 1},
    {2, 11, 2},
    {2, 31, 52}, //UAB1 UBC1
    {4, 15, 12,13,1},
    {4, 15, 13,14,1},
    {2, 31, 52}, //UAB2 UBC2
    {4, 15, 12,13,2},
    {4, 15, 13,14,2},
    {2, 31, 52}, //UL1 UL2
    {2, 15, 1},
    {2, 15, 2},
};

const unchar  menuID1111[DZnum][7] = //定值设置
{
    {4, 62, 52, 111, 112}, //有压定值

    {4, 64, 52, 111, 112}, //无压定值
	
    {5, 87, 156, 157, 83, 10}, //方式1t

    {5, 87, 156, 158, 83, 10}, //方式2t
		
    {5, 87, 156, 159, 83, 10}, //方式3t
		
    {5, 87, 156, 160, 83, 10}, //方式4t
};


const unchar DZunit[DZnum] = //定值单位   12A 30S 16V  1819HZ
{
 UNIT_U,UNIT_U,UNIT_T,UNIT_T,UNIT_T,UNIT_T,
};

const unchar Prot_sm[12] = {0, 1, 2, 3} ; //SOE显示保护名字,
const unchar con_sm[CONnum+1] = {0, 1, 1, 1, 1, 1,1}; //控制字类型 1投退 0出口 con_sm[0]无效
const unchar  menuID1112[CONnum][9] = //控制字
{
    {5, 155, 69, 87, 156,157}, //自投方式1
    {5, 155, 69, 87, 156,158}, //自投方式2
    {5, 155, 69, 87, 156,159}, //自投方式3
    {5, 155, 69, 87, 156,160}, //自投方式4
    {7, 157,57, 99, 31, 52, 14,8}, //1线路电压检查
    {7, 158,57, 99, 31, 52, 14,8}, //2线路电压检查
};

const unchar menuID1114[17][5] =
{
    {4, 41, 42, 17, 74}, //通讯地址
    {3, 43, 44, 45,}, //波特率
    {4, 31, 52, 75, 80}, //电压变比
    {4, 31, 48, 75, 80}, //电流变比
    {3, 150, 49, 1}, //PIA系数
    {3, 151, 49, 1}, //PIB系数
    {3, 152, 49, 1}, //PIC系数
    {3, 115, 49, 1}, //MIA系数
    {3, 116, 49, 1}, //MIB系数
    {3, 117, 49, 1}, //MIC系数
    {3, 118, 49, 1}, //UA系数
    {3, 119, 49, 1}, //UB系数
    {3, 120, 49, 1}, //UC系数
    {3, 122, 49, 1}, //I0系数
    {3, 121, 49, 1}, //U0系数
	{2, 153,86}, //网口地址
	{2,154,25},//子网掩码
};

structmenu  MENU[16] = //菜单ID,显示起始位置,父菜单(0表示无),子菜单个数(0表示无),子菜单起始(0表示无),光标位置,菜单行数,菜单每行字数,菜单内容
{
    {0, 0, 0, 1, 1, 1, 5, 7, &mainmenu[0][0]}, //主界面
    {1, 0, 0, 6, 2, 1, 6, 5, &menuID1[0][0]}, //主菜单
    {2, 0, 1, 1, 8, 0, 1, 5, &menuID11[0][0]}, //输入密码
    {3, 0, 1, 1, 9, 1, 2, 5, &menuID12[0][0]}, //查看删除报告
    {4, 0, 1, 2, 10, 1, 2, 5, &menuID13[0][0]}, //查看时钟
    {5, 0, 1, 0, 0, 0, 2, 17, &menuID14[0][0]}, //输入检测
    {6, 0, 1, 0, 0, 0, 1, 5, &menuID15[0][0]}, //遥控测试
    {7, 0, 1, 0, 0, 0, 3, 6, &menuID16[0][0]}, //版本号
    {8, 0, 2, 4, 12, 1, 4, 6, &menuID111[0][0]}, //定值设置,控制字设置,修改密码
    {9, 0, 3, 0, 0, 1, 0, 0, &menuID121[0]}, //查看报告内容
    {10, 0, 4, 0, 0, 0, 3, 5, &menuID131[0][0]}, //时钟
    {11, 0, 4, 0, 0, 0, 3, 5, &menuID132[0][0]}, //修改时钟
    {12, 0, 8, 0, 0, 1, DZnum, 7, &menuID1111[0][0]}, //定值设置
    {13, 0, 8, 0, 0, 1, CONnum, 9, &menuID1112[0][0]}, //控制字设置
    {14, 0, 8, 0, 0, 0, 1, 6, &menuID1113[0][0]}, //修改密码
    {15, 0, 8, 0, 0, 1, 17, 5, &menuID1114[0][0]}, //系统参数
};

const unchar  menuID16[4][8] = //版本号  美能
{
    {6, 146, 147, 148, 149, 144, 145},
    {7, 12, 36, 32, 6, 0, 1, 5},
    {5, 16, 1, 10, 0, 0},
    {4, 68,109, 155, 69},


//{8,134,68,135,146, 147, 148, 149, 144, 145}, //名称
//{3,20,21,22},{5,16,1,10,0,0},   //版本号
//{3,136,137,25},{4,3,13,8,16},   //检验码
};
