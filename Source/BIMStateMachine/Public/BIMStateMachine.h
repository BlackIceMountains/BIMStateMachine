// Copyright Black Ice Mountains. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BIMStateMachine.generated.h"

class UCanvas;
class FDebugDisplayInfo;

/**
 * Base state of the state machine
 */
UCLASS()
class BIMSTATEMACHINE_API UBIMStateMachineBaseState : public UObject
{
	GENERATED_BODY()

public:

	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual bool CanEnter() const { return false; }
	virtual bool CanExit() const { return true; }

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos);


	UBIMStateMachineBaseState& AddTransitionTo(UBIMStateMachineBaseState& State);
	UBIMStateMachineBaseState& AddInterruptTransitionTo(UBIMStateMachineBaseState& State);

	UPROPERTY()
	TArray<UBIMStateMachineBaseState*> Transitions;

	UPROPERTY()
	TArray<UBIMStateMachineBaseState*> InterruptTransitions;
};



/**
 * Generic state machine
 */
UCLASS()
class BIMSTATEMACHINE_API UBIMStateMachine : public UObject
{
	GENERATED_BODY()

public:
	UBIMStateMachine();
	virtual ~UBIMStateMachine() override;

	void Update();
	void Enable(bool bEnable);

	UBIMStateMachineBaseState& RegisterState(UBIMStateMachineBaseState* State);

	template <typename STATE_TYPE>
	STATE_TYPE& AddState(FName Name);

	/** Get name of the active state. */
	FName GetCurrentStateName() const;

	void SetInitialState(UBIMStateMachineBaseState* State);

	void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos);

private:

	void ProcessTransitions(bool Interrupts);

	bool bEnabled = false;

	UPROPERTY()
	TArray<UBIMStateMachineBaseState*> States;

	UPROPERTY()
	UBIMStateMachineBaseState* CurrentState;
};

template <typename STATE_TYPE>
STATE_TYPE& UBIMStateMachine::AddState(FName Name)
{
	static_assert(std::is_base_of<UBIMStateMachineBaseState, STATE_TYPE>::value, "Passed state type must inherit from UBIMStateMachineBaseState");
	auto State = NewObject<STATE_TYPE>(this, Name);
	RegisterState(State);
	return *State;
}
