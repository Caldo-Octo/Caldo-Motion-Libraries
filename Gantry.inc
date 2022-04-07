const OutfeedXPos = 1084;
const OutfeedYPos = 890.42;
const OutfeedZPos = 3.6;
const PumpStagingX = 100.0;
const DenesterStagingX = 800;
const DenesterStagingY = 1128;
const DenesterStagingZ = 0;
const OutfeedStagingX = 924;
const OutfeedStagingY = 942;
const OutfeedStagingZ = 3.6;
const DenesterXPos = 1100;
const DenesterYPos = 1128;
const DenesterZPos = 7;


Type TGantry=object
	XMotor:T1Axis;
	YMotor:T1Axis;
	ZMotor:T1Axis;
	ZEncoder:T1Axis; {This is a virtual axis necessary for Z's encoder}
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
	


	procedure Initialize( var XMot:T1Axis; var YMot:T1Axis; var ZMot:T1Axis; var ZEnc:T1Axis);
		begin
			XMotor:=XMot;
			YMotor:=YMot;
			ZMotor:=ZMot;
			ZEncoder:=ZEnc;
			AtPump:=false;
			if (XMot.ActualPosition > 99) and (XMot.ActualPosition < 101) and (YMot.ActualPosition <3) then
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
			if (XMotor.ActualPosition <101) and (XMotor.ActualPosition > 99) then
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
			XMotor.MoveBy(BowlDiameter);
		end;
