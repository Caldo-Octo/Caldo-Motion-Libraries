{-------------------------------Generic Container---------------------------}
type TContainer=object
	Motor:TDistributedT1Axis;
	Id:integer;
	XCoord:double;
	YCoord:double;
	ZCoord:double;
	IsPump:boolean;
	StagingXCoord:double;
	StagingYCoord:double;
	DispenseSpeed:double;
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
	
end;

procedure  TContainer.InitializePump(var Axis:TDistributedT1Axis;Identifier:integer;XPos:double;YPos:double;Speed:double);
	begin
		Motor:=Axis;
		XCoord:=XPos;
		YCoord:=YPos;
		ZCoord:=5;
		Id:=Identifier;
		IsPump:=true;
		StagingXCoord:=100.0;
		if (Identifier<=2) then
			StagingYCoord:=1464.5
		else if (Identifier=3) or (Identifier=4) then
			StagingYCoord:=982.0
		else if (Identifier=5) or (Identifier=6) then
			StagingYCoord:=500.0
		else if (Identifier=7) or (Identifier=8) then
			StagingYcoord:=18.5;
		DispenseSpeed:=Speed;
		Current:=2.0;
		WeightSP:=0;
		ReverseDelay:=10;
		DispensePermissive:=false;
		ReverseEnabled:=true;
		DispenseComplete:=false;
		Revolutions:=180;
		ReverseRevolutions:=36;
		GantryPattern:=0;
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
		Current:=2.0;
		WeightSP:=0;
		ReverseDelay:=10;
		DispensePermissive:=false;
		ReverseEnabled:=true;
		DispenseComplete:=false;	
		Revolutions:=18;
		ReverseRevolutions:=6;
		GantryPattern:=0;
		GantryPatternEnabled:=false;
		BowlSpinEnabled:=true;
		
	end;
 procedure TContainer.Dispense;
	 var ReverseSwitch:boolean;
	 var ReverseSpeed:double;
	begin
		ReverseSpeed:= (0-DispenseSpeed);
		DispensePermissive:=true;
		Motor.SetMotor(On);
		Motor.SetCurrent(Current);
		if IsPump then
			begin
				Motor.BeginMoveBy(1000);
				while DispensePermissive do 
					yield;
				if ReverseEnabled then
					begin
					Motor.Stop;
					Motor.MoveTo(-20);
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
	
	