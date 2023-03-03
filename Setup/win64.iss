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
Source: "..\bin\VST3\AmbiDecoder_o1_8ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiDecoder_o1_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiDecoder_o2_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiDecoder_o3_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiDecoder_o4_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiDecoder_o5_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiDecoder_o6_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiDecoder_o7_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o1_1ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o1_8ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o1_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o2_1ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o2_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o3_1ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o3_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o4_1ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o4_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o5_1ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o5_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o6_1ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o6_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o7_1ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: "..\bin\VST3\AmbiEncoder_o7_64ch.vst3"; DestDir: "{app}"; Flags: ignoreversion; Components: VST3Plugins
Source: ".\Redist\VC_redist.x64.exe"; DestDir: "{tmp}"; Flags: dontcopy
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Mono_o1.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Mono_o3.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Mono_o5.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Multi_o1.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Multi_o3.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion
Source: "ICST_AmbisonicsTemplates\ICST_AmbiPlugins_Multi_o5.RPP"; DestDir: "{userappdata}/REAPER/ProjectTemplates"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\BF_Player_o1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\BF_Player_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\BF_Player_o5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Bformat-Master_o1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Bformat-Master_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Bformat-Master_o5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_DECODER_o1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_DECODER_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_DECODER_o5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Kompositionsstudio_Decoder_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Mono_AmbiEncoder_o1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Mono_AmbiEncoder_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Mono_AmbiEncoder_o5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Multi_AmbiEncoder_o1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Multi_AmbiEncoder_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Multi_AmbiEncoder_o3_4x_mono_sources.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Multi_AmbiEncoder_o3_8x_mono_sources.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Multi_AmbiEncoder_o3_16x_mono_sources.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ICST_Multi_AmbiEncoder_o5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins\ZHdK_IA-Space_Decoder_o5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\AMBEO A-to-B.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\DearVR_BINAURAL.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\FX_Bformat_o3(IEM).RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\IEM_BFormat_Manipulator_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\IEM_Binaural_Decoder_o1.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\IEM_Binaural_Decoder_o3.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\IEM_Binaural_Decoder_o5.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\RODE_Soundfield.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion
Source: "ICST_AmbisonicsTrackTemplates\ICST Ambisonics Plugins 3rd party\ZYLIA_to_Bformat.RTrackTemplate"; DestDir: "{userappdata}/REAPER\TrackTemplates\ICST AmbiPlugins 3rdParty"; Flags: ignoreversion

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