EnableExplicit

;{ Init
InitSound()
OpenWindow(0, #PB_Ignore, #PB_Ignore, #PB_Ignore, #PB_Ignore, "SleepSoundly", #PB_Window_Invisible)
CreateImage(0, 32, 32)
AddSysTrayIcon(0, WindowID(0), ImageID(0))
SysTrayIconToolTip(0, "SleepSoundly")
CreatePopupMenu(0)
MenuItem(0, "Preferences")
MenuItem(1, "Exit")
CatchSound(0, ?Silence)
;} End Init

;{ Main
PlaySound(0, #PB_Sound_Loop)
Repeat
  Define Event = WaitWindowEvent(1)
  Select Event
    Case #PB_Event_SysTray
      HideWindow(0, #False)
      DisplayPopupMenu(0, WindowID(0))
      HideWindow(0, #True)
    Case #PB_Event_Menu
      Select EventMenu()
        Case 0
          MessageRequester("", "Preferences")
        Case 1
          End
      EndSelect
  EndSelect
Until Event = #PB_Event_CloseWindow
;} End Main

;{ Data
DataSection
  Silence:
  IncludeBinary "Silence.wav"
EndDataSection
;} End Data

; IDE Options = PureBasic 6.03 LTS (Windows - x64)
; CursorPosition = 6
; FirstLine = 6
; Folding = -
; EnableXP
; DPIAware
; Executable = SleepSoundly.exe