// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ExaminationNote.h"
#include "Components/TextRenderComponent.h"
AExaminationNote::AExaminationNote()
{
	//TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	//TextRender->SetupAttachment(GetRootComponent());

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXT_Render"));
	TextRender->SetupAttachment(GetRootComponent());
	TextRender->VerticalAlignment = EVerticalTextAligment::EVRTA_TextTop;
	TextRender->SetWorldSize(3);
	TextRender->SetRelativeRotation(FRotator(90, 90, 0));
	TextRender->SetTextRenderColor(FColor::Black);
	TextRender->SetRelativeLocation(FVector(-10, -15, 0.001));
	NoteText = FText::FromString(TEXT("현재 위치는 괴물이 출몰하여<br>위험합니다. 도주하십시오!"));
	

	BackTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Back_TEXT_Render"));
	BackTextRender->SetupAttachment(GetRootComponent());
	BackTextRender->VerticalAlignment = EVerticalTextAligment::EVRTA_TextTop;
	BackTextRender->SetWorldSize(3);
	BackTextRender->SetRelativeRotation(FRotator(-90, -90, 0));
	BackTextRender->SetTextRenderColor(FColor::Black);
	BackTextRender->SetRelativeLocation(FVector(10, -15, -0.12));
	BackNoteText = FText::FromString(TEXT("어떤 종이는 뒷부분에 <br>중요한 단서가 있습니다."));
	
}

void AExaminationNote::BeginPlay()
{
	Super::BeginPlay();
	TextRender->SetText(NoteText);
	BackTextRender->SetText(BackNoteText);
}
