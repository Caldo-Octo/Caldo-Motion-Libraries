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
	procedure UpdateSpeed(Speed:single); forward;
	procedure UpdateAccel(Accel_:single); forward;
	procedure UpdateDecel(Decel_:single); forward;
	procedure UpdateWeightSP(Weight:double); forward;
	procedure UpdateRevolutions(Revs:double); forward;
	
	
end;

procedure  TContainer.InitializePump(var Axis:TDistributedT1Axis;XPos:double;YPos:double;Identifier:integer;Speed:double);
	begin
		Motor:=Axis;
		XCoord:=XPos;
		YCoord:=YPos;
		ZCoord:=3;
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
		Revolutions:=180;
		ReverseRevolutions:=-6;
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
		ReverseSpeed:=Speed;
		Accel:=10;
		Decel:=10;
		Current:=2.0;
		WeightSP:=0;
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
	
	procedure TContainer.UpdateSpeed(Speed:single); 
		begin
			DispenseSpeed:=Speed;
		end;

	procedure TContainer.UpdateAccel(Accel_:single);
		begin
			Accel:=Accel_;
		end;

	procedure TContainer.UpdateDecel(Decel_:single);
		begin
			Decel:=Decel_;
		end;
	procedure TContainer.UpdateWeightSP(Weight:double);
		begin
			WeightSP:=Weight;
		end;
	procedure TContainer.UpdateRevolutions(Revs:double, ReverseRevs:double);
		begin
			Revolutions:=Revs;
			ReverseRevolutions:=ReverseRevs;
		end;
	
