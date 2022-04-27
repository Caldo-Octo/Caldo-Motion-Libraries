{==================================================================Systemwide Constants======================================================}

const Col1Coord = 151;
const Col2Coord = 301;
const Col3Coord = 465;
const Col4Coord = 625;
const Col5Coord = 780;
const Row1Coord = 1464;
const Row2Coord = 987.5;
const Row3Coord = 495;
const Row4Coord = 18;
const ZDispensePos = 9;
const AugDefaultSpeed = 20;
const LPumpDefaultX = -75;
const RPumpDefaultX = -55;
const PumpsDefaultSpeed = 80;
const OutfeedXPos = 1084;
const OutfeedYPos = 890.42;
const OutfeedZPos = 3.6;
const PumpStagingX = 100.0;
const DenesterStagingX = 800;
const DenesterStagingY = 1128;
const DenesterStagingZ = 0;
const OutfeedStagingX = 924;
const OutfeedStagingY = 956.41;
const OutfeedStagingZ = 3.6;
const DenesterXPos = 1100;
const DenesterYPos = 1128;
const DenesterZPos = 7;

{==================================================================Type Defs===============================================================}
type TContainer=object
	Motor:TDistributedT1Axis;
	Id:integer;
	XCoord:double;
	YCoord:double;
	ZCoord:double;
	IsPump:boolean;
	StagingXCoord:double;
	StagingYCoord:double;
	DispenseSpeed:single;
	ReverseSpeed:single;
	Accel:single;
	Decel:single;
	Current:double;
	WeightSP:double;
	ReverseDelay:double;
	DispensePermissive:boolean;
	ReverseEnabled:boolean;
	DispenseComplete:boolean;
	Revolutions:double;
	ReverseRevolutions:double;
	GantryPattern:integer;
	GantryPatternEnabled:boolean;
	BowlSpinEnabled:boolean;
	
	procedure InitializePump(var Axis:TDistributedT1Axis;XPos:double;YPos:double;Identifier:integer;Speed:double); forward;
	procedure InitializeAuger(var Axis:TDistributedT1Axis;XPos:double;YPos:double;Identifier:integer;Speed:double); forward;
	procedure Dispense; forward;
	procedure UpdateSettings; forward;

	
	
end;

procedure  TContainer.InitializePump(var Axis:TDistributedT1Axis;XPos:double;YPos:double;Identifier:integer;Speed:double);
	begin
		Motor:=Axis;
		XCoord:=XPos;
		YCoord:=YPos;
		ZCoord:=2.6;
		Id:=Identifier;
		IsPump:=true;
		StagingXCoord:=100.0;
		if (Identifier<=22) then
			StagingYCoord:=1549.0
		else if (Identifier=23) or (Identifier=24) then
			StagingYCoord:=1000
		else if (Identifier=25) or (Identifier=26) then
			StagingYCoord:=500.0
		else if (Identifier=27) or (Identifier=28) then
			StagingYcoord:=18.5;
		DispenseSpeed:=Speed;
		ReverseSpeed:=Speed;
		Accel:=10;
		Decel:=10;
		Current:=2.0;
		WeightSP:=0;
		ReverseDelay:=10;
		DispensePermissive:=false;
		ReverseEnabled:=true;
		DispenseComplete:=false;
		Revolutions:=1800;
		ReverseRevolutions:=-6;
		GantryPattern:=1;
		GantryPatternEnabled:=false;
		BowlSpinEnabled:=true;
	end;
	
procedure TContainer.InitializeAuger(var Axis:TDistributedT1Axis;XPos:double;YPos:double;Identifier:integer;Speed:double);
	begin
		Motor:=Axis;
		XCoord:=XPos;
		YCoord:=YPos;
		ZCoord:=8;
		Id:=Identifier;
		IsPump:=false;
		StagingXCoord:=XCoord;
		StagingYCoord:=YCoord;
		DispenseSpeed:=Speed;
		ReverseSpeed:=Speed;
		Accel:=10;
		Decel:=10;
		Current:=2.0;
		WeightSP:=150;
		ReverseDelay:=10;
		DispensePermissive:=false;
		ReverseEnabled:=true;
		DispenseComplete:=false;	
		Revolutions:=18;
		ReverseRevolutions:=-6;
		GantryPattern:=0;
		GantryPatternEnabled:=false;
		BowlSpinEnabled:=true;
		
	end;
 procedure TContainer.Dispense;
	 var ReverseSwitch:boolean;
	 var ReverseSpeed:double;
	begin
		DispensePermissive:=true;
		Motor.SetMotor(On);
		Motor.SetSpeed(DispenseSpeed);
		Motor.SetCurrent(Current);
		Motor.SetAccel(Accel);
		Motor.SetDecel(Decel);
		if IsPump then
			begin
				Motor.BeginMoveBy(Revolutions);
				while DispensePermissive do 
					yield;
				if ReverseEnabled then
					begin
					Motor.SetSpeed(ReverseSpeed);
					Motor.Stop;
					Motor.MoveBy(ReverseRevolutions);
					{Delay(ReverseDelay);}
					end;
				Motor.Stop;
				Motor.SetCurrent(0);
				Motor.SetMotor(Off);
			end
		else
			begin
				ReverseSwitch:=false;
				while DispensePermissive do
				 begin
					if Motor.MoveIsFinished and not ReverseSwitch then
						begin
						Motor.BeginMoveBy(Revolutions);
						ReverseSwitch:=true;
						end
					else if Motor.MoveIsFinished and ReverseSwitch then
						begin
							Motor.BeginMoveBy(ReverseRevolutions);
							ReverseSwitch:=false;
						end;
					yield;
				 end;
				 Motor.Stop;
				 Motor.SetCurrent(0);
				 Motor.SetMotor(Off);
			end;
	end;
	
	procedure TContainer.UpdateSettings; 
		begin
			Motor.SetAccel(Accel);
			Motor.SetDecel(Decel);
		end;

{===============================================================Variable Declarations==================================================================}
var Containers: array[1..28] of TContainer;
var GlobalContainerNo:integer;
var GlobalManualDispenseComplete:boolean;
var GlobalLoadCell:double;
var AutoProcessComplete:boolean;
var AutoDispenseComplete:boolean;
var GlobalStartSignal:boolean;
var LoadCellScaled:double;
var OctoState:integer;
var GantryEnabled:boolean;
var SystemInAuto:boolean;
var SystemReady:boolean;
var ManualMode:boolean;
var SystemStatusEnum:integer;
var GlobalRequestDenester:boolean;
var GlobalSpinRequest:boolean;
var LoadCellFiltered:double;

{==================================================================Single Instance Objects=============================================================================}

var Gantry:object
	XMotor:T1Axis;
	YMotor:T1Axis;
	ZMotor:T1Axis;
	ZEncoder:T1Axis; {This is a virtual axis necessary for Z's encoder}
	XPositionSP:single;
	YPositionSP:single;
	ZPositionSP:single;
	XSpeed:single;
	YSpeed:single;
	ZSpeed:single;
	XAccel:single;
	YAccel:single;
	ZAccel:single;
	XDecel:single;
	YDecel:single;
	ZDecel:single;
	XCurrent:single;
	YCurrent:single;
	ZCurrent:single;
	SafeToMove:boolean;
	AtPump:boolean;
	AtPumpStaging:boolean;
	AtContainers:boolean;
	AtDenesterStaging:boolean;
	AtDenester:boolean;
	AtHome:boolean;
	AtIdlePos:boolean;
	AtOutfeedStaging:boolean;
	CurrentPump:integer;
	PreviousPumpY:double;
	UpdateSettings:boolean;
	Enable:boolean;
	
	procedure Initialize( var XMot:T1Axis; var YMot:T1Axis; var ZMot:T1Axis; var ZEnc:T1Axis);
		begin
			XMotor:=XMot;
			YMotor:=YMot;
			ZMotor:=ZMot;
			ZEncoder:=ZEnc;
			AtPump:=false;
			if (XMot.ActualPosition > 95) and (XMot.ActualPosition < 101) and (YMot.ActualPosition <3) then
				AtHome:=true;
			
		end;
		
	procedure GoToIdlePos;
		begin
		if AtHome then
			begin
				AtHome:=false;
				ZMotor.MoveTo(0);
				YMotor.MoveTo(501);
				Delay(100);
				XMotor.BeginMoveTo(924);
				YMotor.BeginMoveTo(1128);
				while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
					yield;
				AtIdlePos:=true;
			end;
		end;
		
	procedure MoveToStaging(var Container:TContainer);
		begin
			ZMotor.MoveTo(0);
			if AtPump then
				begin
				XMotor.MoveTo(Container.StagingXCoord);
				YMotor.MoveTo(Container.StagingYCoord);
				AtPump:=false;
				end
			else
				begin
				XMotor.BeginMoveTo(Container.StagingXCoord);
				YMotor.BeginMoveTo(Container.StagingYCoord);
				while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
					yield;
				end;
			AtPumpStaging:=true;
		end;

	procedure MoveToPump(var Container:TContainer);
		begin
			if AtPumpStaging and Container.IsPump then
				begin
					XMotor.MoveTo(Container.XCoord);
					AtPump:=true;
					PreviousPumpY:=trunc(Container.YCoord);
					AtPumpStaging:=false;
				end
			else if AtPump and (PreviousPumpY = Container.Ycoord) then
				XMotor.MoveTo(Container.XCoord);
			ZMotor.MoveTo(Container.ZCoord);
		end;
	
		
	
	
	procedure JointAxisMove(var Container:TContainer);
		begin 
			if AtPump then
				begin
					ZMotor.MoveTo(0);
					XMotor.MoveTo(PumpStagingX);
					AtPump:=false;
					PreviousPumpY:=0;
					AtPumpStaging:=true;
				end;
			if AtHome or AtPumpStaging or AtContainers or AtDenesterStaging then
				begin
					AtPumpStaging:=false;
					AtContainers:=false;
					AtDenesterStaging:=false;
					ZMotor.MoveTo(0);
					XMotor.BeginMoveTo(Container.XCoord);
					YMotor.BeginMoveTo(Container.YCoord);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
					ZMotor.MoveTo(9);
				end;
			if (XMotor.ActualPosition <102) and (XMotor.ActualPosition > 95) then
				AtPumpStaging := true
			else if (XMotor.ActualPosition > 101) and (XMotor.ActualPosition < 900) then
				AtContainers := true
			else if (XMotor.ActualPosition > 900) and (round(YMotor.ActualPosition) = 956) then
				AtDenesterStaging := true;
		end;
		
		
	procedure MoveToDenesterStaging;
		begin
			if AtHome or AtDenester or AtIdlePos then
				begin
					AtDenester:=false;
					AtHome:=false;
					AtContainers:=false;
					ZMotor.MoveTo(0);
					XMotor.BeginMoveTo(DenesterStagingX);
					YMotor.BeginMoveTo(DenesterStagingY);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
				end;
				AtDenesterStaging:=true;
				//Strange behavior not mving to denester
		end;
	
	procedure MoveToDenester;
		begin
			if AtDenesterStaging then
				begin
					AtDenesterStaging:=false;
					XMotor.MoveTo(DenesterXPos);
					ZMotor.MoveTo(DenesterZPos);
					AtDenester:=true;
				end;
		end;
		
		
	procedure MoveToOutfeedStaging;
		begin
			if AtPump then
				begin
				ZMotor.MoveTo(0);
				XMotor.MoveTo(PumpStagingX);
				end;
			if AtPumpStaging or AtContainers or AtHome then
				begin
					ZMotor.MoveTo(0);
					AtPumpStaging:=false;
					AtContainers:=false;
					if YMotor.ActualPosition < 500 then
						YMotor.MoveTo(501);
					XMotor.BeginMoveTo(OutfeedStagingX);
					YMotor.BeginMoveTo(OutfeedStagingY);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
					ZMotor.MoveTo(OutfeedStagingZ);
					AtOutfeedStaging:=true;
				end;
		end;
	
	procedure MoveToOutfeed;
		begin
			if AtOutfeedStaging then
				begin
					XMotor.MoveTo(OutfeedXPos);
					YMotor.MoveTo(OutfeedYPos);
					
					XMotor.MoveTo(OutfeedStagingX);
					ZMotor.MoveTo(0);
					YMotor.MoveTo(OutfeedStagingY);
					
					XMotor.MoveTo(800);
				end;
			
		end;
	
	procedure ZigZagPattern(DeltaX:double;DeltaZ:double;Cycles:integer);
		var flipflop:boolean;
		var i:integer;
		begin
			Flipflop:=false;
			for i:=0 to Cycles do
				begin
					if flipflop then
						begin
							XMotor.BeginMoveBy(-DeltaX);
							ZMotor.BeginMoveBy(-DeltaZ);
							FlipFlop=false;
						end
					else 
						begin
							ZMotor.BeginMoveBy(DeltaZ);	
							flipflop:=true;
						end;
					while not (XMotor.MoveIsFinished and ZMotor.MoveIsFinished) do
						yield;		
				end;
		end;
		
	procedure StraightLinePattern(BowlDiameter:integer);
		begin
			XMotor.MoveBy(BowlDiameter*5);
		end;
	
	function GantryReady:boolean;
		begin
			GantryReady:=XMotor.MotorIsOn and YMotor.MotorIsOn and Zmotor.MotorIsOn;
		end;
	
	procedure EventHandler;
    	var EnableMem:boolean;
    	var UpdateSettingsMem:boolean;
		begin
			if Enable and (not EnableMem) then
				begin
					XMotor.SetMotor(on);
					YMotor.SetMotor(on);
					ZMotor.SetMotor(on);
				end;
			EnableMem:=Enable;

			if UpdateSettings and (not UpdateSettingsMem) then
				begin
					XMotor.SetSpeed(XSpeed);
					YMotor.SetSpeed(YSpeed);
					ZMotor.SetSpeed(ZSpeed);
					XMotor.SetAccel(XAccel);
					YMotor.SetAccel(YAccel);
					ZMotor.SetAccel(ZAccel);
					XMotor.SetDecel(XDecel);
					YMotor.SetDecel(YDecel);
					ZMotor.SetDecel(ZDecel);
					{XMotor.SetCurrent(XCurrent);
					YMotor.SetCurrent(YCurrent);
					ZMotor.SetCurrent(ZCurrent);}
				end;
			UpdateSettingsMem:=UpdateSettings;
		end;
end;

{-----------------SystemMode Helper Functions----------------------}
	{We can't use object methods directly when calling a new task so 
	they have to be wrapped in a function that can}
	
procedure DispenseWrapper;
	begin
		Containers[GlobalContainerNo].Dispense;
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
var GlobalContainerNoIndicator:integer;
function UpdateLoadCellScaled(ScalingFactor:double):double;
    begin
        UpdateLoadCellScaled:=LoadCell.Voltage*ScalingFactor;
    end;


var SystemMode:object
	InAuto:boolean;
	InManual:boolean;
	CurrentWeightSP:double;
	DispensePermissive:boolean;
	GantryEnabled:boolean;
	GantryError:boolean;
	ManualDispenseComplete:boolean;
	Container:TContainer;
	procedure ToggleMode; forward;
	function MoveToContainer:boolean; forward;
	procedure SetWeightSP; forward;
	procedure StartGantryPattern; forward;
	procedure AutoDispense; forward;
	procedure ManualDispense; forward;
	procedure StartCycle; forward;
end;
procedure SystemMode.ToggleMode;
	var mode:boolean;
	begin
		if mode then
			begin
			InManual:=true;
			InAuto:=false;
			end
		else
			begin
			InAuto:=true;
			InManual:=false;
			end;
	end;

function SystemMode.MoveToContainer:boolean;
	begin
		if GantryEnabled then
			begin
				if GlobalContainerNo <= 20 then
					Gantry.JointAxisMove(Container)
				else
					begin
						Gantry.MoveToStaging(Container);
						Gantry.MoveToPump(Container);
					end;
			end
		else
			MoveToContainer:=false;
		MoveToContainer:=true;
	end;

procedure SystemMode.SetWeightSP;
	var LoadCellScaled:double;
	begin
		LoadCellScaled := UpdateLoadCellScaled(309);
		CurrentWeightSP:=Containers[GlobalContainerNo].WeightSP+LoadCellScaled;
	end;

procedure SystemMode.StartGantryPattern;
	begin
		if Container.GantryPatternEnabled and GantryEnabled then
			begin
				if Container.GantryPattern = 1 then
					BeginTask(TaskAddr(LinePatternWrapper))
				else if Container.GantryPattern = 2 then
					BeginTask(TaskAddr(LinePatternWrapper));
			end;
	end;

procedure SystemMode.AutoDispense;
	var GantryMoveComplete:boolean;
	var LoadCellScaled:double;
	var LastWeight:double;
	var speedMem:single;
	begin
		Container:=Containers[GlobalContainerNo];
		CurrentWeightSp:=(LoadCell.Voltage*360)+Container.WeightSP;
		{GantryMoveComplete:=MoveToContainer;}
		if Container.IsPump then
			begin
				Gantry.MoveToStaging(Container);
				Gantry.MoveToPump(Container);
			end
		else
			begin
				Gantry.JointAxisMove(Container);
		
			end;
		if true then
			begin
				if((container.GantryPattern = 1) and Gantry.AtPump) then
					begin
						speedMem:=Gantry.XMotor.Speed;
						Gantry.Xmotor.SetSpeed(5);
						Gantry.Xmotor.BeginMoveBy(-50);
						
					end;
					
				if Container.BowlSpinEnabled then
					Spin.SetValue(255);

				BeginTask(TaskAddr(DispenseWrapper));
				LoadCellScaled:=0; {tesing loadcell}
				while LoadCellScaled < CurrentWeightSp do {removed CurrentWeightSP}
					begin
						yield;
						begin
							LoadCellScaled:=(LoadCell.Voltage*360);
						end;
					end;
				GlobalContainerNoIndicator:= GlobalContainerNo;	
				Containers[GlobalContainerNo].DispensePermissive:=false;
			
				Spin.SetValue(0);
				if Gantry.AtPump then
					begin
						Gantry.XMotor.BeginStop;
						Delay(1000);
					end;
					Gantry.XMotor.SetSpeed(speedMem);
				AutoProcessComplete:=true;
				
				{Fix this later, Dispense Permissive should also Stop gantry pattern}
				while TaskPresent(TaskAddr(DispenseWrapper)) do
					yield;
				Gantry.ZMotor.MoveTo(0);
			end
		else
			GantryError:=true;
	end;
		
	procedure SystemMode.ManualDispense;
	var GantryMoveComplete:boolean;
	begin
		GantryMoveComplete:=MoveToContainer;
		if GantryMoveComplete or not GantryEnabled then
			begin
				StartGantryPattern;
				BeginTask(TaskAddr(DispenseWrapper));
				while not GlobalManualDispenseComplete do
						yield;
				Containers[GlobalContainerNo].DispensePermissive:=false;
				{Fix this later, Dispense Permissive should also Stop gantry pattern}
				while (TaskPresent(TaskAddr(DispenseWrapper)) or TaskPresent(TaskAddr(ZigPatternWrapper)) or TaskPresent(TaskAddr(LinePatternWrapper))) do
					yield;
			end
		else
			GantryError:=true;
	end;

procedure SystemMode.StartCycle;
	begin
		if SystemMode.InAuto then
			AutoDispense
		else
			ManualDispense;
	end;

procedure LoadCellFilter;
	var Total:double;
	var i:integer;
	begin
	Total:=0;
	i=0;
	while i < 10 do
		begin
			yield;
			begin
				total := total+LoadCell.Voltage;
				i := i+1;
			end;
		end;
		LoadCellFiltered:=total/i;
	end;


procedure InitializeSubsystems;
	begin
		{LoadCell.Initialize(16,1);}
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

procedure ButtonHandler(Container:integer);
	var DispenseTaskPresent:boolean;
	begin
	DispenseTaskPresent:=TaskPresent(TaskAddr(DispenseWrapper));
	if not DispenseTaskPresent and SystemMode.InManual then
		begin
			if (GlobalContainerNo <= 28) then
				begin
					SystemMode.StartCycle;
				end
			else if (GlobalContainerNo = 29) then
				begin
					Gantry.MoveToDenesterStaging;
					Gantry.MoveToDenester;
					Delay(4000);
					Gantry.ZMotor.MoveTo(0);
					Gantry.MoveToDenesterStaging;
				end
			else if (GlobalContainerNo = 30) then
				begin
					Gantry.MoveToOutfeedStaging;
					Delay(1000);
					Gantry.MoveToOutfeed;
				end
			else if (GlobalContainerNo = 40) then
				begin
					Gantry.GoToIdlePos;
				end;
		end;
	end;




var IngredientQueue:Array [0..12] of integer;
var GlobalAbortSignal:boolean;
var Debug:integer;
var DebugContIn:integer;

procedure AutoStateMachine;
	var AutoState:integer;
	var IngredientIndex:integer;
	var ActiveContainer:TContainer;
	var XMotorSpeedMem:single;
	var StartingLCWeight:double;
	var FinishingWeightSP:double;

	begin
	Debug:=0;
	GlobalAbortSignal:=false;
	GlobalStartSignal:=false;
	IngredientIndex:=0;
	AutoState:=0;
	StartingLCWeight:=0;
	FinishingWeightSP:=0;
	BeginTask(TaskAddr(LoadCellFilter));
		while True do
			begin
				yield;
				begin
					Debug:=AutoState;
					if  GlobalAbortSignal then
						begin
						Gantry.XMotor.BeginStop;
						Gantry.YMotor.BeginStop;
						Gantry.ZMotor.MoveTo(0);
						IngredientIndex:=0;
						GlobalStartSignal:=false;
						GlobalAbortSignal:=false;
						AutoState:=0;
						end;
				{Wait for Start signal from either modbus or S2M}
					if (AutoState = 0)then 
						if GlobalStartSignal then
							begin
								GlobalStartSignal:=false;
								AutoState:=10;
							end;
							
				{Go to denester}
					else if  (AutoState = 10) then
						begin
							SystemMode.GantryEnabled:=true;
							Gantry.MoveToDenesterStaging;
							Gantry.MoveToDenester;
							AutoState:=15;
						end

				{Dispense bowl}
					else if (AutoState = 15) then
						if Gantry.AtDenester then
							begin
								Denester.SetValue(255);
								Delay(3200);
								Denester.SetValue(0);
								Gantry.MoveToDenesterStaging;
								AutoState:=20;
							end;
						
				{Set container}	
					else if (AutoState = 20) then
						begin
							GlobalContainerNo:=IngredientQueue[IngredientIndex];
							if (GlobalContainerNo = 0) then
								AutoState:=60
							else
								AutoState:=30;
						end;
						{Old Code}
						{begin
							if (IngredientQueue[IngredientIndex]<>0) then
								begin
									GlobalContainerNo:=IngredientQueue[IngredientIndex];
									SystemMode.AutoDispense;									
									IngredientIndex:=IngredientIndex+1;
								end
							else
								AutoState:=30;
						end}
					{Go to container}
					else if (AutoState = 30) then
						begin
							ActiveContainer:=Containers[GlobalContainerNo];
							Debug:=ActiveContainer.id;
							if ActiveContainer.IsPump then {Must be higher than staging minus 10}
								begin
									Gantry.MoveToStaging(ActiveContainer);
									Gantry.MoveToPump(ActiveContainer);
								end;
							else
								Gantry.JointAxisMove(ActiveContainer);{this might be root cause of bug}
							AutoState:=40;
						end;
					{Dispense}
					else if (AutoState=40) then
						BeginTask(TaskAddr(DispenseWrapper));

						if ActiveContainer.BowlSpinEnabled then
							Spin.SetValue(255);

						if (ActiveContainer.IsPump and ActiveContainer.GantryPattern = 1) then
							begin
								XMotorSpeedMem:=Gantry.Xmotor.Speed;
								Gantry.XMotor.SetSpeed(10);
								Gantry.XMotor.BeginMoveBy(-50);
							end;
						StartingLCWeight:=LoadCellFiltered;
						FinishingWeightSP:=StartingLCWeight+ActiveContainer.WeightSP;
						while LoadCellFiltered < FinishingWeightSP do
							yield;
						Containers[GlobalContainerNo].DispensePermissive:=false; {we need to use this instead of Active Container so that the same instance stops (not sure how S2M handles it)}
						Gantry.XMotor.BeginStop;
						Spin.SetValue(0);
						while TaskPresent(TaskAddr(DispenseWrapper)) do
							yield;
						AutoState:=50;
					
					else if (AutoState=50) then
						begin
							IngredientIndex:=IngredientIndex+1;
							AutoState=20;
						end;
				{Go to output}
					else if (AutoState = 60) then
						begin
							Gantry.MoveToOutfeedStaging;
							Delay(200);
							{Gantry.MoveToOutfeed;}
							AutoState:=0;
						end
					end;
				end;
			end;
