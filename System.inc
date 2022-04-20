{================System Constants=================}

{=================System Wide Variables============}
var GlobalStartSignal:boolean;
var OctoMode:SystemMode;
var Containers: array[1..28] of TContainer;
var Gantry:TGantry;
var LoadCellScaled:double;
var OctoState:integer;
var GantryEnabled:boolean;
var GlobalContainerNo:integer;
var SystemInAuto:boolean;
var SystemReady:boolean;
var ManualMode:boolean;
var SystemStatusEnum:integer;
var LoadCell:TAnalogInput;

procedure InitializeSubsystems;
	begin
		LoadCell.Initialize(16,1);
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
		yield;
		Containers[11].InitializeAuger(Remote_Motor_17,Col1Coord,Row3Coord,11,AugDefaultSpeed);
		Containers[12].InitializeAuger(Remote_Motor_18,Col2Coord,Row3Coord,12,AugDefaultSpeed);
		Containers[13].InitializeAuger(Remote_Motor_15,Col3Coord,Row3Coord,13,AugDefaultSpeed);
		Containers[14].InitializeAuger(Remote_Motor_16,Col4Coord,Row3Coord,14,AugDefaultSpeed);
		Containers[15].InitializeAuger(Remote_Motor_13,Col5Coord,Row3Coord,15,AugDefaultSpeed);
		Containers[16].InitializeAuger(Remote_Motor_14,Col1Coord,Row4Coord,16,AugDefaultSpeed);
		Containers[17].InitializeAuger(Remote_Motor_11,Col2Coord,Row4Coord,17,AugDefaultSpeed);
		Containers[18].InitializeAuger(Remote_Motor_12,Col3Coord,Row4Coord,18,AugDefaultSpeed);
		Containers[19].InitializeAuger(Remote_Motor_9,Col4Coord,Row4Coord,19,AugDefaultSpeed);
		Containers[20].InitializeAuger(Remote_Motor_10,Col5Coord,Row4Coord,20,AugDefaultSpeed);
		yield;	
		Containers[21].InitializePump(Remote_Motor_8,LPumpDefaultX,Row1Coord,21,PumpsDefaultSpeed);
		Containers[22].InitializePump(Remote_Motor_7,RPumpDefaultX,Row1Coord,22,PumpsDefaultSpeed);
		Containers[23].InitializePump(Remote_Motor_6,LPumpDefaultX,Row2Coord,23,PumpsDefaultSpeed);
		Containers[24].InitializePump(Remote_Motor_5,RPumpDefaultX,Row2Coord,24,PumpsDefaultSpeed);
		Containers[25].InitializePump(Remote_Motor_4,LPumpDefaultX,Row3Coord,25,PumpsDefaultSpeed);
		Containers[26].InitializePump(Remote_Motor_3,RPumpDefaultX,Row3Coord,26,PumpsDefaultSpeed);
		Containers[27].InitializePump(Remote_Motor_2,LPumpDefaultX,Row4Coord,27,PumpsDefaultSpeed);
		Containers[28].InitializePump(Augar1,RPumpDefaultX,Row4Coord,28,PumpsDefaultSpeed);
		yield;
		Gantry.Initialize(X_Motor,Y_Motor,Stepper,StepperEncoder);
		SystemReady:=true;
		if (X_Motor.ActualPosition > 99) and (X_Motor.ActualPosition < 101) and (Y_Motor.ActualPosition<10) then
			Gantry.AtHome:=true;
        Gantry.XMotor.SetMotor(on);
        Gantry.YMotor.SetMotor(on);
        Gantry.ZMotor.SetMotor(on);
	end;

function UpdateLoadCellScaled(LoadCell_:TAnalogInput;ScalingFactor:double):double;
    begin
        UpdateLoadCellScaled:=LoadCell_.Value*ScalingFactor;
    end;

{-----------------Task Wrapper Functions----------------------}
	{We can't use object methods directly when calling a new task so 
	they have to be wrapped in a function that can}
	
procedure DispenseWrapper;
	begin
		Containers[CurrentContainerNo].Dispense;
	end;
	
procedure ZigPatternWrapper;
	begin
		Gantry.ZigZagPattern(5,5,5)
	end;
procedure LinePatternWrapper;
	begin
		Gantry.StraightLinePattern(5)
	end;
{-----------------End Task Wraper Functions------------------}	

procedure ButtonHandler(Container:integer);
	var DispenseTaskPresent:boolean;
	begin
	DispenseTaskPresent:=TaskPresent(TaskAddr(DispenseWrapper));
	if not DispenseTaskPresent and OctoMode.InManual then
		begin
			if Container <= 28 then
				begin
                    OctoMode.CurrentContainerNo:=Container;
					OctoMode.StartCycle;
				end
			else if Container = 29 then
				begin
					Gantry.MoveToDenesterStaging;
					Gantry.MoveToDenester;
					Delay(4000);
					Denester.SetValue(0);
					Gantry.ZMotor.MoveTo(0);
					Gantry.MoveToDenesterStaging;
				end
			else if CurrentContainer = 30 then
				begin
					Gantry.MoveToOutfeedStaging;
					Delay(1000);
					Gantry.MoveToOutfeed;
				end
			else if CurrentContainer = 40 then
				begin
					Gantry.GoToIdlePos;
				end;
		end;
	end;