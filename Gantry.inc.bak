const OutfeedXPos = 1084;
const OutfeedYPos = 890.42;
const OutfeedZPos = 5.76;
const PumpStagingX = 100.0;
const DenesterStagingX = 915.0;
const DenesterStagingY = 956.69;
const DenesterStagingZ = 5.76;
const OutfeedStagingX = 915.0;
const OutfeedStagingY = 956.69;
const OutfeedStagingZ = 5.76;
	
Type TGantry:=object
	XMotor:T1Axis;
	YMotor:T1Axis;
	ZMotor:T1Axis;
	ZEncoder:T1Axis; {This is a virtual axis necessary for Z's encoder}
	GantrySafeToMove:boolean;
	GantryPermissive:boolean;
	CurrentZone:integer;{0 - Home, 1 - Pumps, 2 - PumpStaging, 3 - Containers, 4 - Outfeed, Denester Staging, 5 - Denester, Outfeed Zones, 99 - Unknown}
	GantryError:integer; {1 - impossible move to pump, 2- Impossible Move To containers}
	DenesterXPos:double;
	DenesterYPos:double;
	DenesterZPos:double;
	CurrentPump:integer;
	AtPump:boolean;
	AtPumpStaging:boolean;
	AtContainers:boolean;
	AtDenesterStaging:boolean;
	AtHome:boolean;
	AtOutfeedStaging:boolean;
	

	procedure Initialize( var XMot:T1Axis; var YMot:T1Axis; var ZMot:T1Axis; var ZEnc:T1Axis);
		begin
			XMotor:=XMot;
			YMotor:=YMot;
			ZMotor:=ZMot;
			ZEncoder:=ZEnc;
		end;
		

	procedure MoveToPumpStaging(var Pump:TPump);
		begin
			if AtDenesterStaging or AtContainers then
				begin
					AtContainers:=false;
					AtDenesterStaging:=false;
					ZMotor.MoveTo(0);
					XMotor.BeginMoveTo(Pump.StagingXCoord);
					YMotor.BeginMoveTo(Pump.StagingYCoord);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
				end
			else if AtPump then
				begin
					AtPump:=false;
					XMotor.MoveTo(PumpStagingX);
					YMotor.MoveTo(Pump.YCoord);
				end;
			AtPumpStaging:=true;
		end;
		
	procedure MoveToPump(var Pump:TPump);
		begin
			MoveToPumpStaging(Pump);
			if AtPumpStaging then
			begin
				XMotor.MoveTo(Pump.XCoord);
				ZMotor.MoveTo(Pump.ZCoord);
				AtPump:=true;
				CurrentPump:=Pump.Id;
				AtPumpStaging:=false;
			end;
		end;
	
		
	
	
	procedure MoveToContainer(var Container:TAugerContainer);
		begin 
			if AtPumpStaging or AtContainers or AtDenesterStaging then
				begin
					AtPumpStaging:=false;
					AtContainers:=false;
					AtDenesterStaging:=false;
					ZMotor.MoveTo(0);
					XMotor.BeginMoveTo(Container.XCoord);
					YMotor.BeginMoveTo(Container.YCoord);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
				end;
		end;
		
	procedure MoveToDenesterStaging;
		begin
			if AtHome then
				begin
					AtHome:=false;
					ZMotor.MoveTo(0);
					XMotor.BeginMoveTo(DenesterStagingX);
					YMotor.BeginMoveTo(DenesterStagingY);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
					ZMotor.MoveTo(DenesterStagingZ);
					AtDenesterStaging:=true;
				end
		end;
	
	procedure MoveToDenester;
		begin
			if AtDenesterStaging then
				begin
					AtDenesterStaging:=false;
					XMotor.BeginMoveTo(DenesterXPos);
					YMotor.BeginMoveTo(DenesterYPos);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
					ZMotor.MoveTo(DenesterZPos);
				end;
		end;
		
		
	procedure MoveToOutfeedStaging;
		begin
			if AtPumpStaging or AtContainers then
				begin
					AtPumpStaging:=false;
					AtContainers:=false;
					XMotor.BeginMoveTo(OutfeedStagingX);
					YMotor.BeginMoveTo(OutfeedStagingZ);
					while not (XMotor.MoveIsFinished and YMotor.MoveIsFinished) do
						yield;
					ZMotor.MoveTo(OutfeedStagingZ);
				end;
		end;
	
	procedure MoveToOutfeed;
		begin
			if AtOutfeedStaging then
				begin
					XMotor.MoveTo(OutfeedXPos);
					YMotor.MoveTo(OutfeedYPos);
					XMotor.MoveTo(OutfeedStagingX);
					YMotor.MoveTo(OutfeedStagingY);
					ZMotor.MoveTo(OutfeedStagingZ);
				end;
			
		end;
	
	procedure ZigZagPattern(DeltaX:double;DeltaZ:double;Cycles:integer);
		var i:integer;
		begin
			for i:=0 to Cycles do
				begin
					if ((i and $01) = 1) then
						begin
						XMotor.BeginMoveBy(-DeltaX);
						ZMotor.BeginMoveBy(-DeltaZ);
						while not (XMotor.MoveIsFinished and ZMotor.MoveIsFinished) do
							yield;
						end
					else 
						ZMotor.BeginMoveBy(DeltaZ);	
						while not (XMotor.MoveIsFinished and ZMotor.MoveIsFinished) do
							yield;
						end
				end;
				

		end;
	
end;