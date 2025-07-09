// Copyright 2024 Eren Balatkan. All Rights Reserved.


#include "MnhTracerComponent.h"

#include "GameplayTagContainer.h"
#include "MnhSubsystem.h"
#include "MnhTracer.h"

DEFINE_LOG_CATEGORY(LogMnh)

class FMissNoHitModule;
// Sets default values for this component's properties
UMnhTracerComponent::UMnhTracerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMnhTracerComponent::BeginDestroy()
{
	Super::BeginDestroy();

	if (const auto World = GetWorld())
	{
		OnDestroyed.Broadcast();
		for (const auto& TracerConfig : TracerConfigs)
		{
			TracerConfig.MarkTracerDataForRemoval(World);
		}
	}
}

void UMnhTracerComponent::InitializeTracers(const FGameplayTagContainer TracerTags, UPrimitiveComponent* TracerSource)
{
	if (TracerTags.IsEmpty())
	{
		const FString Message = FString::Printf(TEXT("MissNoHit Warning: Tracer Tags are empty on Initialize Tracers function"));
		FMnhHelpers::Mnh_Log(Message);
		return;
	}
	
	if (!bIsInitialized)
	{
		EarlyTracerInitializations.Add(FTracerInitializationData{TracerTags, TracerSource});
		return;
	}
	
	if (!IsValid(TracerSource))
	{
		const auto Owner = GetOwner();
		FString Message;
		if (!Owner)
		{
			Message = FString::Printf(TEXT("MissNoHit Warning: Tracer Source is not valid"));
		}
		else
		{
			Message = FString::Printf(TEXT("MissNoHit Warning: Tracer Source is not valid on object: %s"), *Owner->GetName());
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
		return;
	}

	int InitializedTracerCount = 0;
	for (auto& TracerConfig : TracerConfigs)
	{
		if(TracerTags.HasTagExact(TracerConfig.TracerTag))
		{
			TracerConfig.InitializeParameters(TracerSource);
			InitializedTracerCount++;
		}
	}
	if (InitializedTracerCount == 0)
	{
		const FString Message = FString::Printf(TEXT("MissNoHit Warning: No Tracer with Tag [%s] found during initialization"), *TracerTags.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	}
}


void UMnhTracerComponent::StopAllTracers()
{
	for (auto& TracerConfig : TracerConfigs)
	{
		if (TracerConfig.TracerDataIdx == -1)
		{
			continue;
		}
		TracerConfig.ChangeTracerState(GetWorld(), false);
		OnTracerStopped.Broadcast(TracerConfig.TracerTag);
	}
}

void UMnhTracerComponent::ResetHitCache()
{
	HitCache.Reset();
}

void UMnhTracerComponent::AddToIgnoredActors(const FGameplayTagContainer TracerTags, AActor* Actor)
{
	if (Actor == nullptr)
	{
		FMnhHelpers::Mnh_Log("MissNoHit Warning: Actor is null, cannot add to ignored actors");
		return;
	}
	for (auto& TracerConfig: TracerConfigs)
	{
		if (!TracerTags.HasTag(TracerConfig.TracerTag))
		{
			continue;
		}
		
		TracerConfig.CollisionParams.AddIgnoredActor(Actor);
		if (TracerConfig.TracerDataIdx != -1)
		{
			const auto MnhSubsystem = UMnhSubsystem::GetMnhSubsystem(GetWorld());
			auto& TracerData = MnhSubsystem->GetTracerDataAt(TracerConfig.TracerDataIdx);
			TracerData.CollisionParams.AddIgnoredActor(Actor);
		}
	}
}

void UMnhTracerComponent::ResetIgnoredActors(const FGameplayTagContainer TracerTags)
{
	for (auto& TracerConfig: TracerConfigs)
	{
		if (!TracerTags.HasTag(TracerConfig.TracerTag))
		{
			continue;
		}

		TracerConfig.CollisionParams.ClearIgnoredActors();
		// TracerConfig.CollisionParams.ClearIgnoredSourceObjects(); // 5.6+

		if (TracerConfig.TracerDataIdx != -1)
		{
			const auto MnhSubsystem = UMnhSubsystem::GetMnhSubsystem(GetWorld());
			auto& TracerData = MnhSubsystem->GetTracerDataAt(TracerConfig.TracerDataIdx);

			TracerData.CollisionParams.ClearIgnoredActors();
			// TracerData.CollisionParams.ClearIgnoredSourceObjects();  // 5.6+
		}
	}
}

void UMnhTracerComponent::AddToIgnoredActorsArray(const FGameplayTagContainer TracerTags, TArray<AActor*> Actors)
{
	for (const auto Actor : Actors)
	{
		AddToIgnoredActors(TracerTags, Actor);
	}
}

void UMnhTracerComponent::StartTracers(const FGameplayTagContainer TracerTags, const bool bResetHitCache)
{
	StartTracersInternal(TracerTags, bResetHitCache, false);
}

int UMnhTracerComponent::AddNewTracer(const FGameplayTag TracerTag, const EMnhTraceSource TraceSource,
	const FMnhTraceSettings& TraceSettings, const EMnhTracerTickType TracerTickType,
	const int TargetFps, const int TargetDistanceTraveled, const EDrawDebugTrace::Type DrawDebugType, const float DrawDebugLifetime)
{
	int TracerConfigIdx = FindTracerConfig(TracerTag);
	if (TracerConfigIdx != -1)
	{
		FMnhTracerConfig& ExistingTracerConfig = TracerConfigs[TracerConfigIdx];
		
		if (ExistingTracerConfig.TraceSource != TraceSource)
		{
			const FString Message = FString::Printf(TEXT("MissNoHit Warning: Tracer with Tag [%s] already exists with different Trace Source."
												"You can only have multiple tracers with same tag if they also have same trace sources"), *TracerTag.ToString());
			FMnhHelpers::Mnh_Log(Message);
			return -1;
		}
	}

	TracerConfigIdx = TracerConfigs.AddDefaulted();
	auto& TracerConfig = TracerConfigs[TracerConfigIdx];
	TracerConfig.OwnerTracerConfigIdx = TracerConfigIdx;
	TracerConfig.TracerTag = TracerTag;
	TracerConfig.TraceSource = TraceSource;
	TracerConfig.TraceSettings = TraceSettings;
	TracerConfig.TracerTickType = TracerTickType;
	TracerConfig.TargetFps = TargetFps;
	TracerConfig.TickDistanceTraveled = TargetDistanceTraveled;
	TracerConfig.DrawDebugType = DrawDebugType;
	TracerConfig.DebugDrawTime = DrawDebugLifetime;
	TracerConfig.OwnerTracerComponent = this;
	return TracerConfigIdx;
}

void UMnhTracerComponent::AddNewMnhComponentTracer(const FGameplayTag TracerTag, const FMnhTraceSettings TraceSettings, const EMnhTracerTickType TracerTickType,
                                                   const int TargetFps, const int TargetDistanceTraveled, const EDrawDebugTrace::Type DrawDebugType, const float DrawDebugLifetime)
{
	const auto TracerIdx =
		AddNewTracer(TracerTag, EMnhTraceSource::MnhShapeComponent, TraceSettings, TracerTickType, TargetFps, TargetDistanceTraveled, DrawDebugType, DrawDebugLifetime);
	if (TracerIdx != -1)
	{
		TracerConfigs[TracerIdx].RegisterTracerData(GetWorld());
	}
}

void UMnhTracerComponent::AddNewPhysicsAssetTracer(const FGameplayTag TracerTag, const FName SocketOrBoneName, const FMnhTraceSettings TraceSettings,
                                                   const EMnhTracerTickType TracerTickType, const int TargetFps, const int TargetDistanceTraveled,
                                                   const EDrawDebugTrace::Type DrawDebugType, const float DrawDebugLifetime)
{
	const auto TracerIdx =
		AddNewTracer(TracerTag, EMnhTraceSource::PhysicsAsset, TraceSettings, TracerTickType, TargetFps, TargetDistanceTraveled, DrawDebugType, DrawDebugLifetime);
	if (TracerIdx != -1)
	{
		auto& TracerConfig = TracerConfigs[TracerIdx];
		TracerConfig.SocketOrBoneName = SocketOrBoneName;
		TracerConfig.RegisterTracerData(GetWorld());
	}
}

void UMnhTracerComponent::AddNewAnimNotifyTracer(const FGameplayTag TracerTag, const FMnhTraceSettings TraceSettings, const EMnhTracerTickType TracerTickType,
                                                 const int TargetFps, const int TargetDistanceTraveled, const EDrawDebugTrace::Type DrawDebugType, const float DrawDebugLifetime)
{
	const auto TracerIdx =
		AddNewTracer(TracerTag, EMnhTraceSource::AnimNotify, TraceSettings, TracerTickType, TargetFps, TargetDistanceTraveled, DrawDebugType, DrawDebugLifetime);
	if (TracerIdx != -1)
	{
		TracerConfigs[TracerIdx].RegisterTracerData(GetWorld());
	}
}

void UMnhTracerComponent::AddNewStaticMeshSocketsTracer(const FGameplayTag TracerTag, const FName MeshSocket1, const FName MeshSocket2, const float MeshSocketTracerRadius,
                                                        const float MeshSocketTracerLengthOffset, const FMnhTraceSettings TraceSettings,
                                                        const EMnhTracerTickType TracerTickType, const int TargetFps, const int TargetDistanceTraveled,
                                                        const EDrawDebugTrace::Type DrawDebugType, const float DrawDebugLifetime)
{
	const auto TracerIdx =
		AddNewTracer(TracerTag, EMnhTraceSource::StaticMeshSockets, TraceSettings, TracerTickType, TargetFps, TargetDistanceTraveled, DrawDebugType, DrawDebugLifetime);
	if (TracerIdx != -1)
	{
		auto& TracerConfig = TracerConfigs[TracerIdx];
		TracerConfig.MeshSocket_1 = MeshSocket1;
		TracerConfig.MeshSocket_2 = MeshSocket2;
		TracerConfig.MeshSocketTracerRadius = MeshSocketTracerRadius;
		TracerConfig.MeshSocketTracerLengthOffset = MeshSocketTracerLengthOffset;
		TracerConfig.RegisterTracerData(GetWorld());
	}
}

void UMnhTracerComponent::AddNewSkeletalMeshSocketsTracer(const FGameplayTag TracerTag, const FName MeshSocket1, const FName MeshSocket2, const float MeshSocketTracerRadius,
                                                          const float MeshSocketTracerLengthOffset, const FMnhTraceSettings TraceSettings,
                                                          const EMnhTracerTickType TracerTickType, const int TargetFps, const int TargetDistanceTraveled,
                                                          const EDrawDebugTrace::Type DrawDebugType, const float DrawDebugLifetime)
{
	const auto TracerIdx =
		AddNewTracer(TracerTag, EMnhTraceSource::SkeletalMeshSockets, TraceSettings, TracerTickType, TargetFps, TargetDistanceTraveled, DrawDebugType, DrawDebugLifetime);
	if (TracerIdx != -1)
	{
		auto& TracerConfig = TracerConfigs[TracerIdx];
		TracerConfig.MeshSocket_1 = MeshSocket1;
		TracerConfig.MeshSocket_2 = MeshSocket2;
		TracerConfig.MeshSocketTracerRadius = MeshSocketTracerRadius;
		TracerConfig.MeshSocketTracerLengthOffset = MeshSocketTracerLengthOffset;
		TracerConfig.RegisterTracerData(GetWorld());
	}
}

void UMnhTracerComponent::StartTracersInternal(const FGameplayTagContainer& TracerTags, bool bResetHitCache, bool AllowAnimNotify=false)
{
	SCOPE_CYCLE_COUNTER(STAT_MNHStartTracer);

	if (bResetHitCache)
	{
	    for (int i = HitCache.Num() - 1; i >= 0; --i)
	    {
	        const auto &HitRecord = HitCache[i];
	        for (const auto TracerTag : TracerTags)
	        {
	            if (HitRecord.TracerTag.MatchesTag(TracerTag))
	            {
	                HitCache.RemoveAt(i);
	                break;
	            }
	        }
	    }
	}

	for (auto& TracerConfig: TracerConfigs)
	{
		if (TracerConfig.SourceComponent == nullptr && TracerConfig.TraceSource != EMnhTraceSource::AnimNotify)
		{
			FMnhHelpers::Mnh_Log("MissNoHit Warning: Tracer Source is not initialized for Tracer Config: "
						"" + TracerConfig.TracerTag.ToString() + " On Actor: " + GetOwner()->GetName());
			continue;
		}
		
		if (TracerTags.HasTagExact(TracerConfig.TracerTag))
		{
			if (!AllowAnimNotify)
			{
				if (TracerConfig.TraceSource == EMnhTraceSource::AnimNotify)
				{
					const FString DebugMessage = FString::Printf(TEXT("MissNoHit Warning: "
															   "AnimNotifyTracer [%s] on Owner [%s] cannot be started manually"),
															   *TracerConfig.TracerTag.ToString(), *GetOwner()->GetName());
					FMnhHelpers::Mnh_Log(DebugMessage);
					continue;
				}
			}

			if (TracerConfig.TracerDataIdx == -1)
			{
				continue;
			}
			
			TracerConfig.ChangeTracerState(GetWorld(), true);
			OnTracerStarted.Broadcast(TracerConfig.TracerTag);
		}
	}
}

void UMnhTracerComponent::StopTracers(const FGameplayTagContainer TracerTags)
{
	for (auto& TracerConfig : TracerConfigs)
	{
		if (TracerTags.HasTagExact(TracerConfig.TracerTag) && TracerConfig.TracerDataIdx != -1)
		{
			TracerConfig.ChangeTracerState(GetWorld(), false);
			OnTracerStopped.Broadcast(TracerConfig.TracerTag);
		}
	}
}

void UMnhTracerComponent::StopTracersDelayed(const FGameplayTagContainer& TracerTags)
{
	for (auto& TracerConfig : TracerConfigs)
	{
		if (TracerTags.HasTagExact(TracerConfig.TracerTag) && TracerConfig.TracerDataIdx != -1)
		{
			TracerConfig.ChangeTracerState(GetWorld(), false, false);
			OnTracerStopped.Broadcast(TracerConfig.TracerTag);
		}
	}
}

int UMnhTracerComponent::FindTracerConfig(const FGameplayTag TracerTag)
{
	for (int Idx = 0; Idx < TracerConfigs.Num(); Idx++)
	{
		const FMnhTracerConfig& TracerConfig = TracerConfigs[Idx];
		if (TracerConfig.TracerTag.MatchesTagExact(TracerTag))
		{
			return Idx;
		}
	}
	return -1;
}

// Called when the game starts
void UMnhTracerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (int TracerConfigIdx = 0; TracerConfigIdx < TracerConfigs.Num(); TracerConfigIdx++)
	{
		auto& TracerConfig = TracerConfigs[TracerConfigIdx];
		TracerConfig.OwnerTracerConfigIdx = TracerConfigIdx;
		TracerConfig.OwnerTracerComponent = this;
		TracerConfig.RegisterTracerData(GetWorld());
	}
	
	bIsInitialized = true;
	
	for (const auto& [TracerTags, TracerSource] : EarlyTracerInitializations)
	{
		InitializeTracers(TracerTags, TracerSource);
	}
}

bool UMnhTracerComponent::CheckFilters(const FHitResult& HitResult, const FGameplayTag TracerTag, int TickIdxArg)
{
	SCOPE_CYCLE_COUNTER(STAT_MNHCheckFilters);

	if (HitCache.Num() > 2048)
	{
		HitCache.Reset();
		const FString Message = FString::Printf(TEXT("MissNoHit Warning: Your Tracer Hit Cache"
													 " contains over 2048 elements, it is very likely that you are not resetting"
													 "your Hit Cache, this can lead to performance and memory leak problems!"
													 " Please enable MissNoHit tracers only when they are needed"));
		FMnhHelpers::Mnh_Log(Message);
	}

	for (auto& HitRecord : HitCache)
    {
        if (FilterType == EMnhFilterType::FilterSameActorPerTracer)
        {
        	if (HitRecord.TracerTag.MatchesTag(TracerTag) && HitRecord.HitResult.GetActor() == HitResult.GetActor())
        	{
        		return false;
        	}
        }
        else if (FilterType == EMnhFilterType::FilterSameActorAcrossAllTracers)
        {
        	if (HitRecord.HitResult.GetActor() == HitResult.GetActor())
        	{
        		return false;
        	}
        }
    }
	
	return true;
}

void UMnhTracerComponent::OnTracerHitDetected(const FGameplayTag TracerTag, const TArray<FHitResult>& HitResults, const float DeltaTime, const int TickIdx)
{
	SCOPE_CYCLE_COUNTER(STAT_MnhTracerComponentHitDetected)
	FScopeLock ScopedLock(&TraceDoneScopeLock);

	for (const auto& HitResult : HitResults)
	{
		if (FilterType == EMnhFilterType::None)
		{
			OnHitDetected.Broadcast(TracerTag, HitResult, DeltaTime);
		}
		else if(CheckFilters(HitResult, TracerTag, TickIdx))
		{
			HitCache.Add(FMnhHitCache{HitResult, TracerTag, TickIdx});
			OnHitDetected.Broadcast(TracerTag, HitResult, DeltaTime);
		}
	}
}

