// Copyright Black Ice Mountains. All Rights Reserved.

#include "BIMStateMachine.h"

#include "Engine/Canvas.h"

UBIMStateMachine::UBIMStateMachine() = default;
UBIMStateMachine::~UBIMStateMachine() = default;

void UBIMStateMachine::Update()
{
	if (!CurrentState)
	{
		return;
	}


	bool InterruptsOnly = !CurrentState->CanExit();
	ProcessTransitions(InterruptsOnly);
}


void UBIMStateMachine::ProcessTransitions(bool Interrupts)
{
	const TArray<UBIMStateMachineBaseState*>& Transitions = Interrupts ? CurrentState->InterruptTransitions : CurrentState->Transitions;
	for(UBIMStateMachineBaseState* State : Transitions)
	{
		if (State == CurrentState)
		{
			continue;
		}

		if (State->CanEnter())
		{
			CurrentState->OnExit();
			CurrentState = State;
			CurrentState->OnEnter();
			break;
		}
	}
}

UBIMStateMachineBaseState& UBIMStateMachine::RegisterState(UBIMStateMachineBaseState* State)
{
	States.Add(State);
	return *State;
}

void UBIMStateMachine::SetInitialState(UBIMStateMachineBaseState* State)
{
	verify(States.Find(State) != INDEX_NONE);
	verify(!CurrentState);
	CurrentState = State;
}

FName UBIMStateMachine::GetCurrentStateName() const
{
	if (CurrentState)
	{
		return CurrentState->GetFName();
	}
	return NAME_None;
}

void UBIMStateMachine::Enable(bool bEnable)
{
	if (bEnabled == bEnable)
	{
		return;
	}

	bEnabled = bEnable;

	if (CurrentState)
	{
		if (bEnable)
		{
			CurrentState->OnEnter();
		}
		else
		{
			CurrentState->OnExit();
		}
	}

}

void UBIMStateMachine::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetDrawColor(FColor::Orange);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("** State machine - %s **"), *GetName()));

	for (UBIMStateMachineBaseState* State : States)
	{
		DisplayDebugManager.SetDrawColor((State == CurrentState) ? FColor::Green : FColor::White);
		DisplayDebugManager.DrawString(State->GetName(), 10.0f);

		State->DisplayDebug(Canvas, DebugDisplay, YL, YPos);
	}
}

void UBIMStateMachineBaseState::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
}

UBIMStateMachineBaseState& UBIMStateMachineBaseState::AddTransitionTo(UBIMStateMachineBaseState& State)
{
	checkf(Transitions.Find(&State) == INDEX_NONE, TEXT("Tried to add duplicate transition between state %s and %s"), *GetName(), *State.GetName());
	checkf(InterruptTransitions.Find(&State) == INDEX_NONE, TEXT("Tried to add duplicate transition between state %s and %s"), *GetName(), *State.GetName());
	Transitions.Add(&State);
	return *this;
}

UBIMStateMachineBaseState& UBIMStateMachineBaseState::AddInterruptTransitionTo(UBIMStateMachineBaseState& State)
{
	checkf(Transitions.Find(&State) == INDEX_NONE, TEXT("Tried to add duplicate transition between state %s and %s"), *GetName(), *State.GetName());
	checkf(InterruptTransitions.Find(&State) == INDEX_NONE, TEXT("Tried to add duplicate transition between state %s and %s"), *GetName(), *State.GetName());
	InterruptTransitions.Add(&State);
	return *this;
}
