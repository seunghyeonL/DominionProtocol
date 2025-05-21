// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/BossRoomDoor.h"

ABossRoomDoor::ABossRoomDoor()
{
    LeftDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("LeftDoorPivot"));
    LeftDoorPivot->SetupAttachment(SceneComp);
    LeftDoorPivot->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));

    DoorMesh->SetupAttachment(LeftDoorPivot);

    RightDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("RightDoorPivot"));
    RightDoorPivot->SetupAttachment(SceneComp);
    RightDoorPivot->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

    SecondDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondDoorMesh"));
    SecondDoorMesh->SetupAttachment(RightDoorPivot);
    SecondDoorMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void ABossRoomDoor::OpenDoor(float Value)
{
	DoorMesh->SetRelativeRotation(FRotator(0.f, DoorRotateAngle * Value, 0.f));
	SecondDoorMesh->SetRelativeRotation(FRotator(0.f, 180-DoorRotateAngle * Value, 0.f));
}

void ABossRoomDoor::OnStoryProgressUpdated_Implementation(EGameStoryState NewState)
{
}
