#include<iostream>
#include <Kinect.h>
#pragma comment ( lib, "kinect20.lib" ) 
using namespace std;

int main()
{
	printf("Hello, Wellcome to kinect world!\n");
	IKinectSensor* bb; //申请一个Sensor指针
	HRESULT hr = GetDefaultKinectSensor(&bb); // 获取一个默认的Sensor
	if ( FAILED(hr) )
	{
		printf("No Kinect connect to your pc!\n");
		goto endstop;
	}
	BOOLEAN bIsOpen = 0;
	bb->get_IsOpen(&bIsOpen); // 查看下是否已经打开
	printf("bIsOpen： %d\n", bIsOpen);

	if ( !bIsOpen ) // 没打开，则尝试打开
	{
		hr = bb->Open();
		if ( FAILED(hr) )
		{
			printf("Kinect Open Failed!\n");
			goto endstop;
		}
		printf("Kinect opened! But it need sometime to work!\n");
		// 这里一定要多等会，否则下面的判断都是错误的
		printf("Wait For 3000 ms...\n");
		Sleep(3000);
	}
	bIsOpen = 0;
	bb->get_IsOpen(&bIsOpen); // 是否已经打开
	printf("bIsOpen： %d\n", bIsOpen);
	BOOLEAN bAvaliable = 0;
	bb->get_IsAvailable(&bAvaliable); // 是否可用
	printf("bAvaliable： %d\n", bAvaliable);

	DWORD dwCapability = 0; 
	bb->get_KinectCapabilities(&dwCapability); // 获取容量
	printf("dwCapability： %d\n", dwCapability);
	WCHAR bbuid[256] = { 0 };
	bb->get_UniqueKinectId(256, bbuid); // 获取唯一ID
	printf("UID: %s\n",bbuid);

	// 音频数据获取
	// 获取身体数据
	IBodyFrameSource* bodys = nullptr;
	bb->get_BodyFrameSource(&bodys); // Body 数据源
	INT32 nBodyNum = 0;
	bodys->get_BodyCount(&nBodyNum); // 获取body 个数，没用，一直是6
	printf("Body Num: %d\n", nBodyNum);
	IBodyFrameReader* bodyr = nullptr;
	bodys->OpenReader(&bodyr); // 准备读取body数据

	while (true)
	{
		IBodyFrame* bodyf = nullptr;
		bodyr->AcquireLatestFrame(&bodyf); // 获取最近的一帧数据
		if ( !bodyf )
		{
			Sleep(100);
			printf(".");
			continue;
		}
		IBody* ppBodies[BODY_COUNT] = { 0 };
		bodyf->GetAndRefreshBodyData(BODY_COUNT, ppBodies); // 更新所有人身体数据
		for (int i = 0; i < BODY_COUNT; ++i)
		{
			IBody* pBody = ppBodies[i]; // 轮询每个人的信息
			if (pBody)
			{
				BOOLEAN bTracked = false;
				hr = pBody->get_IsTracked(&bTracked); // 检测是否被跟踪，即是否有这个人
				if (bTracked)
				{
					Joint joints[JointType_Count];
					HandState leftHandState = HandState_Unknown;
					HandState rightHandState = HandState_Unknown;
					pBody->get_HandLeftState(&leftHandState); // 获取左手的状态
					pBody->get_HandRightState(&rightHandState); // 获取右手的状态

					hr = pBody->GetJoints(_countof(joints), joints); // 获取身体的骨骼信息，25点
					printf("Person %d : Joints[0].Z  %.2f\n", i, joints[0].Position.X); //简单的输出他的信息

				}
			}
		}
		for (int i = 0; i < BODY_COUNT; ++i)
		{
			ppBodies[i]->Release();
		}
		bodyf->Release();
	}


endclose:
	bb->Close();
endstop:
	system("pause");
	return 0;
}
