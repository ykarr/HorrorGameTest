// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CameraShake/HGRunCameraShake.h"

UHGRunCameraShake::UHGRunCameraShake()
{
		bSingleInstance = true;
		OscillationDuration = 0.2; //�������ӽð�
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