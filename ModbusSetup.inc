const StatusWordIndex = $21;
const ContolWordIndex = $22;
const CurrentControllerIndex = $23;
const ModbusModuleNo = $10;
var mb_StatusWord:TModbusWord;
var mb_ControlWord:TModbusWord;
var mb_CurrentContainer:TModbusWord;

mb_StatusWord.Initialize(ModbusModuleNo, StatusWordIndex, 0);
yield;
mb_ControlWord.Initialize(ModbusModuleNo, ControlWorldIndex, 0);
yield;
mb_CurrentContainer.Initialize(ModbusModuleNo, CurrentControllerIndex, 0);
yield;


