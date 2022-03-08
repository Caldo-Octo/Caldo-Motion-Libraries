var ManualDispenseComplete:boolean;
var Containers: Array[1..28] of TContainer;
var Gantry:TGantry;
var GantryEnabled:boolean;
var CurrentContainerNo:integer;
var CurrentContainerType:integer; {1 Auger, 2 Pump}
var LoadCellScaled:double;

const Col1Coord = 133.0;
const Col2Coord = 288.1;
const Col3Coord = 443.0;
const Col4Coord = 597.9;
const Col5Coord = 771.0;
const Row1Coord = 1464.5;
const Row2Coord = 979.2;
const Row3Coord = 500.0;
const Row4Coord = 18.5;
const AugDefaultSpeed = 20.0;
const LPumpDefaultX = -75.0;
const RPumpDefaultX = -55.0;

const PumpsDefaultSpeed = 40;


{-----------------Helper Functions----------------------}
	{We can't use object methods directly when calling a new task so 
	they have to be wrapped in a function that can}
	
procedure DispenseWrapper;
	begin
		Containers[CurrentContainerNo].Dispense;
	end;
	
procedure ZigPatternWrapper;
	begin
		Containers[CurrentContainerNo].ZigZagPattern(5,5,5)
	end;
procedure LinePatternWrapper;
	begin
		Containers[CurrentContainerNo].StraightLinePattern(5)
	end;
{-----------------End Helper Functions------------------}	


procedure InitializeActuators;
	begin
		{We need to change the Remote_Motor_Names eventually}
		Containers[1].InitializeAuger(Remote_Motor_27,Col1Coord,Row1Coord,1,AugDefaultSpeed);
		Containers[2].InitializeAuger(Remote_Motor_28,Col2Coord,Row1Coord,2,AugDefaultSpeed);
		Containers[3].InitializeAuger(Remote_Motor_25,Col3Coord,Row1Coord,3,AugDefaultSpeed);
		Containers[4].InitializeAuger(Remote_Motor_26,Col4Coord,Row1Coord,4,AugDefaultSpeed);
		Containers[5].InitializeAuger(Remote_Motor_23,Col5Coord,Row1Coord,5,AugDefaultSpeed);
		Containers[6].InitializeAuger(Remote_Motor_24,Col1Coord,Row2Coord,6,AugDefaultSpeed);
		Containers[7].InitializeAuger(Remote_Motor_21,Col2Coord,Row2Coord,7,AugDefaultSpeed);
		Containers[8].InitializeAuger(Remote_Motor_22,Col3Coord,Row2Coord,8,AugDefaultSpeed);
		Containers[9].InitializeAuger(Remote_Motor_19,Col4Coord,Row2Coord,9,AugDefaultSpeed);
		Containers[10].InitializeAuger(Remote_Motor_20,Col5Coord,Row2Coord,10,AugDefaultSpeed);
		Containers[11].InitializeAuger(Remote_Motor_21,Col1Coord,Row3Coord,11,AugDefaultSpeed);
		Containers[12].InitializeAuger(Remote_Motor_18,Col2Coord,Row3Coord,12,AugDefaultSpeed);
		Containers[13].InitializeAuger(Remote_Motor_15,Col3Coord,Row3Coord,13,AugDefaultSpeed);
		Containers[14].InitializeAuger(Remote_Motor_16,Col4Coord,Row3Coord,14,AugDefaultSpeed);
		Containers[15].InitializeAuger(Remote_Motor_13,Col5Coord,Row3Coord,15,AugDefaultSpeed);
		Containers[16].InitializeAuger(Remote_Motor_10,Col1Coord,Row4Coord,16,AugDefaultSpeed);
		Containers[17].InitializeAuger(Remote_Motor_9,Col2Coord,Row4Coord,17,AugDefaultSpeed);
		Containers[18].InitializeAuger(Remote_Motor_12,Col3Coord,Row4Coord,18,AugDefaultSpeed);
		Containers[19].InitializeAuger(Remote_Motor_11,Col4Coord,Row4Coord,19,AugDefaultSpeed);
		Containers[20].InitializeAuger(Remote_Motor_14,Col5Coord,Row4Coord,20,AugDefaultSpeed);
		
		Containers[21].InitalizePump(Remote_Motor_8,LPumpDefaultX,Row1Coord,21,ContainersDefaultSpeed);
		Containers[22].InitalizePump(Remote_Motor_7,RPumpDefaultX,Row1Coord,22,ContainersDefaultSpeed);
		Containers[23].InitalizePump(Remote_Motor_6,LPumpDefaultX,Row1Coord,23,ContainersDefaultSpeed);
		Containers[24].InitalizePump(Remote_Motor_5,RPumpDefaultX,Row1Coord,24,ContainersDefaultSpeed);
		Containers[25].InitalizePump(Remote_Motor_4,LPumpDefaultX,Row1Coord,25,ContainersDefaultSpeed);
		Containers[26].InitalizePump(Remote_Motor_3,RPumpDefaultX,Row1Coord,26,ContainersDefaultSpeed);
		Containers[27].InitalizePump(Remote_Motor_2,LPumpDefaultX,Row1Coord,27,ContainersDefaultSpeed);
		Containers[28].InitalizePump(Augar1,RPumpDefaultX,Row1Coord,28,ContainersDefaultSpeed);
		
		Gantry.Initialize(X_Motor,YMotor,Stepper,StepperEncoder);
	end;
	
	
	procedure AutoDispense;
		var CurrentWeightSP:double;
		begin
		if GantryEnabled then
			begin
			if CurrentContainerNo <= 20 then
				Gantry.JointAxisMove(Containers[CurrentContainerNo])
			else
				begin
				Gantry.MoveToStaging(Containers[CurrentContainerNo]);
				Gantry.MoveToContainers(Containers[CurrentContainerNo]);
				end;
			end;
		CurrentWeightSP:=Containers[CurrentContainerNo].WeightSP+LoadCellScaled;
		BeginTask(TaskAddr(DispenseWrapper));
		if Containers[CurrentContainerNo].GantryPatternEnabled then
			begin
				if Containers[CurrentContainerNo].GantryPattern = 1 then
					BeginTask(TaskAddr(ZigPatternWrapper))
				else if Containers[CurrentContainerNo].GantryPattern = 2 then
					BeginTask(TaskAddr(LinePatternWrapper));
			end;
		while LoadCellScaled < CurrentWeightSP do
			yield;
		Containers[CurrentContainerNo].DispenseEnable:=false;
		while TaskPresent(TaskAddr(DispenseWrapper)) and (TaskPresent(TaskAddr(ZigPatternWrapper)) or TaskPresent(TaskAddr(LinePatternWrapper))) do
			yield;
		end;