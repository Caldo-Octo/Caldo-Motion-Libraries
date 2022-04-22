{----------------------------------------------------------------------------------------------------}
{                                                                                                    }
{                        Modbus TCP/IP Slave for use with HMIs and other Masters                     }
{                                                                                                    }
{----------------------------------------------------------------------------------------------------}
{                                                                                                    }
{ 20210511 - Persistence Improvements                                                                }
{____________________________________________________________________________________________________}


const ec_IllegalDataSizeField=1006;
const ec_DataNotPresent=1007;


Type TQuadToWordBigEndian=record
  Word3:integer;
  Word2:integer;
  Word1:integer;
  Word0:integer;
  end;

Type TLongintToWordBigEndian=record
  HighWord:integer;
  LowWord:integer;
  end;
  

function BooleanToWord(State:boolean):longint;
  begin
  if State then
    BooleanToWord:=1
  else
    BooleanToWord:=0;
  end;

var Modbus:object
  const MaxRegisterNumber=400;
  const MaxCoilNumber=100;
  
  Register:array[0..MaxRegisterNumber] of integer;
  Coil:array[0..MaxCoilNumber] of Boolean;
  RegisterPersistentKey:array[0..MaxRegisterNumber] of integer;
  CoilPersistentKey:array[0..MaxCoilNumber] of integer;

  ResponseString:string[127];
  ResponseIndex:longint;
  FunctionCode:longint;
  CommandCounter:longint;
  FaultCounter:longint;
  procedure SavePersistentModbusCoilValueAt(CoilAddress:longint); forward;
  procedure SavePersistentModbusRegisterValueAt(RegisterAddress:longint); forward;

  {---------------------------}
  Procedure SetLongintRegister(RegNumber:integer; Value:longint);
  {---------------------------}

	  var LongintToWord:TLongintToWord;

	  begin
	  LongintToWord:=TLongintToWord(Value);
	  Register[RegNumber]:=LongintToWord.LowWord;
	  Register[RegNumber+1]:=LongintToWord.HighWord;
	  end;

  {---------------------------}
  Function LongintRegister(RegNumber:integer):longint;
  {---------------------------}

	  var LongintToWord:TLongintToWord;

	  begin
	  LongintToWord.LowWord:=Register[RegNumber];
	  LongintToWord.HighWord:=Register[RegNumber+1];
	  LongintRegister:=Longint(LongintToWord);
	  end;
	  
  {---------------------------}
  Procedure SetSingleRegister(RegNumber:integer; Value:single);
  {---------------------------}

    begin
	SetLongintRegister(RegNumber,longint(Value));
    end;

  {---------------------------}
  Function SingleRegister(RegNumber:integer):single;
  {---------------------------}

    begin
	SingleRegister:=single(LongintRegister(RegNumber));
    end;	  
  

  {---------------------------}
  Procedure Initialize;
  {---------------------------}

    var scanner:longint;

    begin
    for scanner:=0 to MaxRegisterNumber do
      begin
      Register[scanner]:=0;
      RegisterPersistentKey[scanner]:=0;
      if (scanner and $1F)=0 then
        yield;
      end;

    for scanner:=0 to MaxCoilNumber do
      begin
      Coil[scanner]:=false;
      CoilPersistentKey[scanner]:=0;
      if (scanner and $1F)=0 then
        yield;
      end;
    end;

  {--------------------------------------------------------------------------------------}
  Procedure PatchMessageLength;
  {--------------------------------------------------------------------------------------}
  { The message length field in the header, at index 4, needs to be assigned after we    }
  { know the total message size after the Message Length field (new data starts at 6)    }
  {______________________________________________________________________________________}

    var MessageLength:longint;

    begin
    MessageLength:=ResponseIndex-6;
    StringAtPut(ResponseString,4,MessageLength div $100);
    StringAtPut(ResponseString,5,MessageLength and $FF);
    end;

  {------------------------------------------}
  Procedure ResponseAppendByte(aByte:longint);
  {------------------------------------------}

    begin
  {$IFDEF DEBUGACTIVE}
    DebugList.writeln('ResponseAppendByte: ',aByte);
  {$ENDIF}

    StringAtPut(ResponseString,ResponseIndex,aByte);
    ResponseIndex:=ResponseIndex+1;
    end;

  {------------------------------------------}
  Procedure ResponseAppendWord(aWord:longint);
  {------------------------------------------}

    var LongToWord:TLongintToWordBigEndian;
    var Value:longint;

    begin
  {$IFDEF DEBUGACTIVE}
    DebugList.writeln('ResponseAppendWord: ',aWord);
  {$ENDIF}

    StringAtPut(ResponseString,ResponseIndex,aWord div $100);
    ResponseIndex:=ResponseIndex+1;
    StringAtPut(ResponseString,ResponseIndex,aWord); {just copy low byte}
    ResponseIndex:=ResponseIndex+1;
    end;


  {---------------------------}
  Procedure SetLongintRegister(RegNumber:integer; Value:longint);
  {---------------------------}

    var Former:TLongintToWordBigEndian;

    begin
    Former:=TLongintToWordBigEndian(Value);
    Register[RegNumber+1]:=Former.HighWord;
    Register[RegNumber+2]:=Former.LowWord;
    end;


  {---------------------------}
  function GetLongintRegister(RegNumber:integer):longint;
  {---------------------------}

    var Former:TLongintToWordBigEndian;

    begin
    Former.LowWord:=Register[RegNumber+2];
    Former.HighWord:=Register[RegNumber+1];
    GetLongintRegister:=longint(Former);
    end;


  {---------------------------}
  Procedure SetDoubleRegister(RegNumber:integer; Value:Double);
  {---------------------------}

    var Former:TQuadToWordBigEndian;

    begin
    Former:=TQuadToWordBigEndian(Value);
    Register[RegNumber+1]:=Former.Word3;
    Register[RegNumber+2]:=Former.Word2;
    Register[RegNumber+3]:=Former.Word1;
    Register[RegNumber+4]:=Former.Word0;
    end;

  {---------------------------}
  function GetDoubleRegister(RegNumber:integer):double;
  {---------------------------}

    var Former:TQuadToWordBigEndian;

    begin
    Former.Word3:=Register[RegNumber+1];
    Former.Word2:=Register[RegNumber+2];
    Former.Word1:=Register[RegNumber+3];
    Former.Word0:=Register[RegNumber+4];
    GetDoubleRegister:=longint(Former);
    end;
  end;

{-------------------------------------------------------------}
Procedure Modbus.SavePersistentModbusCoilValueAt(CoilAddress:longint);
{-------------------------------------------------------------}

  var Key:longint;
  var MaskedIndex:longint;
  var CoilValue:boolean;
  var PersistentValue:boolean;

  begin
  Key:=CoilPersistentKey[CoilAddress];
  MaskedIndex:=Key and $FFF;
  CoilValue:=Coil[CoilAddress];
  PersistentValue:=(FlashManager.PersistentWord[MaskedIndex] <> 0);
  if PersistentValue <> CoilValue then
    begin
    if CoilValue <> 0 then
      FlashManager.PersistentWord[MaskedIndex]:=true
    else
      FlashManager.PersistentWord[MaskedIndex]:=false;
    FlashManager.PersistentWordChangedTimeStamp[MaskedIndex]:=SampleCounterAvoidZero;
    end;
  end;


{-------------------------------------------------------------}
Procedure Modbus.SavePersistentModbusRegisterValueAt(RegisterAddress:longint);
{-------------------------------------------------------------}
{ A change in the modbus registers has been indicated by a    }
{ routine in the protocol. Registers are organized into data  }
{ types that the protocol does not know about. Named Modbus   }
{ data types have indicated the data type information in an   }
{ array named RegisterPersistentKey. These array elements     }
{ contain the persistent key index format that identifies     }
{ data size, persistent index, and at bit position $1000      }
{ whether this particular register is the base register of a  }
{ multiregister type. This moves values form the modbus       }
{ registers over to the peristent cache. It can check to see  }
{ if there is a difference. If there's no difference then do  }
{ not rewrite.                                                }
{_____________________________________________________________}

  var Key:longint;
  var MaskedIndex:longint;
  var RegisterValue:longint;
  var DataSizeField:longint;
  var BaseAddress:longint;

  begin
  Key:=RegisterPersistentKey[RegisterAddress];
  DataSizeField:=Key and $E000;
  if DataSizeField = $2000 then {word register}
    begin
    {no need to check $1000 since all words are complete and at the right base}
    MaskedIndex:=Key and $FFF;
    RegisterValue:=Register[RegisterAddress];
    if FlashManager.PersistentWord[MaskedIndex] <> RegisterValue then
      begin
      FlashManager.PersistentWord[MaskedIndex]:=RegisterValue
      FlashManager.PersistentWordChangedTimeStamp[MaskedIndex]:=SampleCounterAvoidZero;
      end;    
    end

  else if DataSizeField = $4000 then {longint/single register}
    begin
    if (Key and $1000) = $1000 then
      BaseAddress=RegisterAddress
    else
      BaseAddress:=RegisterAddress-1;
    MaskedIndex:=Key and $FFF;
    RegisterValue:=LongintRegister(BaseAddress);
    if FlashManager.PersistentLongint[MaskedIndex] <> RegisterValue then
      begin
      FlashManager.PersistentLongint[MaskedIndex]:=RegisterValue
      FlashManager.PersistentLongintChangedTimeStamp[MaskedIndex]:=SampleCounterAvoidZero;
      end;    
    end

  else
    Escape(ec_IllegalDataSizeField);
  end;



{------------------------------------------------------------------------------}
Type TModbusCoil=object
{------------------------------------------------------------------------------}

  Index:longint;
  PersistentIndex:integer;  

  procedure Initialize(anUnusedModule:longint; anIndex:longint; aPersistentIndex:longint);

    var Value:integer;
    var PIndex:longint;
    var MaskedIndex:longint;

    begin
    Index:=anIndex; {maybe -1}
    PersistentIndex:=aPersistentIndex;
    if aPersistentIndex <> 0 then
      begin
      Modbus.CoilPersistentKey[Index]:=aPersistentIndex;
      MaskedIndex:=aPersistentIndex and $FFF;
      FlashManager.PersistentWordChangedTimeStamp[MaskedIndex]:=0;
      Value:=FlashManager.PersistentWord[MaskedIndex];
      Modbus.Coil[Index]:=(Value <> 0);
      end
    else
      Modbus.Coil[Index]:=false;
    end;

  procedure Turn(State:boolean);

    var MaskedIndex:longint;

    begin
    if PersistentIndex <> 0 then
      begin
      if Modbus.Coil[Index] <> State then
        begin
        Modbus.Coil[Index]:=State;
        MaskedIndex:=PersistentIndex and $FFF;
        if State then
          FlashManager.PersistentWord[MaskedIndex]:=1
        else
          FlashManager.PersistentWord[MaskedIndex]:=0;
        FlashManager.PersistentWordChangedTimeStamp[MaskedIndex]:=SampleCounterAvoidZero;
        end;
      end
    else
      Modbus.Coil[Index]:=State;
    end;

  function IsOn:boolean;
    begin
    IsOn:=Modbus.Coil[Index];
    end;

  function Is_On:boolean;
    begin
    Is_On:=Modbus.Coil[Index];
    end;

  function IsOff:boolean;
    begin
    IsOff:=not Modbus.Coil[Index];
    end;

  function Is_Off:boolean;
    begin
    Is_Off:=not Modbus.Coil[Index];
    end;
  end;

{------------------------------------------------------------------------------}
Type TModbusWord=object
{------------------------------------------------------------------------------}

  Index:longint;
  PersistentIndex:integer;

  procedure Initialize(anUnusedModule:longint; anIndex:longint; aPersistentIndex:longint);

    var Value:integer;
    var MaskedIndex:longint;

    begin
    Index:=anIndex;
    PersistentIndex:=aPersistentIndex;
    if aPersistentIndex <> 0 then
      begin
      Modbus.RegisterPersistentKey[Index]:=aPersistentIndex;
      MaskedIndex:=aPersistentIndex and $FFF;
      FlashManager.PersistentWordChangedTimeStamp[MaskedIndex]:=0;
      Value:=FlashManager.PersistentWord[MaskedIndex];
      Modbus.Register[Index]:=Value;
      end
    else
      Modbus.Register[Index]:=0;
    end;

  procedure SetValue(Value:single);

    var RoundedValue:longint;
    var MaskedIndex:longint;

    begin
    RoundedValue:=round(Value);
    if PersistentIndex <> 0 then
      begin
      if RoundedValue <> Modbus.Register[Index] then
        begin
        MaskedIndex:=PersistentIndex and $FFF;
        FlashManager.PersistentWord[MaskedIndex]:=RoundedValue;
        Modbus.Register[Index]:=RoundedValue;
        FlashManager.PersistentWordChangedTimeStamp[MaskedIndex]:=SampleCounterAvoidZero;
        end;
      end
    else
      Modbus.Register[Index]:=RoundedValue;
    end;

  procedure Turn(State:boolean);
    begin
    if State then
      SetValue(1)
    else
      SetValue(0);
    end;

  procedure Set(Value:single);
    begin
    SetValue(Value);
    end;

  function Value:longint;
    begin
    Value:=Modbus.Register[Index];
    end;

  function Is_On:boolean;
    begin
    Is_On:=(Modbus.Register[Index] <> 0);
    end;

  function IsOn:boolean;
    begin
    IsOn:=(Modbus.Register[Index] <> 0);
    end;

  function Is_Off:boolean;
    begin
    Is_Off:=(Modbus.Register[Index] = 0);
    end;

  function IsOff:boolean;
    begin
    IsOff:=(Modbus.Register[Index] = 0);
    end;
  end; {of TModbusWord}



{------------------------------------------------------------------------------}
Type TModbusLongint=object
{------------------------------------------------------------------------------}

  Index:longint;
  PersistentIndex:integer;

  procedure Initialize(anUnusedModule:longint; anIndex:longint; aPersistentIndex:longint);

    var MaskedIndex:longint;

    begin
    Index:=anIndex;
    PersistentIndex:=aPersistentIndex;
    if aPersistentIndex <> 0 then
      begin
      Modbus.RegisterPersistentKey[Index]:=aPersistentIndex or $1000;
      Modbus.RegisterPersistentKey[Index+1]:=aPersistentIndex;
      MaskedIndex:=PersistentIndex and $FFF;
      Modbus.SetLongintRegister(Index,round(FlashManager.PersistentLongint[MaskedIndex]))
      FlashManager.PersistentLongintChangedTimeStamp[MaskedIndex]:=0;
      end
    else
      Modbus.SetLongintRegister(Index,0);
    end;


  procedure SetValue(Value:double);

    var RoundedValue:longint;
    var MaskedIndex:longint;

    begin
    RoundedValue:=round(Value);
    if PersistentIndex <> 0 then
      begin
      if RoundedValue <> Modbus.LongintRegister(Index) then
        begin
        MaskedIndex:=PersistentIndex and $FFF;
        FlashManager.PersistentLongint[MaskedIndex]:=RoundedValue;
        Modbus.SetLongintRegister(Index,RoundedValue);
        FlashManager.PersistentLongintChangedTimeStamp[MaskedIndex]:=SampleCounterAvoidZero;
        end;
      end
    else
      Modbus.SetLongintRegister(Index,round(Value));
    end;


  procedure Set(Value:single);
    begin
    SetValue(Value);
    end;

  function Value:longint;
    begin
    Value:=Modbus.LongintRegister(Index);
    end;
  end; {of TModbusLongint}


{------------------------------------------------------------------------------}
Type TModbusSingle=object
{------------------------------------------------------------------------------}

  Index:longint;
  PersistentIndex:integer;

  procedure Initialize(anUnusedModule:longint; anIndex:longint; aPersistentIndex:integer);

    var Value:longint;
    var MaskedIndex:longint;

    begin
    Index:=anIndex;
    PersistentIndex:=aPersistentIndex;
    if aPersistentIndex <> 0 then
      begin
      Modbus.RegisterPersistentKey[Index]:=aPersistentIndex or $1000;
      Modbus.RegisterPersistentKey[Index+1]:=aPersistentIndex;
      MaskedIndex:=PersistentIndex and $FFF;
      Value:=FlashManager.PersistentLongint[MaskedIndex]
      Modbus.SetLongintRegister(anIndex,Value)
      FlashManager.PersistentLongintChangedTimeStamp[MaskedIndex]:=0;
      end
    else
      Modbus.SetSingleRegister(anIndex,0);
    end;


  procedure SetValue(Value:single);

    var MaskedIndex:longint;

    begin
    if PersistentIndex <> 0 then
      begin
      if Value <> Modbus.SingleRegister(Index) then
        begin
        MaskedIndex:=PersistentIndex and $FFF;
        FlashManager.PersistentLongint[MaskedIndex]:=longint(Value);
        Modbus.SetSingleRegister(Index,Value);
        FlashManager.PersistentLongintChangedTimeStamp[MaskedIndex]:=SampleCounterAvoidZero;
        end;
      end
    else
      Modbus.SetSingleRegister(Index,Value);
    end;


  procedure Set(Value:single);
    begin
    SetValue(Value);
    end;

  function Value:single;
    begin
    Value:=Modbus.SingleRegister(Index);
    end;
  end; {of TModbusSingle}



