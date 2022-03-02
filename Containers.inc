
var ManualDispenseComplete:boolean;
var CurrentPump:TPump;
var CurrentAuger:TAugerContainer;


{------------------------------Pump typedef---------------------------------}
type TPump=object
	Pump:TDistributedT1Axis;
	Id:integer;
	ContType:integer;
	XCoord:double;
	YCoord:double;
	Zcoord:double;
	DispenseSpeed:double;
	SpinEnabled:boolean;
	WeightSP:double;
	DispenseEnabled:boolean;
	GantryPatternEnabled:boolean;
	ReverseEnabled:boolean;
	StagingXCoord:double;
	StagingYCoord:double;
	DripDelay:double;
	Current:double;
	
	procedure Initialize(var Motor:TDistributedT1Axis;XPos:double;Ypos:double;ZPos:double);
		begin
			Pump:=Motor;
			ContType:=1;
			XCoord:=XPos;
			YCoord:=YPos;
			ZCoord:=Zpos;
			DispenseSpeed:=60;
			WeightSP:=5;{in grams}
			SpinEnabled:=false;
			DispenseEnabled:=false;
			ReverseEnabled:=true;
			DispenseDelay:=500;
			Current:=2
		end;
		
	procedure Dispense;
		begin
			Pump.SetMotor(On);
			Pump.SetCurrentTo(Current);
			Pump.Jog(DispenseSpeed);
			DispenseEnabled:=true;
			while DispenseEnabled do
				yield;
			if ReverseEnabled then
				begin
					Pump.Jog(-DispenseSpeed);
				end
			Delay(DispenseDelay);
			Pump.BeginStop;
			Pump.SetMotor(Off);
		end;
	end;

{------------------------------Auger typedef---------------------------------}
	
type TAugerContainer=object
	Auger:TDistributedT1Axis;
	ContType:integer;
	XCoord:double;
	YCoord:double;
	ZCoord:double;
	Revolutions:double;
	ReverseRevolutions:double;
	DispenseSpeed:double;
	SpinEnabled:boolean;
	ZigZagEnabled:boolean;
	WeightSP:double;
	DispenseEnabled:true;
	
	procedure Initialize(Motor:TDistributedT1Axis;XPos:double;YPos:double;ZPos:double);
		begin
			Auger:=Motor;
			ContType:=2;
			XCoord:=XPos;
			YCoord:=YPos;
			ZCoord:=ZPos;
			DispenseSpeed:=2;
			Revolutions:=18;
			ReverseRevolutions:=6;
			SpinEnabled:=false;
			ZigZagEnabled:=false;
			WeightSP:=5;{in grams}
			DispenseEnabled:=false;
			Current:single;
		end;

	procedure Dispense;
		var Reverse:boolean;
		begin
			Auger.SetCurrentTo(Current);
			Auger.SetMotor(on);
			Reverse:=false;
			DispenseEnabled:=true;
			while DispenseEnabled do
				begin
				if Auger.MoveIsFinished and not Reverse then
					begin
					Auger.BeginMoveBy(Revolutions);
					Reverse:=true;
					end
				else if Auger.MoveIsFinished and Reverse then
					begin
					Auger.BeginMoveBy(ReverseRevolutions);
					Reverse:=false;
					end;
				yield;
				end;
		end;
	end;
		
	procedure PumpDispenseWrapper;
	{This allows us to call a method from a pump object in BeginTask}		
		begin
			CurrentPump.Dispense;
		end;
	
	procedure AugerDispenseWrapper;
		begin
			CurrentAuger.Dispense;
		end;