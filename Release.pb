EnableExplicit

; Determine if the given argument was passed to the executable. Searches case-insensatively.
Macro IsCmdArg(_Arg): Bool(FindString(ProgramParameter(), _Arg, #PB_String_NoCase) > 0): EndMacro

; Concatinate a list of files/folders with the correct path seperator.
Procedure$ ConcatFiles(Files$)
	Protected Res$, I
	For I = 1 To CountString(Files$, ",") + 1
		Res$ + StringField(Files$, I, ",") + #PS$
	Next
	ProcedureReturn RTrim(Res$, #PS$)
EndProcedure

; Attempt to determine the name of a project by reading the .git/config file. If there are any errors, the return value is an empty string.
Procedure$ GetProjectName()
	Protected ConfigPath$ =ConcatFiles(GetCurrentDirectory() + ",.git,config")
	If FileSize(ConfigPath$) <= 0: ProcedureReturn "": EndIf
	If Not OpenPreferences(ConfigPath$): ProcedureReturn "": EndIf
	PreferenceGroup(~"remote \"origin\"")
	Protected ConfigURL$ = ReadPreferenceString("url", "")
	If ConfigURL$ = "": ProcedureReturn "": EndIf
	If Not CreateRegularExpression(0, "^(?:https?:\/\/)?(?:www\.)?.*?\/([^\/]+?)(?:\.git)?$"): ProcedureReturn: EndIf
	ExamineRegularExpression(0, ConfigURL$)
	NextRegularExpressionMatch(0)
	ProcedureReturn RegularExpressionGroup(0, 1)
EndProcedure

Procedure$ Sanatize(TheData.s)
	TheData = Trim(TheData, " ")
	TheData = Trim(TheData, "-")
	ProcedureReturn TheData
EndProcedure

; Compiles the project with the given name using the PB IDE. You should have PureBasic.exe on your PATH.
Procedure.b CompilePBProject(ProjectName.s)
	If FileSize(ProjectName + ".pbp") <= 0: Goto Error: EndIf
	Protected PBIde = RunProgram("PureBasic", "/BUILD " + ProjectName + ".pbp", GetCurrentDirectory(), #PB_Program_Open | #PB_Program_Read | #PB_Program_Error | #PB_Program_UTF8 | #PB_Program_Hide)
	If Not IsProgram(PBIde): Goto Error: EndIf
	Protected Output.s
	Repeat
		Protected BytesAvail = AvailableProgramOutput(PBIde)
		If BytesAvail > 0: Output + ReadProgramString(PBIde) + #LF$: EndIf
		Protected Error$ = ReadProgramError(PBIde)
		If Error$ <> "": Output + ReadProgramError(PBIde): EndIf
	Until BytesAvail <= 0 Or Not ProgramRunning(PBIde)
	CloseProgram(PBIde)
	ProcedureReturn #True
	Error:
	ProcedureReturn #False
EndProcedure

Procedure.b FinalizeBinary(BinaryName$)
	If FileSize(BinaryName$) <= 0: Goto Error: EndIf
	If Not IsCmdArg("NO_UPX")
		Protected Extension.s = LCase(GetExtensionPart(BinaryName$))
		If Extension = "exe" Or Extension = "dll":RunProgram("upx", "--best --force-overwrite --compress-icons=3 " + BinaryName$, GetCurrentDirectory(), #PB_Program_Wait | #PB_Program_Hide): EndIf
	EndIf
	ProcedureReturn #True
	Error:
	ProcedureReturn #False
EndProcedure

Define Projname$ = GetProjectName()
If Projname$ = "": MessageRequester("No project found", "", #PB_MessageRequester_Error): End 1: EndIf
If Not CompilePBProject(Projname$): MessageRequester("Couldn't compile project", "", #PB_MessageRequester_Error): End 1: EndIf
If Not FinalizeBinary(Projname$ + ".exe"): MessageRequester("Couldn't finalize binary", "", #PB_MessageRequester_Error): End 1: EndIf
