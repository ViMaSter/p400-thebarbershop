// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDataLoadWorker.h"

#pragma region MT Load

//***********************************************************
//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
//***********************************************************
FTBSDataLoadWorker* FTBSDataLoadWorker::Runnable = NULL;

FTBSDataLoadWorker::FTBSDataLoadWorker(TArray<FBeardComparisonData*>& TheArray, UDataTable* IN_DataTable)
	: DataTable(IN_DataTable)
	, StopTaskCounter(0)
	, RowCount(0)
{
	BeardCompData = &TheArray;
	LoadingFinished = false;
	Task = ETBSMultiThreadingTask::BeardComparison;

	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FTBSDataLoadWorker::FTBSDataLoadWorker(TArray<FTBSEquipmentData*>& TheArray, UDataTable* IN_DataTable)
	: DataTable(IN_DataTable)
	, StopTaskCounter(0)
	, RowCount(0)
{
	EquipmentDataData = &TheArray;
	LoadingFinished = false;
	Task = ETBSMultiThreadingTask::Equipment;

	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FTBSDataLoadWorker::FTBSDataLoadWorker(TArray<FLevelUpData*>& TheArray, UDataTable* IN_DataTable)
	: DataTable(IN_DataTable)
	, StopTaskCounter(0)
	, RowCount(0)
{
	LevelData = &TheArray;
	LoadingFinished = false;
	Task = ETBSMultiThreadingTask::Level;

	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}
FTBSDataLoadWorker::FTBSDataLoadWorker(TArray<FTimeBonusData*>& TheArray, UDataTable* IN_DataTable)
	: DataTable(IN_DataTable)
	, StopTaskCounter(0)
	, RowCount(0)
{
	BonusData = &TheArray;
	LoadingFinished = false;
	Task = ETBSMultiThreadingTask::Bonus;

	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FTBSDataLoadWorker::~FTBSDataLoadWorker() {
	delete Thread;
	Thread = NULL;
}

//Init
bool FTBSDataLoadWorker::Init() {
	//Init the Data 
	BeardCompData->Empty();

	UE_LOG(LogClass, Log, TEXT("*************************************"));
	UE_LOG(LogClass, Log, TEXT("*** Load CSV Data Thread Started! ***"));
	UE_LOG(LogClass, Log, TEXT("*************************************"));

	return true;
}

//Run
uint32 FTBSDataLoadWorker::Run() {
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	switch (Task)
	{
	case ETBSMultiThreadingTask::BeardComparison:
		while (!IsFinished())
		{
			LoadBeardCompData();
			RowCount++;
		}
		break;
	case ETBSMultiThreadingTask::Equipment:
		while (!IsFinished())
		{
			LoadEquipmentData();
			RowCount++;
		}
		break;
	case ETBSMultiThreadingTask::Level:
		while (!IsFinished())
		{
			LoadLevelData();
			RowCount++;
		}
		break;
	case ETBSMultiThreadingTask::Bonus:
		while (!IsFinished())
		{
			LoadBonusData();
			RowCount++;
		}
		break;
	}

	return 0;
}

//stop
void FTBSDataLoadWorker::Stop()
{
	StopTaskCounter.Increment();
}

FTBSDataLoadWorker* FTBSDataLoadWorker::JoyInitBeardComp(TArray<FBeardComparisonData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker(TheArray, IN_DataTable);
	}
	return Runnable;
}

FTBSDataLoadWorker* FTBSDataLoadWorker::JoyInitEquipment(TArray<FTBSEquipmentData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker(TheArray, IN_DataTable);
	}
	return Runnable;
}

FTBSDataLoadWorker*  FTBSDataLoadWorker::JoyInitLevel(TArray<FLevelUpData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker(TheArray, IN_DataTable);
	}
	return Runnable;
}
FTBSDataLoadWorker*  FTBSDataLoadWorker::JoyInitBonus(TArray<FTimeBonusData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker(TheArray, IN_DataTable);
	}
	return Runnable;
}

void FTBSDataLoadWorker::EnsureCompletion() {
	Stop();
	if (Thread)	{
		Thread->WaitForCompletion();
	}
}

void FTBSDataLoadWorker::Shutdown() {
	if (Runnable)
	{
		if (Runnable->Thread) {
			delete Runnable->Thread;
			Runnable->Thread = NULL;
		}
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool FTBSDataLoadWorker::IsThreadFinished() {
	if (Runnable) return Runnable->IsFinished();
	return true;
}

void FTBSDataLoadWorker::LoadBeardCompData() {
	FBeardComparisonData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(RowCount);
	FName Row = FName(*String);
	CurrentData = DataTable->FindRow<FBeardComparisonData>(Row, Context, false);
	if (CurrentData == NULL) {
		LoadingFinished = true;
	}
	else{
		BeardCompData->Add(CurrentData);
	}
}

void FTBSDataLoadWorker::LoadEquipmentData() {
	FTBSEquipmentData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(RowCount);
	FName Row = FName(*String);
	CurrentData = DataTable->FindRow<FTBSEquipmentData>(Row, Context, false);
	if (CurrentData == NULL) {
		LoadingFinished = true;
	}
	else{
		EquipmentDataData->Add(CurrentData);
	}
}

void FTBSDataLoadWorker::LoadLevelData() {
	FLevelUpData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(RowCount);
	FName Row = FName(*String);
	CurrentData = DataTable->FindRow<FLevelUpData>(Row, Context, false);
	if (CurrentData == NULL) {
		LoadingFinished = true;
	}
	else{
		LevelData->Add(CurrentData);
	}
}

void FTBSDataLoadWorker::LoadBonusData() {
	FTimeBonusData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(RowCount);
	FName Row = FName(*String);
	CurrentData = DataTable->FindRow<FTimeBonusData>(Row, Context, false);
	if (CurrentData == NULL) {
		LoadingFinished = true;
	}
	else{
		BonusData->Add(CurrentData);
	}
}


#pragma endregion