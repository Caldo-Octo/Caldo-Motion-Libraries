var Containers: array[1..28] of TContainer;
var GlobalContainerNo:integer;
{var LoadCell:TAnalogInput;}
var GlobalManualDispenseComplete:boolean;
{-----------------Task Wrapper Functions----------------------}
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
	procedure SetContainer; forward;
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
procedure SystemMode.SetContainer;
	begin
		Container:=Containers[GlobalContainerNo];
	end;

function SystemMode.MoveToContainer:boolean;
	begin
		if Gantry.GantryReady and GantryEnabled then
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
		LoadCellScaled := UpdateLoadCellScaled(1);
		CurrentWeightSP:=Container.WeightSP+LoadCellScaled;
	end;

procedure SystemMode.StartGantryPattern;
	begin
		if Container.GantryPatternEnabled and GantryEnabled then
			begin
				if Container.GantryPattern = 1 then
					BeginTask(TaskAddr(ZigPatternWrapper))
				else if Container.GantryPattern = 2 then
					BeginTask(TaskAddr(LinePatternWrapper));
			end;
	end;

procedure SystemMode.AutoDispense;
	var GantryMoveComplete:boolean;
	var LoadCellScaled:double;
	begin
		SetWeightSP;
		GantryMoveComplete:=MoveToContainer;
		if GantryMoveComplete then
			begin
				StartGantryPattern;
				BeginTask(TaskAddr(DispenseWrapper));
				LoadCellScaled:=0;
				while (LoadCellScaled<CurrentWeightSP) do
					begin
						yield;
						begin
						LoadCellScaled:=UpdateLoadCellScaled(1);
						end;
					end;
				Container.DispensePermissive:=false;
				{Fix this later, Dispense Permissive should also Stop gantry pattern}
				while (TaskPresent(TaskAddr(DispenseWrapper)) or TaskPresent(TaskAddr(ZigPatternWrapper)) or TaskPresent(TaskAddr(LinePatternWrapper))) do
					yield;
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
		SetContainer;
		if SystemMode.InAuto then
			AutoDispense
		else
			ManualDispense;
	end;


