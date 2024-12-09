// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CameraShake/HGWalkCameraShake.h"

UHGWalkCameraShake::UHGWalkCameraShake()
{
	bSingleInstance = true;
	OscillationDuration = 0.2; //진동지속시간
	/*위치 회전*/
	RotOscillation.Pitch.Amplitude = 0.3; //회전진동,진폭. (끄덕끄덕)
	RotOscillation.Pitch.Frequency = 12;
	RotOscillation.Yaw.Amplitude = 0.3;//회전진동,진폭. (도리도리)
	RotOscillation.Yaw.Frequency = 6;
	RotOscillation.Roll.Amplitude = 0.2;
	RotOscillation.Roll.Frequency = 3;

	/*loc Oscillation*/
	LocOscillation.Z.Amplitude = 0.6;
	LocOscillation.Z.Frequency = 12;
}
/*
		RotOscillation.Pitch.Amplitude = 5.0f; // Pitch (X 축) 방향의 흔들림 강도
		RotOscillation.Pitch.Frequency = 10.0f; // Pitch (X 축) 방향의 흔들림 주파수
		RotOscillation.Yaw.Amplitude = 0.0f; // Yaw (Y 축) 방향의 흔들림 강도
		RotOscillation.Yaw.Frequency = 0.0f; // Yaw (Y 축) 방향의 흔들림 주파수
		RotOscillation.Roll.Amplitude = 0.0f; // Roll (Z 축) 방향의 흔들림 강도
		RotOscillation.Roll.Frequency = 0.0f; // Roll (Z 축) 방향의 흔들림 주파수
*/