# Installation and Testing Instructions

## Latest Build - Crash Fix v2
**Build Time**: After second round of crash fixes  
**Key Changes**: Deferred initialization, better error handling

### Critical Fixes in This Version:
1. ✅ **Deferred Input Capture** - Hooks now start on first tick, not during creation
2. ✅ **Graphics Context Safety** - Added try-catch around texture creation
3. ✅ **Empty Entry Handling** - No rendering when there are no keystrokes
4. ✅ **Better Logging** - More detailed error messages for diagnosis

## Installation Steps

### Step 1: Close OBS Completely
- Make SURE OBS is fully closed
- Check Task Manager to verify no OBS processes are running

### Step 2: Install the Plugin

**Method A - PowerShell Script (Run as Administrator)**:
```powershell
cd "d:\OBS Plugins\Keystroke History"
.\install-plugin.ps1
```

**Method B - Manual Copy**:
1. Navigate to: `D:\OBS Plugins\Keystroke History\build\Release\`
2. Copy `keystroke-history.dll`
3. Paste to: `C:\Program Files\obs-studio\obs-plugins\64bit\`
   (Requires Administrator)

### Step 3: Start OBS

**IMPORTANT**: Run OBS as Administrator for keyboard hooks to work:
1. Right-click OBS shortcut
2. Select "Run as administrator"

### Step 4: Test the Plugin

1. **Add the Source**:
   - In OBS, go to Sources panel
   - Click + (Plus button)
   - Scroll down to **"Keystroke History"**
   - Click OK (use default settings first)

2. **What Should Happen**:
   - Source should appear in sources list without crashing
   - You should see a small empty box in the preview
   - No crash = SUCCESS!

3. **Test Keystroke Capture**:
   - Click on another application (like Notepad or browser)
   - Type some keys: `hello world`
   - Keystrokes should appear in the OBS preview!

## If It Crashes Again

### Immediate Steps:
1. Close OBS
2. Check the log file (see below)
3. Send me the relevant parts of the log

### Get the Log File:
```powershell
# Run this in PowerShell to find and open the latest log
$logPath = "$env:APPDATA\obs-studio\logs"
$latestLog = Get-ChildItem $logPath -Filter "*.txt" | Sort-Object LastWriteTime -Descending | Select-Object -First 1
notepad $latestLog.FullName
```

### What to Look For in the Log:
Search for these lines (Ctrl+F):
- `[Keystroke History]` - All plugin messages
- `Plugin loaded successfully` - Should be there
- `source created` - Should appear when you add the source
- `GDI+ initialized` - Should appear shortly after
- `Input capture started` - Should appear within first second
- Any `ERROR` or `exception` messages

### Send Me This Info:
1. Last 20 lines that contain `[Keystroke History]`
2. Any error messages after those lines
3. What you were doing when it crashed

## Expected Log Messages

**Successful Startup**:
```
[Keystroke History] Plugin loaded successfully (version 1.0.0)
[Keystroke History] Keystroke History source created (capture will start on first render)
[Keystroke History] GDI+ initialized successfully
[Keystroke History] Input capture started successfully
[Keystroke History] Keyboard hook installed successfully
[Keystroke History] Mouse hook installed successfully
```

## Testing Checklist

- [ ] OBS starts without crashing
- [ ] Can add Keystroke History source without crashing  
- [ ] Source appears in sources list
- [ ] Can see empty overlay in preview
- [ ] Typing in another app shows keystrokes in OBS
- [ ] Mouse clicks are captured (if enabled)
- [ ] Key combinations work (Ctrl+C, Alt+Tab, etc.)
- [ ] Keystrokes in password fields are hidden

## Settings to Try

Once it's working, try these settings:

### Minimal Safe Settings:
- Maximum Entries: 3
- Font: Arial
- Font Size: 20
- Show Mouse Clicks: No
- Fade Duration: 0 (no fade)
- Background: No

### Recommended Settings:
- Maximum Entries: 5
- Font: Arial
- Font Size: 24
- Show Mouse Clicks: Yes
- Ignore Modifier Keys: Yes
- Fade Duration: 3 seconds
- Background: Yes (50% opacity)

### Advanced Settings:
- Maximum Entries: 10
- Font: Consolas or Courier New (monospace)
- Font Size: 28
- All features enabled

## Troubleshooting

### Problem: Plugin doesn't appear in sources list
- Check if DLL is in correct location
- Restart OBS
- Check OBS log for loading errors

### Problem: Source adds but shows nothing
- Type some keys in another application
- Make sure OBS is running as Administrator
- Check if hooks were installed (look in log)

### Problem: Keystrokes not showing
- OBS must run as Administrator for hooks to work
- Some anti-virus software may block keyboard hooks
- Check Windows Security settings

### Problem: OBS becomes unresponsive
- Reduce Maximum Entries to 3
- Disable fade effects (set to 0)
- Disable background
- Try different font

## Debug Mode

To get maximum logging, I can add more debug output. Let me know if you need a debug build with verbose logging.

## Success Indicators

You'll know it's working when:
1. ✅ OBS doesn't crash when adding the source
2. ✅ You see the source in your sources list
3. ✅ Typing shows up in the OBS preview
4. ✅ The overlay updates in real-time
5. ✅ Old entries fade out (if fade enabled)

## Next Steps After Success

Once working:
1. Position and resize the overlay in your scene
2. Customize colors to match your stream
3. Adjust font size for visibility
4. Test with your actual streaming setup
5. Create scenes with different configurations

## Contact

If it still crashes after this build:
- Send me the log file section with Keystroke History messages
- Tell me the exact moment it crashes (on adding source? on first keystroke? etc.)
- List your OBS version and Windows version

I'll analyze the logs and create a more defensive version if needed.
