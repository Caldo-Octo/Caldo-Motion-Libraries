var ManualDispenseComplete:boolean;
var Augers: Array[1..20] of TContainer;
var Pumps: Array[1..8] of TContainer;
var Gantry:TGantry;
var GantryEnabled:boolean;
var CurrentContainerNo:integer;
var CurrentContainerType:integer; {1 Auger, 2 Pump}

const Col1Coord = 133.0;
const Col2Coord = 288.1;
const Col3Coord = 443.0;
const Col4Coord = 597.9;
const Col5Coord = 771.0;
const Row1Coord = 1464.5;
const Row2Coord = 979.2;
const Row3Coord = 500.0;
const Row4Coord = 18.5;
const AugDefaultSpeed = 20.0;
const LPumpDefaultX = -75.0;
const RPumpDefaultX = -55.0;

const PumpsDefaultSpeed = 40;

procedure InitializeActuators;
	begin
		{We need to change the Remote_Motor_Names eventually}
		Augers[1].InitializeAuger(Remote_Motor_27,Col1Coord,Row1Coord,1,AugDefaultSpeed);
		Augers[2].InitializeAuger(Remote_Motor_28,Col2Coord,Row1Coord,2,AugDefaultSpeed);
		Augers[3].InitializeAuger(Remote_Motor_25,Col3Coord,Row1Coord,3,AugDefaultSpeed);
		Augers[4].InitializeAuger(Remote_Motor_26,Col4Coord,Row1Coord,4,AugDefaultSpeed);
		Augers[5].InitializeAuger(Remote_Motor_23,Col5Coord,Row1Coord,5,AugDefaultSpeed);
		Augers[6].InitializeAuger(Remote_Motor_24,Col1Coord,Row2Coord,6,AugDefaultSpeed);
		Augers[7].InitializeAuger(Remote_Motor_21,Col2Coord,Row2Coord,7,AugDefaultSpeed);
		Augers[8].InitializeAuger(Remote_Motor_22,Col3Coord,Row2Coord,8,AugDefaultSpeed);
		Augers[9].InitializeAuger(Remote_Motor_19,Col4Coord,Row2Coord,9,AugDefaultSpeed);
		Augers[10].InitializeAuger(Remote_Motor_20,Col5Coord,Row2Coord,10,AugDefaultSpeed);
		Augers[11].InitializeAuger(Remote_Motor_21,Col1Coord,Row3Coord,11,AugDefaultSpeed);
		Augers[12].InitializeAuger(Remote_Motor_18,Col2Coord,Row3Coord,12,AugDefaultSpeed);
		Augers[13].InitializeAuger(Remote_Motor_15,Col3Coord,Row3Coord,13,AugDefaultSpeed);
		Augers[14].InitializeAuger(Remote_Motor_16,Col4Coord,Row3Coord,14,AugDefaultSpeed);
		Augers[15].InitializeAuger(Remote_Motor_13,Col5Coord,Row3Coord,15,AugDefaultSpeed);
		Augers[16].InitializeAuger(Remote_Motor_10,Col1Coord,Row4Coord,16,AugDefaultSpeed);
		Augers[17].InitializeAuger(Remote_Motor_9,Col2Coord,Row4Coord,17,AugDefaultSpeed);
		Augers[18].InitializeAuger(Remote_Motor_12,Col3Coord,Row4Coord,18,AugDefaultSpeed);
		Augers[19].InitializeAuger(Remote_Motor_11,Col4Coord,Row4Coord,19,AugDefaultSpeed);
		Augers[20].InitializeAuger(Remote_Motor_14,Col5Coord,Row4Coord,20,AugDefaultSpeed);
		
		Pumps[1].InitalizePump(Remote_Motor_8,LPumpDefaultX,Row1Coord,21,PumpsDefaultSpeed);
		Pumps[2].InitalizePump(Remote_Motor_7,RPumpDefaultX,Row1Coord,22,PumpsDefaultSpeed);
		Pumps[3].InitalizePump(Remote_Motor_6,LPumpDefaultX,Row1Coord,23,PumpsDefaultSpeed);
		Pumps[4].InitalizePump(Remote_Motor_5,RPumpDefaultX,Row1Coord,24,PumpsDefaultSpeed);
		Pumps[5].InitalizePump(Remote_Motor_4,LPumpDefaultX,Row1Coord,25,PumpsDefaultSpeed);
		Pumps[6].InitalizePump(Remote_Motor_3,RPumpDefaultX,Row1Coord,26,PumpsDefaultSpeed);
		Pumps[7].InitalizePump(Remote_Motor_2,LPumpDefaultX,Row1Coord,27,PumpsDefaultSpeed);
		Pumps[8].InitalizePump(Augar1,RPumpDefaultX,Row1Coord,28,PumpsDefaultSpeed);
	end;
	
	
	procedure ManualDispense;
		begin
		end;