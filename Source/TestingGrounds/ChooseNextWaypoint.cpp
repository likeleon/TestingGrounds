// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrollingGuard.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr || AIController->GetPawn() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APatrollingGuard* Guard = Cast<APatrollingGuard>(AIController->GetPawn());
	if (Guard == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const TArray<AActor*>& PatrolPoints = Guard->PatrolPoints;
	if (PatrolPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	// Set next waypoint
	int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	if (Index >= PatrolPoints.Num())
	{
		return EBTNodeResult::Failed;
	}
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle waypoint index
	int32 NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
