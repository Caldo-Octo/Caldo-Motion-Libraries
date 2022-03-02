var Augers: array[1..20] of TAugerContainer;
var Pumps: array[1..8] of TPump;
var Gantry:TGantry;
var Heaters: array[1..10] of TTempController;
var Thermocouples: array[1..10] of TModbusWord;
var Relays: array[1..10] of boolean;


procedure InitializeHeaters;
	var Heater:integer;
	begin
		for Heater:=1 to 10 do
			Heaters[Heater].Initialize;
	end;

procedure InitializeGantry;
  begin
	Gantry.Initialize(X_Motor,Y_Motor,Stepper,StepperEncoder);
  end;
  
procedure InitializeContainers;
  begin
	  Augers[1].Initialize(Remote_Motor_27,131,1464.5,8.5);
	  Augers[2].Initialize(Remote_Motor_28,288,1464.5,8.5);
	  Augers[3].Initialize(Remote_Motor_25,443,1464.5,8.5);
	  Augers[4].Initialize(Remote_Motor_26,597,1464.5,8.5);
	  Augers[5].Initialize(Remote_Motor_23,753,1464.5,8.5);
	  Augers[6].Initialize(Remote_Motor_24,131,982,8.5);
	  Augers[7].Initialize(Remote_Motor_21,288,982,8.5);
	  Augers[8].Initialize(Remote_Motor_22,443,982,8.5);
	  Augers[9].Initialize(Remote_Motor_19,597,982,8.5);
	  Augers[10].Initialize(Remote_Motor_20,753,982,8.5);
	  Augers[11].Initialize(Remote_Motor_17,131,500,8.5);
	  Augers[12].Initialize(Remote_Motor_18,288,500,8.5);
	  Augers[13].Initialize(Remote_Motor_15,443,500,8.5);
	  Augers[14].Initialize(Remote_Motor_16,597,500,8.5);
	  Augers[15].Initialize(Remote_Motor_13,753,500,8.5);
	  Augers[16].Initialize(Remote_Motor_10,131,18,8.5);
	  Augers[17].Initialize(Remote_Motor_9,288,18,8.5);
	  Augers[18].Initialize(Remote_Motor_12,443,18,8.5);
	  Augers[19].Initialize(Remote_Motor_11,597,18,8.5);
	  Augers[20].Initialize(Remote_Motor_14,753,18,8.5);
  end;
  
  procedure InitializePumps;
	  begin
		  Pumps[1].Initialize(Remote_Motor_8,-15,1464,2);
		  Pumps[2].Initialize(Remote_Motor_7,-50,1464,2);
		  Pumps[3].Initialize(Remote_Motor_6,-50,982,2);
		  Pumps[4].Initialize(Remote_Motor_5,-15,982,2);
		  Pumps[5].Initialize(Remote_Motor_4,-50,500,2);
		  Pumps[6].Initialize(Remote_Motor_3,-15,500,2);
		  Pumps[7].Initialize(Remote_Motor_2,-50,18,2);
		  Pumps[8].Initialize(Augar1,-15,18,2);
	  end;
  
  procedure InitializeSystems;
	  begin
		InitializeGantry;
		InitializeContainers;
		InitializePumps;
		InitializeHeaters;
	  end;