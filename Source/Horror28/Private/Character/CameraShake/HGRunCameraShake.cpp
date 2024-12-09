// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CameraShake/HGRunCameraShake.h"

UHGRunCameraShake::UHGRunCameraShake()
{
		bSingleInstance = true;
		OscillationDuration = 0.2; //진동지속시간
		/*위치 회전*/
		RotOscillation.Pitch.Amplitude = 0.5; //회전진동,진폭. (끄덕끄덕)
		RotOscillation.Pitch.Frequency = 17;
		RotOscillation.Yaw.Amplitude = 0.5;//회전진동,진폭. (도리도리)
		RotOscillation.Yaw.Frequency = 8;
		RotOscillation.Roll.Amplitude = 0.25;
		RotOscillation.Roll.Frequency = 5;

		/*loc Oscillation*/
		LocOscillation.Z.Amplitude = 2;
		LocOscillation.Z.Frequency = 17;
}