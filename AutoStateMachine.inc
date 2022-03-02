
var MoveRequest:bool;
var ContainerType:integer; 
var CurrentPump:TPump;
var CurrentAuger:TAugerContainer;
var MBCurrentContainer:integer;
var LoadCell:double;

procedure AutoDispense;
{States 0 - waiting for init}
	var CurrentWeight:double;
	begin
	{Octo ready for Cyle}
		
	CurrentWeight:=LoadCell;
	if ContainerType = 1 then
		begin
			CurrentPump:=Pumps[MBCurrentContainer];
			Gantry.MoveToPump[CurrentPump];
			BeginTask(PumpDispenseWrapper);
			if CurrentPump.
			while LoadCell < (CurrentWeight+Pump.WeightSP) do
				yield;
			{Fix pattern logic}
			CurrentPump.DispenseEnable:=false;
			while TaskPresent(TaskAddr(PumpDispenseWrapper)) do
				yield;
		end
	else if ContainerType = 2 then
		begin
			CurrentAuger:=Augers[MBCurrentContainer];
			Gantry.MoveToContainer[CurrentAuger];
			BeginTask(AugerDispenseWrapper);
			while LoadCell < (CurrentWeight+Pump.WeightSP) do
				yield;
			CurrentAuger.DispenseEnable:=false;
			while TaskPresent(TaskAddr(AugerDispenseWrapper)) do
				yield;
		end
	end;