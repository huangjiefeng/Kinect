#include<iostream>
#include <Kinect.h>
#pragma comment ( lib, "kinect20.lib" ) 
using namespace std;

int main()
{
	printf("Hello, Wellcome to kinect world!\n");
	IKinectSensor* bb; //����һ��Sensorָ��
	HRESULT hr = GetDefaultKinectSensor(&bb); // ��ȡһ��Ĭ�ϵ�Sensor
	if ( FAILED(hr) )
	{
		printf("No Kinect connect to your pc!\n");
		goto endstop;
	}
	BOOLEAN bIsOpen = 0;
	bb->get_IsOpen(&bIsOpen); // �鿴���Ƿ��Ѿ���
	printf("bIsOpen�� %d\n", bIsOpen);

	if ( !bIsOpen ) // û�򿪣����Դ�
	{
		hr = bb->Open();
		if ( FAILED(hr) )
		{
			printf("Kinect Open Failed!\n");
			goto endstop;
		}
		printf("Kinect opened! But it need sometime to work!\n");
		// ����һ��Ҫ��Ȼᣬ����������ж϶��Ǵ����
		printf("Wait For 3000 ms...\n");
		Sleep(3000);
	}
	bIsOpen = 0;
	bb->get_IsOpen(&bIsOpen); // �Ƿ��Ѿ���
	printf("bIsOpen�� %d\n", bIsOpen);
	BOOLEAN bAvaliable = 0;
	bb->get_IsAvailable(&bAvaliable); // �Ƿ����
	printf("bAvaliable�� %d\n", bAvaliable);

	DWORD dwCapability = 0; 
	bb->get_KinectCapabilities(&dwCapability); // ��ȡ����
	printf("dwCapability�� %d\n", dwCapability);
	WCHAR bbuid[256] = { 0 };
	bb->get_UniqueKinectId(256, bbuid); // ��ȡΨһID
	printf("UID: %s\n",bbuid);

	// ��Ƶ���ݻ�ȡ
	// ��ȡ��������
	IBodyFrameSource* bodys = nullptr;
	bb->get_BodyFrameSource(&bodys); // Body ����Դ
	INT32 nBodyNum = 0;
	bodys->get_BodyCount(&nBodyNum); // ��ȡbody ������û�ã�һֱ��6
	printf("Body Num: %d\n", nBodyNum);
	IBodyFrameReader* bodyr = nullptr;
	bodys->OpenReader(&bodyr); // ׼����ȡbody����

	while (true)
	{
		IBodyFrame* bodyf = nullptr;
		bodyr->AcquireLatestFrame(&bodyf); // ��ȡ�����һ֡����
		if ( !bodyf )
		{
			Sleep(100);
			printf(".");
			continue;
		}
		IBody* ppBodies[BODY_COUNT] = { 0 };
		bodyf->GetAndRefreshBodyData(BODY_COUNT, ppBodies); // ������������������
		for (int i = 0; i < BODY_COUNT; ++i)
		{
			IBody* pBody = ppBodies[i]; // ��ѯÿ���˵���Ϣ
			if (pBody)
			{
				BOOLEAN bTracked = false;
				hr = pBody->get_IsTracked(&bTracked); // ����Ƿ񱻸��٣����Ƿ��������
				if (bTracked)
				{
					Joint joints[JointType_Count];
					HandState leftHandState = HandState_Unknown;
					HandState rightHandState = HandState_Unknown;
					pBody->get_HandLeftState(&leftHandState); // ��ȡ���ֵ�״̬
					pBody->get_HandRightState(&rightHandState); // ��ȡ���ֵ�״̬

					hr = pBody->GetJoints(_countof(joints), joints); // ��ȡ����Ĺ�����Ϣ��25��
					printf("Person %d : Joints[0].Z  %.2f\n", i, joints[0].Position.X); //�򵥵����������Ϣ

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
