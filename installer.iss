; OBS Keystroke History Plugin Installer
; Inno Setup Script

#define MyAppName "OBS Keystroke History Plugin"
#define MyAppVersion "1.2.0"
#define MyAppPublisher "David Neesen"
#define MyAppURL "https://github.com/dneesen/obs-keystroke-history"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
AppId={{8F9C3D2E-1B4A-4C7D-9E8F-2A5B6C7D8E9F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\obs-studio
DisableDirPage=no
DirExistsWarning=no
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=LICENSE
OutputDir=.
OutputBaseFilename=obs-keystroke-history-installer-v{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\bin\64bit\obs64.exe
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription={#MyAppName} Installer

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; Plugin DLL - replace on upgrade
Source: "build\Release\keystroke-history.dll"; DestDir: "{app}\obs-plugins\64bit"; Flags: ignoreversion restartreplace uninsrestartdelete
; Locale files - replace on upgrade
Source: "data\locale\en-US.ini"; DestDir: "{app}\data\obs-plugins\keystroke-history\locale"; Flags: ignoreversion

[Messages]
WelcomeLabel2=This will install the %1 on your computer.%n%nMake sure OBS Studio is closed before continuing.
SelectDirLabel3=Select the folder where OBS Studio is installed.%n%nThis is usually:[lb]C:\Program Files\obs-studio[lb][lb]If OBS is installed elsewhere, browse to that location.
SelectDirBrowseLabel=Click Next to continue, or Browse to select a different OBS installation folder.

[Code]
function IsOBSRunning(): Boolean;
var
  ResultCode: Integer;
begin
  // Check if OBS is running using tasklist
  Result := (Exec('cmd.exe', '/C tasklist /FI "IMAGENAME eq obs64.exe" 2>NUL | find /I /N "obs64.exe">NUL', '', SW_HIDE, ewWaitUntilTerminated, ResultCode)) and (ResultCode = 0);
end;

function InitializeSetup(): Boolean;
var
  MsgText: String;
begin
  // Check if OBS Studio is running
  if IsOBSRunning() then
  begin
    MsgText := 'OBS Studio is currently running!' + #13#10 + #13#10 + 
               'Please close OBS Studio before continuing with the installation.' + #13#10 + #13#10 +
               'Click OK to exit the installer.';
    MsgBox(MsgText, mbError, MB_OK);
    Result := False;
    Exit;
  end;
  
  // Auto-detect OBS installation - but still allow user to choose
  if not DirExists(ExpandConstant('{autopf}\obs-studio')) then
  begin
    MsgText := 'OBS Studio installation not found in the default location!' + #13#10 + #13#10 + 
               'You will need to manually select your OBS installation folder in the next step.' + #13#10 + #13#10 +
               'If OBS Studio is not installed, please install it first from:' + #13#10 +
               'https://obsproject.com';
    MsgBox(MsgText, mbInformation, MB_OK);
  end;
  
  Result := True;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  // When on the directory selection page, validate OBS installation
  if CurPageID = wpSelectDir then
  begin
    // Just show the page, validation will happen in NextButtonClick
  end;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
  OBSExePath: String;
  PluginDir: String;
begin
  Result := True;
  
  // Validate OBS installation when leaving directory selection page
  if CurPageID = wpSelectDir then
  begin
    OBSExePath := WizardDirValue + '\bin\64bit\obs64.exe';
    PluginDir := WizardDirValue + '\obs-plugins\64bit';
    
    // Check if this looks like an OBS installation
    if not FileExists(OBSExePath) then
    begin
      MsgBox('The selected directory does not appear to be a valid OBS Studio installation.' + #13#10 + #13#10 +
             'obs64.exe not found at:' + #13#10 +
             OBSExePath + #13#10 + #13#10 +
             'Please select the correct OBS Studio installation folder.', 
             mbError, MB_OK);
      Result := False;
      Exit;
    end;
    
    // Warn if plugin directory doesn't exist (but allow installation)
    if not DirExists(PluginDir) then
    begin
      if MsgBox('The obs-plugins directory does not exist yet.' + #13#10 + #13#10 +
                'This is normal for fresh OBS installations.' + #13#10 +
                'The installer will create the necessary directories.' + #13#10 + #13#10 +
                'Continue with installation?', 
                mbConfirmation, MB_YESNO) = IDNO then
      begin
        Result := False;
        Exit;
      end;
    end;
  end;
end;

function InitializeUninstall(): Boolean;
var
  MsgText: String;
begin
  // Check if OBS is running during uninstall
  if IsOBSRunning() then
  begin
    MsgText := 'OBS Studio is currently running!' + #13#10 + #13#10 + 
               'Please close OBS Studio before uninstalling the plugin.' + #13#10 + #13#10 +
               'Click OK to exit the uninstaller.';
    MsgBox(MsgText, mbError, MB_OK);
    Result := False;
    Exit;
  end;

  Result := MsgBox('Are you sure you want to remove the OBS Keystroke History Plugin?' + #13#10 + #13#10 +
                   'Your plugin settings and saved data will not be removed.', 
                   mbConfirmation, MB_YESNO) = IDYES;
end;

[Run]
Filename: "{app}\bin\64bit\obs64.exe"; Description: "Launch OBS Studio"; Flags: nowait postinstall skipifsilent unchecked

[UninstallDelete]
Type: files; Name: "{app}\obs-plugins\64bit\keystroke-history.dll"
Type: files; Name: "{app}\data\obs-plugins\keystroke-history\locale\en-US.ini"
Type: dirifempty; Name: "{app}\data\obs-plugins\keystroke-history\locale"
Type: dirifempty; Name: "{app}\data\obs-plugins\keystroke-history"
