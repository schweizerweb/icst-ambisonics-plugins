#define MyAppName "AmbiPluginPackageName"
#define MyAppVersion "AmbiPluginVersion"
#define MyAppPublisher "Institute for Computer Music and Sound Technology"
#define MyAppURL "https://www.zhdk.ch/forschung/icst"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{3A492C0F-A415-4507-864F-B009FC2E9EE5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\Common Files\VST3
DisableDirPage=no
DefaultGroupName=ICST_AmbiPlugins
DisableProgramGroupPage=yes
InfoBeforeFile=AmbiPluginReadmePath
OutputBaseFilename={#MyAppName}
OutputDir=packages
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: __REDIST_PLACEHOLDER__; DestDir: "{tmp}"; Flags: dontcopy
; __FILE_SECTION__

[Run]
Filename: "{tmp}\VC_redist.x64.exe"; StatusMsg: "Installing Visual C++ Redistributables"; \
  Parameters: "/quiet"; Check: VCRedistNeedsInstall ; Flags: waituntilterminated

[Components]
; __COMPONENT_SECTION__

[Dirs]
; __DIR_SECTION__

[Code]
function VCRedistNeedsInstall: Boolean;
var 
  Version: String;
begin
  if RegQueryStringValue(HKEY_LOCAL_MACHINE,
       'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Version', Version) then
  begin
    // Is the installed version at least 14.28 ? 
    Log('VC Redist Version check : found ' + Version);
    Result := (CompareStr(Version, 'v14.28.29913.00')<0);
  end
  else 
  begin
    // Not even an old version installed
    Result := True;
  end;
  if (Result) then
  begin
    ExtractTemporaryFile('VC_redist.x64.exe');
  end;
end;