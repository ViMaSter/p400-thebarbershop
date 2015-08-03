// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "TBSBeard.h"
#include "TBSProgression.h"
#include "TBSEquipment.h"

UENUM()
enum class ETBSMultiThreadingTask : uint8 {
	BeardComparison = 0,
	Equipment = 1,
	Level = 2,
	Bonus = 3
};

class FTBSDataLoadWorker : public FRunnable
{
	/** Singleton instance, can access the thread any time via static accessor, if it is active! */
	static FTBSDataLoadWorker* Runnable;

	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;

	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter StopTaskCounter;

	TArray<FBeardComparisonData*>* BeardCompData;
	TArray<FTBSEquipmentData*>* EquipmentDataData;
	TArray<FLevelUpData*>* LevelData;
	TArray<FTimeBonusData*>* BonusData;

	void LoadBeardCompData();
	void LoadEquipmentData();
	void LoadLevelData();
	void LoadBonusData();

private:
	int32				RowCount;
	bool				LoadingFinished;
	ETBSMultiThreadingTask Task;

public:

	UDataTable*			DataTable;

	//Done?
	bool IsFinished() const
	{
		return LoadingFinished;
	}
	
	//Constructor for every possible task (workaround until i found a solution to do it with one array of anything)
	FTBSDataLoadWorker(TArray<FBeardComparisonData*>& TheArray, UDataTable* DataTable);
	FTBSDataLoadWorker(TArray<FTBSEquipmentData*>& TheArray, UDataTable* DataTable);
	FTBSDataLoadWorker(TArray<FLevelUpData*>& TheArray, UDataTable* DataTable);
	FTBSDataLoadWorker(TArray<FTimeBonusData*>& TheArray, UDataTable* DataTable);
	
	// Destructor
	virtual ~FTBSDataLoadWorker();

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	//~~~ Starting Thread ~~~
	// Init function for every possible task (workaround until i found a solution to do it with one array of anything)
	static FTBSDataLoadWorker* JoyInitBeardComp(TArray<FBeardComparisonData*>& TheArray, UDataTable* DataTable); 
	static FTBSDataLoadWorker* JoyInitEquipment(TArray<FTBSEquipmentData*>& TheArray, UDataTable* DataTable);
	static FTBSDataLoadWorker* JoyInitLevel(TArray<FLevelUpData*>& TheArray, UDataTable* DataTable);
	static FTBSDataLoadWorker* JoyInitBonus(TArray<FTimeBonusData*>& TheArray, UDataTable* DataTable);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();

	static bool IsThreadFinished();

};
