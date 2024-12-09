// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HGHeadCameraShake.h"

void UHGHeadCameraShake::HorrorWalkCamera()
{
	/*��ġ ȸ��*/
	RotOscillation.Pitch.Amplitude = 0.3; //ȸ������,����. (��������)
	RotOscillation.Pitch.Frequency = 12;
	RotOscillation.Yaw.Amplitude = 0.3;//ȸ������,����. (��������)
	RotOscillation.Yaw.Frequency = 6;
	RotOscillation.Roll.Amplitude = 0.2;
	RotOscillation.Roll.Frequency = 3;

	/*loc Oscillation*/
	LocOscillation.Z.Amplitude = 0.6;
	LocOscillation.Z.Frequency = 12;
}

void UHGHeadCameraShake::HorrorRunCamera()
{
	//GEngine->AddOnScreenDebugMessage(3, 1, FColor::Red, FString("RUN"), true);
	/*��ġ ȸ��*/
	RotOscillation.Pitch.Amplitude = 0.5; //ȸ������,����. (��������)
	RotOscillation.Pitch.Frequency = 17;
	RotOscillation.Yaw.Amplitude = 0.5;//ȸ������,����. (��������)
	RotOscillation.Yaw.Frequency = 8;
	RotOscillation.Roll.Amplitude = 0.25;
	RotOscillation.Roll.Frequency = 5;

	/*loc Oscillation*/
	LocOscillation.Z.Amplitude = 2;
	LocOscillation.Z.Frequency = 17;
}

UHGHeadCameraShake::UHGHeadCameraShake()
{
	bSingleInstance = true;
	/*��ġ ȸ��*/
	OscillationDuration = 0.2; //�������ӽð�
	RotOscillation.Pitch.Amplitude = 0.3; //ȸ������,����. (��������)
	RotOscillation.Pitch.Frequency = 12;
	RotOscillation.Yaw.Amplitude = 0.3;//ȸ������,����. (��������)
	RotOscillation.Yaw.Frequency = 6;
	RotOscillation.Roll.Amplitude = 0.2;
	RotOscillation.Roll.Frequency = 3;

	/*loc Oscillation*/
	LocOscillation.Z.Amplitude = 0.6;
	LocOscillation.Z.Frequency = 12;
}
/* 
		RotOscillation.Pitch.Amplitude = 5.0f; // Pitch (X ��) ������ ��鸲 ����
        RotOscillation.Pitch.Frequency = 10.0f; // Pitch (X ��) ������ ��鸲 ���ļ�
        RotOscillation.Yaw.Amplitude = 0.0f; // Yaw (Y ��) ������ ��鸲 ����
        RotOscillation.Yaw.Frequency = 0.0f; // Yaw (Y ��) ������ ��鸲 ���ļ�
        RotOscillation.Roll.Amplitude = 0.0f; // Roll (Z ��) ������ ��鸲 ����
        RotOscillation.Roll.Frequency = 0.0f; // Roll (Z ��) ������ ��鸲 ���ļ�
*/