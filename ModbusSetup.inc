const UnusedModuleNo = $10;
{Modbus Constants - Words}
const SystemStatus     = $21;
const SystemCommand    = $22;
const SystemState     = $23;
const _UserWord4_w     = $24;
const _UserWord5_w     = $25;
const _UserWord6_w     = $26;
const _UserWord7_w     = $27;
const _UserWord8_w     = $28;
const _UserWord9_w     = $29;
const GantryStatus    = $2A;
const GantryCommand    = $2B;
const _UserWord12_w    = $2C;
const _UserWord13_w    = $2D;
const _UserWord14_w    = $2E;
const _UserWord15_w    = $2F;
const _UserWord16_w    = $30;
const _UserWord17_w    = $31;
const _UserWord18_w    = $32;
const _UserWord19_w    = $33;
const ContainerStatus    = $34;
const ContainerCommand   = $35;
const CurrentContainer    = $36;
const DispenseRevolutions   = $37;
const ReverseRevolutions    = $38;
const GantryPattern    = $39;
const _UserWord26_w    = $3A;
const _UserWord27_w    = $3B;
const _UserWord28_w    = $3C;
const _UserWord29_w    = $3D;
const _UserWord30_w    = $3E;
const _UserWord31_w    = $3F;
const _UserWord32_w    = $40;
const _UserWordLast_w  = $40;
{Modbus Constants - Singles}
const _UserSingle1_s   = $41;
const _UserSingle2_s   = $43;
const _UserSingle3_s   = $45;
const _UserSingle4_s   = $47;
const _UserSingle5_s   = $49;
const XSpeed   = $4B;
const YSpeed   = $4D;
const ZSpeed   = $4F;
const XAccel   = $51;
const YAccel  = $53;
const ZAccel  = $55;
const XDecel  = $57;
const YDecel  = $59;
const ZDecel  = $5B;
const XCurrent  = $5D;
const YCurrent  = $5F;
const ZCurrent  = $61;
const _UserSingle18_s  = $63;
const _UserSingle19_s  = $65;
const DispenseSpeed  = $67;
const ReverseSpeed  = $69;
const Accel  = $6B;
const Decel  = $6D;
const Current  = $6F;
const _UserSingle25_s  = $71;
const _UserSingle26_s  = $73;
const _UserSingle27_s  = $75;
const _UserSingle28_s  = $77;
const _UserSingle29_s  = $79;
const _UserSingle30_s  = $7B;
const _UserSingle31_s  = $7D;
const _UserSingle32_s  = $7F;
const _UserSingleLast_s= $7F;

{Modbus Constants - Doubles}
const LoadCellConst   = $81;
const _UserDouble2_d   = $85;
const _UserDouble3_d   = $89;
const _UserDouble4_d   = $8D;
const _UserDouble5_d   = $91;
const _UserDouble6_d   = $95;
const _UserDouble7_d   = $99;
const _UserDouble8_d   = $9D;
const _UserDouble9_d   = $A1;
const XPositionSP  = $A5;
const YPositionSP  = $A9;
const ZPositionSP  = $AD;
const XActualPosition  = $B1;
const YActualPosition  = $B5;
const ZActualPosition  = $B9;
const _UserDouble16_d  = $BD;
const _UserDouble17_d  = $C1;
const _UserDouble18_d  = $C5;
const _UserDouble19_d  = $C9;
const DispenseWeight  = $CD;
const XCoord  = $D1;
const YCoord  = $D5;
const ZCoord  = $D9;
const StagingX  = $DD;
const StagingY  = $E1;
const _UserDouble26_d  = $E5;
const _UserDouble27_d  = $E9;
const _UserDouble28_d  = $ED;
const _UserDouble29_d  = $F1;
const _UserDouble30_d  = $F5;
const _UserDouble31_d  = $F9;
const _UserDouble32_d  = $FD;
const _UserDoubleLast_d= $FD;

{System Communications}
var mb_SysStatus:TModbusWord;
var mb_SysCommand:TModbusWord;
var mb_LoadCell:TModbusSingle;
{Gantry Communications}
var mb_GantryStatus:TModbusWord;
var mb_GantryCommand:TModbusWord;
var mb_XSpeed:TModbusSingle;
var mb_YSpeed:TModbusSingle;
var mb_ZSpeed:TModbusSingle;
var mb_XAccel:TModbusSingle;
var mb_YAccel:TModbusSingle;
var mb_ZAccel:TModbusSingle;
var mb_XDecel:TModbusSingle;
var mb_YDecel:TModbusSingle;
var mb_ZDecel:TModbusSingle;
var mb_XCurrent:TModbusSingle;
var mb_YCurrent:TModbusSingle;
var mb_ZCurrent:TModbusSingle;
var mb_XPositionSP:TModbusSingle;
var mb_YPositionSP:TModbusSingle;
var mb_ZPositionSP:TModbusSingle;
var mb_XActualPosition:TModbusSingle;
var mb_YActualPosition:TModbusSingle;
var mb_ZActualPosition:TModbusSingle;
{Container Communication}
var mb_ContainerStatus:TModbusWord;
var mb_ContainerCommand:TModbusWord;
var mb_CurrentContainer:TModbusWord;
var mb_DispenseRevolutions:TModbusWord;
var mb_ReverseRevolutions:TModbusWord;
var mb_GantryPatern:TModbusWord;
var mb_DispenseSpeed:TModbusSingle;
var mb_ReverseSpeed:TModbusSingle;
var mb_Accel:TModbusSingle;
var mb_Decel:TModbusSingle;
var mb_Current:TModbusSingle;
var mb_DispenseWeight:TModbusSingle;
var mb_ContainerXCoord:TModbusSingle;
var mb_ContainerYCoord:TModbusSingle;
var mb_ContainerZCoord:TModbusSingle;
var mb_ContainerStagingX:TModbusSingle;
var mb_ContainerStagingY:TModbusSingle;



procedure SystemModbusCommsInit;
    begin
        mb_SysStatus.Initialize(UnusedModuleNo, SystemStatus, 0);
        mb_SysCommand.Initialize(UnusedModuleNo, SystemCommand, 0);
        mb_LoadCell.Initialize(UnusedModuleNo, LoadCellConst, 0);
        yield;
    end;

procedure GantryModbusCommsInit;
    begin
        mb_GantryStatus.Initialize(UnusedModuleNo, GantryStatus, 0);
        mb_GantryCommand.Initialize(UnusedModuleNo, GantryCommand, 0);
        yield;
        mb_XSpeed.Initialize(UnusedModuleNo, XSpeed, 0);
        mb_YSpeed.Initialize(UnusedModuleNo, YSpeed, 0);
        mb_ZSpeed.Initialize(UnusedModuleNo, ZSpeed, 0);
        mb_XAccel.Initialize(UnusedModuleNo, XAccel, 0);
        mb_YAccel.Initialize(UnusedModuleNo, YAccel, 0);
        mb_ZAccel.Initialize(UnusedModuleNo, ZAccel, 0);
        mb_XDecel.Initialize(UnusedModuleNo, XDecel, 0);
        mb_YDecel.Initialize(UnusedModuleNo, YDecel, 0);
        mb_ZDecel.Initialize(UnusedModuleNo, ZDecel, 0);
        mb_XCurrent.Initialize(UnusedModuleNo, XCurrent, 0);
        mb_YCurrent.Initialize(UnusedModuleNo, YCurrent, 0);
        mb_ZCurrent.Initialize(UnusedModuleNo, ZCurrent, 0);
        yield; 
        mb_XPositionSP.Initialize(UnusedModuleNo, XPositionSP, 0);
        mb_YPositionSP.Initialize(UnusedModuleNo, YPositionSP, 0);
        mb_ZPositionSP.Initialize(UnusedModuleNo, ZPositionSP, 0);
        mb_XActualPosition.Initialize(UnusedModuleNo, XActualPosition, 0);
        mb_YActualPosition.Initialize(UnusedModuleNo, YActualPosition, 0);
        mb_ZActualPosition.Initialize(UnusedModuleNo, ZActualPosition, 0);
        end;

procedure ContainerModbusCommsInit;
    begin
        mb_ContainerStatus.Initialize(UnusedModuleNo, ContainerStatus, 0);
        mb_ContainerCommand.Initialize(UnusedModuleNo, ContainerCommand, 0);
        mb_CurrentContainer.Initialize(UnusedModuleNo, CurrentContainer, 0);
        mb_DispenseRevolutions.Initialize(UnusedModuleNo, DispenseRevolutions, 0);
        mb_ReverseRevolutions.Initialize(UnusedModuleNo, ReverseRevolutions, 0);
        mb_GantryPatern.Initialize(UnusedModuleNo, GantryPattern, 0);
        yield;
        mb_DispenseSpeed.Initialize(UnusedModuleNo, DispenseSpeed, 0);
        mb_ReverseSpeed.Initialize(UnusedModuleNo, ReverseSpeed, 0);
        mb_Accel.Initialize(UnusedModuleNo, Accel, 0);
        mb_Decel.Initialize(UnusedModuleNo, Decel, 0);
        mb_Current.Initialize(UnusedModuleNo, Current, 0);
        yield;
        mb_DispenseWeight.Initialize(UnusedModuleNo, DispenseWeight, 0);
        mb_ContainerXCoord.Initialize(UnusedModuleNo, XCoord, 0);
        mb_ContainerYCoord.Initialize(UnusedModuleNo, YCoord, 0);
        mb_ContainerZCoord.Initialize(UnusedModuleNo, ZCoord, 0);
        mb_ContainerStagingX.Initialize(UnusedModuleNo, StagingX, 0);
        mb_ContainerStagingY.Initialize(UnusedModuleNo, StagingY, 0);
    end;
    
procedure MapModbusMMCInputs;
    var Container:TContainer;
    begin
        {Boolean variables are encoded in modbus words}
        {System Modbus Words}
        {GlobalStartSignal:=mb_SysCommand.Value and $00A;}
        GlobalContainerNo:=mb_CurrentContainer.Value;
        {Gantry Modbus Words}
        {TODO: Add Gantry command masks}
        {Gantry.Enable:=mb_GantryCommand.Value and $0A;}
        {Gantry.UpdateSettings:=mb_GantryCommand.Value and $0C;}
        Gantry.XSpeed:=mb_XSpeed.Value;
        Gantry.YSpeed:=mb_YSpeed.Value;
        Gantry.ZSpeed:=mb_ZSpeed.Value;
        Gantry.XAccel:=mb_XAccel.Value;
        Gantry.YAccel:=mb_YAccel.Value;
        Gantry.ZAccel:=mb_ZAccel.Value;
        Gantry.XDecel:=mb_XDecel.Value;
        Gantry.YDecel:=mb_YDecel.Value;
        Gantry.ZDecel:=mb_ZDecel.Value;
        Gantry.XCurrent:=mb_XCurrent.Value;
        Gantry.YCurrent:=mb_YCurrent.Value;
        Gantry.ZCurrent:=mb_ZCurrent.Value;
        Gantry.XPositionSP:=mb_XPositionSP.Value;
        Gantry.YPositionSP:=mb_YPositionSP.Value;
        Gantry.ZPositionSP:=mb_ZPositionSP.Value;
        {Container Modbus Words} 
        Container:=Containers[GlobalContainerNo];
        {TODO: Add container command masks}
        {Container.ReverseEnabled:=mb_ContainerCommand.Value and $0A
        Container.GantryPatternEnabled:=mb_ContainerCommand.Value and $0B;
        Container.UpdateSettings:=mb_ContainerCommand.Value and $0C;}
        Container.Id:=mb_CurrentContainer;
        Container.Revolutions:=mb_DispenseRevolutions.Value;
        Container.ReverseRevolutions:=mb_ReverseRevolutions.Value;
        Container.DispenseSpeed:=mb_DispenseSpeed.Value;
        Container.ReverseSpeed:=mb_ReverseSpeed.Value;
        Container.GantryPattern:=mb_GantryPatern.Value;
        Container.Accel:=mb_Accel.Value;
        Container.Decel:=mb_Decel.Value;
        Container.Current:=mb_Current.Value;
        Container.WeightSP:=mb_DispenseWeight.Value;
        Container.XCoord:=mb_ContainerXCoord.Value;
        Container.YCoord:=mb_ContainerYCoord.Value;
        Container.ZCoord:=mb_ContainerZCoord.Value;
        Container.StagingXCoord:=mb_ContainerStagingX.Value;
        Container.StagingYCoord:=mb_ContainerStagingY.Value;
    end;

procedure MapModbusMMCOutputs;
    begin
        {System Modbus Outputs}
        mb_SysStatus.SetValue(SystemStatusEnum)
        {Gantry Modbus Outputs}
        mb_GantryStatus.SetValue(0){}
        mb_XActualPosition.SetValue(Gantry.XMotor.ActualPosition)
        mb_YActualPosition.SetValue(Gantry.YMotor.ActualPosition)
        mb_ZActualPosition.SetValue(Gantry.ZMotor.ActualPosition)
        mb_ContainerStatus.SetValue(0){}
    end;

procedure SystemEventHandler;
    var StartSignalMem:boolean;
    begin
        if GlobalStartSignal and (not StartSignalMem) then  
            OctoMode.StartCycle;
        StartSignalMem:=StartSignal;
    end;



procedure ModbusEventDispatcher;
    var StartSignalMem:boolean;
    begin
        while true do
            begin
            yield;
                begin
                    MapModbusMMCInputs;
                    SystemEventHandler;
                    Gantry.EventHandler;
                    MapModbusMMCOutputs;
                end;
            end;
    end;