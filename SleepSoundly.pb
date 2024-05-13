EnableExplicit

InitSound()
OpenWindow(0, #PB_Ignore, #PB_Ignore, #PB_Ignore, #PB_Ignore, "SleepSoundly", #PB_Window_Invisible): CreateImage(0, 32, 32)
AddSysTrayIcon(0, WindowID(0), ImageID(0))
SysTrayIconToolTip(0, "SleepSoundly")
CreatePopupMenu(0)
MenuItem(0, "Preferences")
MenuItem(1, "Exit")
SysTrayIconMenu(0, MenuID(0))
CatchSound(0, ?Silence)
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

DataSection
	Silence:
	IncludeBinary "Silence.wav"
EndDataSection
