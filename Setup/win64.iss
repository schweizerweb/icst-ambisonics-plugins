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
InfoBeforeFile=..\readme.txt
; Uncomment the following line to run in non administrative install mode (install for current user only.)
PrivilegesRequired=admin
OutputBaseFilename=AmbiPluginPackageName
OutputDir=../packages
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\bin\VST3\AmbiDecoder.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_Mono.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_Multi.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: ".\Redist\VC_redist.x64.exe"; DestDir: "{tmp}"; Flags: dontcopy
Source: "ICST_AmbisonicsTemplates\ICST Mono Ambisonics.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion; Components: ReaperTemplates
Source: "ICST_AmbisonicsTemplates\ICST_Ambi_OSC_DEMO.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion; Components: ReaperTemplates
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Multi_O1.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion; Components: ReaperTemplates
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Multi_O3.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion; Components: ReaperTemplates
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Multi_O5.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion; Components: ReaperTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\dearVR_Binaural_Ambi_Micro.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Bformat_O1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Bformat_O3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Bformat_O5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_DECODER_O1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_DECODER_O3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_DECODER_O5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Multi_4x_mono_sources.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Multi_8x_mono_sources.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Multi_16x_mono_sources.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Multi_O1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Multi_O3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Multi_O5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Solo_O1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Solo_O3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Encoder-Solo_O5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\IEM_Binaural_Decoder_O1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\IEM_Binaural_Decoder_O3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\IEM_Binaural_Decoder_O5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST Ambisonics Plugins"; Flags: ignoreversion; Components: ReaperTrackTemplates

[Run]
Filename: "{tmp}\VC_redist.x64.exe"; StatusMsg: "Installing Visual C++ Redistributables"; \
  Parameters: "/quiet"; Check: VCRedistNeedsInstall ; Flags: waituntilterminated

[Components]
Name: "VST3Plugins"; Description: "ICST Ambisonics Plugins VST3"; Types: compact custom full
Name: "ReaperTemplates"; Description: "Reaper Project Templates"; Types: full custom
Name: "ReaperTrackTemplates"; Description: "Reaper Track Templates"; Types: custom full

[Dirs]
Name: "{userappdata}/REAPER\ProjectTemplates"; Components: ReaperTemplates
Name: "{userappdata}/REAPER\TrackTemplates"; Components: ReaperTrackTemplates

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