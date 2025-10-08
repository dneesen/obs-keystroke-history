; OBS Keystroke History Plugin Installer
; Inno Setup Script

#define MyAppName "OBS Keystroke History Plugin"
#define MyAppVersion "1.0.0"
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
DisableDirPage=yes
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=LICENSE
OutputDir=.
OutputBaseFilename=obs-keystroke-history-installer-v{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "build\Release\keystroke-history.dll"; DestDir: "{app}\obs-plugins\64bit"; Flags: ignoreversion
Source: "data\locale\en-US.ini"; DestDir: "{app}\data\obs-plugins\keystroke-history\locale"; Flags: ignoreversion

[Messages]
WelcomeLabel2=This will install the %1 on your computer.%n%nMake sure OBS Studio is closed before continuing.

[Code]
function InitializeSetup(): Boolean;
var
  ResultCode: Integer;
begin
  // Check if OBS Studio is installed
  if not DirExists(ExpandConstant('{autopf}\obs-studio')) then
  begin
    MsgBox('OBS Studio installation not found!' + #13#10 + #13#10 + 
           'Please install OBS Studio first.' + #13#10 + 
           'Download from: https://obsproject.com', mbError, MB_OK);
    Result := False;
  end
  else
  begin
    Result := True;
  end;
end;

function InitializeUninstall(): Boolean;
begin
  Result := MsgBox('Are you sure you want to remove the OBS Keystroke History Plugin?', 
                   mbConfirmation, MB_YESNO) = IDYES;
end;

[Run]
Filename: "{app}\bin\64bit\obs64.exe"; Description: "Launch OBS Studio"; Flags: nowait postinstall skipifsilent unchecked

[UninstallDelete]
Type: files; Name: "{app}\obs-plugins\64bit\keystroke-history.dll"
Type: files; Name: "{app}\data\obs-plugins\keystroke-history\locale\en-US.ini"
Type: dirifempty; Name: "{app}\data\obs-plugins\keystroke-history\locale"
Type: dirifempty; Name: "{app}\data\obs-plugins\keystroke-history"
