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
DefaultDirName={autopf64}\Common Files\VST3
DisableDirPage=no
DefaultGroupName=ICST_AmbiPlugins
DisableProgramGroupPage=yes
InfoBeforeFile=..\readme.txt
; Uncomment the following line to run in non administrative install mode (install for current user only.)
PrivilegesRequired=admin
OutputBaseFilename=AmbiPluginPackageName
OutputDir=../packages
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\bin\VST3\AmbiDecoder_O1_8CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O1_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O2_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O3_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O4_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O5_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O6_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O7_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O1_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O1_8CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O1_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O2_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O2_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O3_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O3_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O4_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O4_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O5_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O5_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O6_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O6_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O7_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O7_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\Redist\VC_redist.x64.exe"; DestDir: {tmp}; Flags: dontcopy
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Run]
Filename: "{tmp}\VC_redist.x64.exe"; StatusMsg: "{cm:InstallingVCredist}"; \
  Parameters: "/quiet"; Check: VCRedistNeedsInstall ; Flags: waituntilterminated

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