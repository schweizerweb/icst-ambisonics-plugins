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
DisableDirPage=yes
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
Source: "..\bin\VST3\AmbiDecoder_O1_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O1_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O2_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O2_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O3_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O3_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O4_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O4_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O5_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O5_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O6_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O6_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O7_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiDecoder_O7_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O1_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O1_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O1_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O2_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O2_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O2_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O3_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O3_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O3_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O4_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O4_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O4_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O5_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O5_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O5_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O6_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O6_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O6_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O7_1CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O7_32CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\VST3\AmbiEncoder_O7_64CH.vst3"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

