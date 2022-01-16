#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<windows.h>
#define WAITING_FOR_SORTING 4
#define SEARCHING 5
#define BREAK 6
#define EXCHANGING 7
#define DONE 1
#define EXIT 0
#define NEXT 2
#define TMPSAVING 3
#define DRAWING 8
#define DONESW 9
using namespace std;
class CMESSAGES;
class CPARAM_SORT;
class CPARAM_DRAWTEXT;
typedef CPARAM_SORT* PCPARAM_SORT;
typedef CPARAM_DRAWTEXT* PCPARAM_DWTXT;
typedef CMESSAGES* PCMSG0;


class CMESSAGES
{
public:
	int event;
	int pos_i, pos_j,pos_k;
}messages;


class CPARAM_SORT
{
public:
	float* a;
	int l;
	PCMSG0 PCMsg;
}param_sort;
PCPARAM_SORT pparam_sort = &param_sort;


class CPARAM_DRAWTEXT
{
public:
	float* arr;
	PCMSG0 PCMsg;
}param_dwtxt;
PCPARAM_DWTXT pparam_dwtxt = &param_dwtxt;



void MsgUpdater(PCMSG0 PCMsg, int event, int pos_i, int pos_j,int pos_k)
{
	PCMsg->event = event;
	PCMsg->pos_i = pos_i;
	PCMsg->pos_j = pos_j;
	PCMsg->pos_k = pos_k;
}

void getboolstr(PTCHAR boolstr,bool n)
{
	if (n) _stprintf_s(boolstr,6,_T("true"));
	else _stprintf_s(boolstr,6, _T("false"));
}


void DrawTxt(PCPARAM_DWTXT param)
{
	TCHAR s_i[10];
	TCHAR s_j[10];
	TCHAR s_chk1[30];
	TCHAR s_chk2[30];
	TCHAR s_break[10];
	TCHAR s_t_asgn[20];
	TCHAR s_t_asgn2[20];
	TCHAR s_t_asgn3[20];
	TCHAR boolstr[10];
	settextstyle(25, 0, _T("Consolas"));
	while (true)
	{
		settextcolor(RGB(255, 255, 255));
		_stprintf_s(s_i, 10, _T("i == %d"), param->PCMsg->pos_i);
		_stprintf_s(s_j, 10, _T("j == %d"), param->PCMsg->pos_j);
		getboolstr(boolstr, param->arr[param->PCMsg->pos_j] < param->arr[param->PCMsg->pos_i]);
		_stprintf_s(s_chk1, 30, _T("arr[%d] < arr[%d] == %s"), param->PCMsg->pos_j, param->PCMsg->pos_i, boolstr);
		getboolstr(boolstr, param->PCMsg->pos_j != param->PCMsg->pos_i - 1);
		_stprintf_s(s_chk2, 30, _T("j != i - 1 == %s"), boolstr);
		_stprintf_s(s_break, 10, _T(";break;"));
		_stprintf_s(s_t_asgn, 20, _T("tmp = arr[%d]"), param->PCMsg->pos_i);
		_stprintf_s(s_t_asgn2, 20, _T("arr[%d] = tmp"), param->PCMsg->pos_k + 1);
		_stprintf_s(s_t_asgn3, 20, _T("arr[%d] = arr[%d]"), param->PCMsg->pos_k + 1, param->PCMsg->pos_k);
		outtextxy(10, 10, s_i);
		outtextxy(10, 40, s_j);
		
		if (param->PCMsg->event == SEARCHING)
		{
			
			if (param->arr[param->PCMsg->pos_j] < param->arr[param->PCMsg->pos_i])
			{
				settextcolor(RGB(244, 121, 131));
				outtextxy(10, 70, s_chk1);
				outtextxy(300, 70, s_break);
				settextcolor(RGB(230, 230, 250));
				outtextxy(10, 130, s_chk2);
				settextcolor(RGB(255, 255, 255));
			}
			else outtextxy(10, 70, s_chk1);
		}
		if (param->PCMsg->event == TMPSAVING)
		{
			outtextxy(10, 70, s_t_asgn);
		}
		if (param->PCMsg->event == EXCHANGING)
		{
			outtextxy(10, 70, s_t_asgn3);
		}
		if (param->PCMsg->event == DONESW)
		{
			outtextxy(10, 70, s_t_asgn2);
		}

	}
}



void sort_anim(PCPARAM_SORT param)
{
	int i, j, k, t;
	
	for (i = 1; i < param->l; i++)
	{
		
		MsgUpdater(param->PCMsg, WAITING_FOR_SORTING, i, -1,-1); //更新事件:a[i]待排
		Sleep(1000);
		for (j = i - 1; j >= 0; j--)
		{
			MsgUpdater(param->PCMsg, SEARCHING, i, j,-1); //更新事件:a[i]待排,搜索到a[j] 
			Sleep(1000);
			if (param->a[j] < param->a[i]) 
			{
				MsgUpdater(param->PCMsg, BREAK, i, j,-1);
				//Sleep(1000);
				break;
			}
		}

		if (j != i - 1)
		{
			t = param->a[i];
			MsgUpdater(param->PCMsg, TMPSAVING, i, j, -1);
			while (param->PCMsg->event != DONE)
			{
				Sleep(10);
			}
			for (k = i - 1; k > j; k--)
			{
				MsgUpdater(param->PCMsg, EXCHANGING, i, j, k);	
				while (param->PCMsg->event != DONE)
				{
					Sleep(10);
				}
				param->a[k + 1] = param->a[k];
				Sleep(1000); 
			}
			MsgUpdater(param->PCMsg, DONESW, i, j, k+1);

			while (param->PCMsg->event != DONE)
			{
				Sleep(10);
			}
			param->PCMsg->event = -1;
			param->a[k + 1] = t;
		}

	}
	param->PCMsg->event = EXIT;
}






void RectRedraw(float arr[], int num,float rect_width,float left_x,float bottom_y,float interval, PCMSG0 PMsg)
{
	BeginBatchDraw();
	int i;
	setfillcolor(RGB(247,238,214)); //柱子填充:米黄色
	float rect_height;
	static int t;
	static float  k_1_rect_height;
	static bool flag_moving = false;
	static bool flag_tmptransporting = false;
	static double dx = 0;
	static int cnt = 0;
	static float t_left_x=0, t_rect_height, t_bottom_y;
	const float leftx = left_x;
	TCHAR ArrIndex[10];
	TCHAR tmp[5];
	_stprintf_s(tmp, 5, _T("tmp"));
	for (i = 0; i < num; i++)
	{
		_stprintf_s(ArrIndex,10, _T("[%d]"), i);
		rect_height = 15 * arr[i];  //计算当前柱子对应高度
		switch (PMsg->event)  //事件处理
		{
		case WAITING_FOR_SORTING: //待排元素柱子变红
		{
			if (i == PMsg->pos_i) //若a[i]为待排元素
			{
				setfillcolor(RGB(244, 121, 131));	 //淡红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else
			{
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
			}
			break;
		}
		case SEARCHING:
		{
			if (i == PMsg->pos_i) //若a[i]为待排元素
			{
				setfillcolor(RGB(244, 121, 131));	 //淡红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else if (i == PMsg->pos_j) //若搜索到a[pos_j]
			{
				setfillcolor(RGB(255, 165, 0));	 //橙色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else
			{
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
			}
			break;



		}
		case BREAK:
		{
			if (i == PMsg->pos_i) //若a[i]为待排元素
			{
				setfillcolor(RGB(244, 121, 131));	 //淡红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else if (i == PMsg->pos_j) //若搜索到a[pos_j]处发生break
			{
				setfillcolor(RGB(201, 55, 86));	 //深红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else
			{
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
			}
			break;

		}
		case TMPSAVING:
		{
			if (i == PMsg->pos_i) //若a[i]为待排元素
			{
				setfillcolor(RGB(169, 169, 169));	 //灰色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				t_rect_height = rect_height;
				t = i;
				t_left_x = left_x + interval * (num - t + 1);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
				PMsg->event = DONE;
			}
			else if (i == PMsg->pos_j) //若搜索到a[pos_j]处发生break
			{
				setfillcolor(RGB(244, 121, 131));  //淡红色
				solidrectangle(t_left_x, bottom_y - t_rect_height, t_left_x + rect_width, bottom_y);
				setfillcolor(RGB(201, 55, 86));	 //深红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else 
			{
				setfillcolor(RGB(230, 230, 250));  //淡紫色
				solidrectangle(t_left_x, bottom_y - t_rect_height, t_left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
			}
			
			break;
		}
		case EXCHANGING:
		{
			setfillcolor(RGB(230, 230, 250));  //淡紫色
			solidrectangle(t_left_x, bottom_y - t_rect_height, t_left_x + rect_width, bottom_y);
			if (i == PMsg->pos_j) //若搜索到a[pos_j]处发生break
			{

				setfillcolor(RGB(201, 55, 86));	 //深红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else if (i == PMsg->pos_k) //若交换到a[pos_k]处
			{

				k_1_rect_height = 15 * arr[i + 1];
				setfillcolor(RGB(169, 169, 169)); //灰色
				solidrectangle(left_x + interval, bottom_y - k_1_rect_height, left_x + interval + rect_width, bottom_y);
				setfillcolor(RGB(135, 206, 235));//天蓝色	//interval==40
				if (flag_moving)
				{
					dx += 0.032;
					cnt++;
				}
				else flag_moving = true;

				if (flag_moving && cnt >= 1250)
				{
					flag_moving = false;
					cnt = 0;
					dx = 0;
					PMsg->event = DONE;
				}
				rect_height = 15 * arr[i];
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				solidrectangle(left_x + dx, bottom_y - rect_height, left_x + dx + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色

			}
			else if (i == PMsg->pos_k + 1);
			else if (i == PMsg->pos_i) //若a[i]为待排元素
			{
				setfillcolor(RGB(244, 121, 131));  //淡红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else
			{
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
			}	
			break;
		}

		case DONE:
		{
			setfillcolor(RGB(230, 230, 250));  //淡紫色
			solidrectangle(t_left_x, bottom_y - t_rect_height, t_left_x + rect_width, bottom_y);
			if (i == PMsg->pos_i) //若a[i]为待排元素
			{
				setfillcolor(RGB(244, 121, 131));  //淡红色 //恢复米黄色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else if (i == PMsg->pos_j) //若搜索到a[pos_j]处发生break
			{

				setfillcolor(RGB(201, 55, 86));	 //深红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else
			{
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
			}
			break;
		}
		case DONESW:
		{
			if (flag_moving)
			{
				cnt++;
				t_left_x -= dx;
			}
			else
			{
				dx = ((11 - PMsg->pos_k) * interval )/ 27000.0;
				flag_moving = true;
			}

			if (cnt >= 27000)
			{
				flag_moving = false;
				cnt = 0;
				dx = 0;
				PMsg->event=DONE;
			}

			if (i == PMsg->pos_i) //若a[i]为待排元素
			{
				setfillcolor(RGB(244, 121, 131));  //淡红色 //恢复米黄色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else if (i == PMsg->pos_j) //若搜索到a[pos_j]处发生break
			{

				setfillcolor(RGB(201, 55, 86));	 //深红色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			}
			else
			{
				setfillcolor(RGB(247, 238, 214)); //恢复米黄色
				solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
			}

			setfillcolor(RGB(230, 230, 250));  //淡紫色
			solidrectangle(t_left_x, bottom_y - t_rect_height, t_left_x + rect_width, bottom_y);
			break;


		}


		default:
		{
			setfillcolor(RGB(230, 230, 250));  //淡紫色
			solidrectangle(t_left_x, bottom_y - t_rect_height, t_left_x + rect_width, bottom_y);
			setfillcolor(RGB(247, 238, 214)); //恢复米黄色
			solidrectangle(left_x, bottom_y - rect_height, left_x + rect_width, bottom_y);
		}

		}

		settextcolor(RGB(255, 255, 255));
		outtextxy(left_x, bottom_y + 10, ArrIndex);
		outtextxy(leftx+interval*11, bottom_y + 10, tmp);
		left_x += interval; //更新柱子左x坐标
	}

	FlushBatchDraw();
	

	
}

int main()
{
	
	float arr[10] = { 15,21,23,12,22,14,10,11,5,7 };
	float width, height;
	width = 800;
	height = 600;
	int l = sizeof(arr) / sizeof(float);
	initgraph(width, height, 0);
	setcolor(RGB(247, 238, 214));
	messages={ -1,-1,-1 };
	PCMSG0 PMsg = &messages;
	MsgUpdater(PMsg, -1, -1, -1, -1);
	param_sort = { arr,l,PMsg };
	param_dwtxt = { arr,PMsg };
	HANDLE Thread_sort=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)sort_anim,pparam_sort,0,NULL);
	HANDLE Thread_dwtxt = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DrawTxt, pparam_dwtxt, 0, NULL);
	while (1)
	{
		cleardevice();
		RectRedraw(arr, l, 20, 40, 560, 40, PMsg);
		if (PMsg->event==EXIT) break;
	};
	
	closegraph();
	return 0;

    
}

