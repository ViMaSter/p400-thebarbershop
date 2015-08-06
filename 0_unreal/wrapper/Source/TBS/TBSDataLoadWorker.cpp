// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDataLoadWorker.h"

#pragma region MT Load

//***********************************************************
//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
//***********************************************************
FTBSDataLoadWorker* FTBSDataLoadWorker::Runnable = NULL;

FTBSDataLoadWorker::FTBSDataLoadWorker()
	: StopTaskCounter(0)
{

	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}
/*
FTBSDataLoadWorker::FTBSDataLoadWorker(TArray<FTBSEquipmentData*>& TheArray, UDataTable* IN_DataTable)
	: StopTaskCounter(0)
{
	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FTBSDataLoadWorker::FTBSDataLoadWorker(TArray<FLevelUpData*>& TheArray, UDataTable* IN_DataTable)
	: StopTaskCounter(0)
{

	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}
FTBSDataLoadWorker::FTBSDataLoadWorker(TArray<FTimeBonusData*>& TheArray, UDataTable* IN_DataTable)
	: StopTaskCounter(0)
{

	Thread = FRunnableThread::Create(this, TEXT("FTBSDataLoadWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}
*/

FTBSDataLoadWorker::~FTBSDataLoadWorker() {
	delete Thread;
	Thread = NULL;
}

//Init
bool FTBSDataLoadWorker::Init() {
	//Init the Data 

	UE_LOG(LogClass, Log, TEXT("*************************************"));
	UE_LOG(LogClass, Log, TEXT("*** Load CSV Data Thread Started! ***"));
	UE_LOG(LogClass, Log, TEXT("*************************************"));

	return true;
}

//Run
uint32 FTBSDataLoadWorker::Run() {
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);
	TArray<FMTRunningTask> TmpArray;
	for(FMTRunningTask Task : RunningTasks){
		switch (Task.Type)
		{
		case ETBSMultiThreadingTask::BeardComparison:
			while (!Task.LoadingFinished)
			{
				LoadBeardCompData(Task);
				Task.RowCount++;
			}
			break;
		case ETBSMultiThreadingTask::Equipment:
			while (!Task.LoadingFinished)
			{
				LoadEquipmentData(Task);
				Task.RowCount++;
			}
			break;
		case ETBSMultiThreadingTask::Level:
			while (!Task.LoadingFinished)
			{
				LoadLevelData(Task);
				Task.RowCount++;
			}
			break;
		case ETBSMultiThreadingTask::Bonus:
			while (!Task.LoadingFinished)
			{
				LoadBonusData(Task);
				Task.RowCount++;
			}
			break;
		}
		TmpArray.Add(Task);
	}
	for (FMTRunningTask Task : TmpArray) {
		RunningTasks.Remove(Task);
	}
	return 0;
}

//stop
void FTBSDataLoadWorker::Stop()
{
	StopTaskCounter.Increment();
}

void FTBSDataLoadWorker::StartNewTaskBeardComp(TArray<FBeardComparisonData*>& TheArray, UDataTable* DataTable) {

	FMTRunningTask Task;
	Task.DataTable = DataTable;
	BeardCompData = &TheArray;
	Task.LoadingFinished = false;
	Task.Type = ETBSMultiThreadingTask::BeardComparison;
	Task.RowCount = 0;

	RunningTasks.Add(Task);
}

void FTBSDataLoadWorker::StartNewTaskEquipment(TArray<FTBSEquipmentData*>& TheArray, UDataTable* DataTable) {

	FMTRunningTask Task;
	Task.DataTable = DataTable;
	EquipmentDataData = &TheArray;
	Task.LoadingFinished = false;
	Task.Type = ETBSMultiThreadingTask::Equipment;
	Task.RowCount = 0;

	RunningTasks.Add(Task);

}

void FTBSDataLoadWorker::StartNewTaskLevel(TArray<FLevelUpData*>& TheArray, UDataTable* DataTable) {

	FMTRunningTask Task;
	Task.DataTable = DataTable;
	LevelData = &TheArray;
	Task.LoadingFinished = false;
	Task.Type = ETBSMultiThreadingTask::Level;
	Task.RowCount = 0;

	RunningTasks.Add(Task);
}

void FTBSDataLoadWorker::StartNewTaskBonus(TArray<FTimeBonusData*>& TheArray, UDataTable* DataTable) {

	FMTRunningTask Task;
	Task.DataTable = DataTable;
	BonusData = &TheArray;
	Task.LoadingFinished = false;
	Task.Type = ETBSMultiThreadingTask::Bonus;
	Task.RowCount = 0;

	RunningTasks.Add(Task);
}



FTBSDataLoadWorker* FTBSDataLoadWorker::JoyInitBeardComp(TArray<FBeardComparisonData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker();
	}
	Runnable->StartNewTaskBeardComp(TheArray, IN_DataTable);
	return Runnable;
}

FTBSDataLoadWorker* FTBSDataLoadWorker::JoyInitEquipment(TArray<FTBSEquipmentData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker();
	}
	Runnable->StartNewTaskEquipment(TheArray, IN_DataTable);
	return Runnable;
}

FTBSDataLoadWorker*  FTBSDataLoadWorker::JoyInitLevel(TArray<FLevelUpData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker();
	}
	Runnable->StartNewTaskLevel(TheArray, IN_DataTable);
	return Runnable;
}

FTBSDataLoadWorker*  FTBSDataLoadWorker::JoyInitBonus(TArray<FTimeBonusData*>& TheArray, UDataTable* IN_DataTable) {
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTBSDataLoadWorker();
	}
	Runnable->StartNewTaskBonus(TheArray, IN_DataTable);
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

void FTBSDataLoadWorker::LoadBeardCompData(FMTRunningTask &Task) {
	FBeardComparisonData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(Task.RowCount);
	FName Row = FName(*String);
	CurrentData = Task.DataTable->FindRow<FBeardComparisonData>(Row, Context, false);
	if (CurrentData == NULL) {
		Task.LoadingFinished = true;
	}
	else{
		BeardCompData->Add(CurrentData);
	}
}

void FTBSDataLoadWorker::LoadEquipmentData(FMTRunningTask &Task) {
	FTBSEquipmentData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(Task.RowCount);
	FName Row = FName(*String);
	CurrentData = Task.DataTable->FindRow<FTBSEquipmentData>(Row, Context, false);
	if (CurrentData == NULL) {
		Task.LoadingFinished = true;
	}
	else{
		EquipmentDataData->Add(CurrentData);
	}
}

void FTBSDataLoadWorker::LoadLevelData(FMTRunningTask &Task) {
	FLevelUpData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(Task.RowCount);
	FName Row = FName(*String);
	CurrentData = Task.DataTable->FindRow<FLevelUpData>(Row, Context, false);
	if (CurrentData == NULL) {
		Task.LoadingFinished = true;
	}
	else{
		LevelData->Add(CurrentData);
	}
}

void FTBSDataLoadWorker::LoadBonusData(FMTRunningTask &Task) {
	FTimeBonusData* CurrentData;
	const FString Context = FString("");
	bool success = true;
	FString String = FString::FromInt(Task.RowCount);
	FName Row = FName(*String);
	CurrentData = Task.DataTable->FindRow<FTimeBonusData>(Row, Context, false);
	if (CurrentData == NULL) {
		Task.LoadingFinished = true;
	}
	else{
		BonusData->Add(CurrentData);
	}
}


#pragma endregion