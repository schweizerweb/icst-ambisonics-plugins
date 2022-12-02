/*
  ==============================================================================

    EncoderPreset.h
    Created: 19 Jan 2020 9:37:14pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AudioParams.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PresetHelper.h"
#include "../../Common/TrackColors.h"
#include "../../Common/ScalingInfo.h"
#include "EncoderSettings.h"

#define DEFAULT_PRESET_NAME "Single Source"

class EncoderPresetHelper : public PresetHelper
{
public:
    EncoderPresetHelper(File presetDirectory, ActionListener* pActionListener, ScalingInfo* pScaling) : PresetHelper(presetDirectory, pActionListener), pScalingInfo(pScaling)
    {
    }
    
    bool loadFromXmlFile(const File file, AudioParams* pAudioParams, AmbiSourceSet* pSourceSet, EncoderSettings* pEncoderSettings)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("EncoderPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "EncoderPreset")
            return false;
        
        pSourceSet->loadFromXml(rootElement->getChildByName("AmbiSourceSet"), pAudioParams);
        pSourceSet->resetIds();
        
        // apply scaler if audio params attached
        if(pAudioParams != nullptr)
            pScalingInfo->SetScaler(pSourceSet->getDistanceScaler());
        
        return pSourceSet->size() > 0;
    }

    bool writeToXmlFile(const File file, AmbiSourceSet* pSourceSet)
    {
        XmlElement* rootElement = new XmlElement("EncoderPreset");

        XmlElement* xmlSources = new XmlElement("AmbiSourceSet");
        pSourceSet->writeToXmlElement(xmlSources);
        rootElement->addChildElement(xmlSources);
        
        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
    
    bool checkValid(File presetFile) override {
        std::unique_ptr<AmbiSourceSet> testSet(new AmbiSourceSet(pScalingInfo));
        EncoderSettings testSettings;
        if(loadFromXmlFile(presetFile, nullptr, testSet.get(), &testSettings))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override {
        buildDefaultPreset(DEFAULT_PRESET_NAME,
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="0"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="a6832c483a024c94a7a371c3f037808c" X="0.0" Y="0.0" Z="0.0"
              Name="1" ColorCode="ff0000ff" Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups/>
  </AmbiSourceSet>
</EncoderPreset>)"
        );
        
        buildDefaultPreset("5.1-Surround",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="54a33757f2a640a79746e6b9abea0f90" X="-0.5000000000000004"
              Y="0.8660254037844384" Z="0.0" Name="L" ColorCode="ff000000"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="1b6e1bcebe63496fa9a6f30d0d33878d" X="0.4999999999999999"
              Y="0.8660254037844387" Z="0.0" Name="R" ColorCode="ff000000"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="18146304124348c094f26e424adb5289" X="0.0" Y="1.0" Z="0.0"
              Name="C" ColorCode="ff000000" Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="ec73a20633794546b48e1abe59d8939d" X="-0.3" Y="1.0" Z="0.0"
              Name="LFE" ColorCode="ffef1515" Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="7a6bbc46a3cf409ebb90fa4834675172" X="-0.9396926207859082"
              Y="-0.3420201433256693" Z="0.0" Name="LS" ColorCode="ff000000"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="942d01be4f544bc1b6103ea570677e49" X="0.9396926207859084"
              Y="-0.3420201433256687" Z="0.0" Name="RS" ColorCode="ff000000"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups />
  </AmbiSourceSet>
</EncoderPreset>)"
        );
        
        buildDefaultPreset("Cube",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="0"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="b0fccafb4211487f854c92e2fdfb8fb8" X="-0.106293913623977"
              Y="0.7112159149364798" Z="0.0" Name="1" ColorCode="ff0006ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="798a9bbc336c4980913a36552c945438" X="-0.1100001154811099"
              Y="0.6499167281987294" Z="0.0" Name="2" ColorCode="ff0006ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="ee11934bf41d4bf3bc034e13359247fb" X="-0.1100071244163954"
              Y="0.5996571449875034" Z="0.0" Name="3" ColorCode="ff0006ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="89450d2201ed4273942f693ac76d6cdd" X="-0.06354169113639867"
              Y="0.7793175922719899" Z="0.0" Name="4" ColorCode="ffbf7224"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="89b1a00ac40f4c88a2a8092bf7e41b28" X="-0.01593479171394108"
              Y="0.7789202738093093" Z="0.0" Name="5" ColorCode="ffbf7224"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="da4db5b0b15646c39e5568571367cf6b" X="0.0411806649051443"
              Y="0.7793170246438124" Z="0.0" Name="6" ColorCode="ffbf7224"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="3d1511cc380a4c6d8bb78d867a70ec54" X="0.08002784865490853"
              Y="0.7231210613363712" Z="0.0" Name="7" ColorCode="ff564a0c"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="9a78473c8d8e476fb97bfc027cc7fad3" X="0.07790193064454126"
              Y="0.6646805860757743" Z="0.0" Name="8" ColorCode="ff564a0c"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="d29efcf1e38445a8a3698de40a27681d" X="0.0719322503111921"
              Y="0.6100719118041676" Z="0.0" Name="9" ColorCode="ff564a0c"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="6ee4bdc811ca4ecf9ad764483c5b18d1" X="-0.005813956260681152"
             Y="0.4534883499145508" Z="0.0" Name="Cube" ColorCode="ffbf7224"
             Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="da4db5b0b15646c39e5568571367cf6b"/>
          <SubPoint Id="89b1a00ac40f4c88a2a8092bf7e41b28"/>
          <SubPoint Id="89450d2201ed4273942f693ac76d6cdd"/>
          <SubPoint Id="b0fccafb4211487f854c92e2fdfb8fb8"/>
          <SubPoint Id="798a9bbc336c4980913a36552c945438"/>
          <SubPoint Id="ee11934bf41d4bf3bc034e13359247fb"/>
          <SubPoint Id="3d1511cc380a4c6d8bb78d867a70ec54"/>
          <SubPoint Id="9a78473c8d8e476fb97bfc027cc7fad3"/>
          <SubPoint Id="d29efcf1e38445a8a3698de40a27681d"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.0"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("GRM-Quadro",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="397960bd6d764818997f58504e399c18" X="-0.3941190455056673"
              Y="0.4840203451964563" Z="0.0" Name="Src_1" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="0d957af4d70f4246a84249c3a3f2680c" X="0.4742049971075997"
              Y="0.3951239116572702" Z="0.0" Name="Src_2" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="15bb166235a640129d0d1184ce5c03ec" X="0.3853085635684135"
              Y="-0.4732001309559968" Z="0.0" Name="Src_3" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="ae282f49b6e941cc8b4b2fbe4ee230a0" X="-0.4830154790448534"
              Y="-0.3843036974168108" Z="0.0" Name="Src_4" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="dc784286f3b04668b0a9b142d3f1cf5f" X="0.007992982864379883"
             Y="-0.007992982864379883" Z="0.0" Name="Src" ColorCode="ff0042ff"
             Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="397960bd6d764818997f58504e399c18"/>
          <SubPoint Id="0d957af4d70f4246a84249c3a3f2680c"/>
          <SubPoint Id="15bb166235a640129d0d1184ce5c03ec"/>
          <SubPoint Id="ae282f49b6e941cc8b4b2fbe4ee230a0"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="-0.05098867333465824" Q3="-0.9986992315965662"/>
        <Stretch Factor="1.620201700925827"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("Group Demo",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="97f683deb4fa4fddbb61987e02677588" X="0.09302324056625366"
              Y="-0.3139535188674927" Z="-0.01069518551230431" Name="1" ColorCode="ffff4500"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="2d82179b8143435ca13092e71f6b6832" X="0.3139534592628479"
              Y="-0.1627907752990723" Z="0.01069518551230431" Name="2" ColorCode="ffff4500"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="969f20205d3e40d7bd825371bcfba995" X="-0.3139534592628479"
              Y="0.1337209343910217" Z="0.0" Name="3" ColorCode="ff040404"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="7c82da2b92834a249219a4f3aa5c61fb" X="0.0116279125213623"
              Y="0.2906976342201233" Z="0.0" Name="4" ColorCode="ff040404"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="8f8f6c94797740e6b570c03ebee2593f" X="0.0" Y="0.1860464811325073"
              Z="0.0" Name="5" ColorCode="ff0053ff" Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="39363b6688ed484284d6acd38d91c683" X="0.2151162624359131"
              Y="0.02906978130340576" Z="0.0" Name="6" ColorCode="ff0053ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="b361000732634710b665746d1db684f6" X="0.1918604373931885"
              Y="0.25" Z="0.0" Name="7" ColorCode="ff0053ff" Gain="1.0" Mute="0"
              Enabled="1"/>
      <Source Uuid="248680ff7d3b4a528068bcf51e7b0636" X="0.08139538764953613"
              Y="0.2558139562606812" Z="0.0" Name="8" ColorCode="ff1e5c16"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="6c2aa0a29f8842c4a6c9e8134c326206" X="0.1686046719551086"
              Y="-0.04651165008544922" Z="0.0" Name="9" ColorCode="ff1e5c16"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="a4273a4125f54144a01683c304423560" X="0.2790697813034058"
              Y="0.1976743936538696" Z="0.0" Name="10" ColorCode="ff1e5c16"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="8bfca8ac089646f983647cd147f778ce" X="0.5348837375640869"
             Y="0.2325581312179565" Z="0.0" Name="G1" ColorCode="ff7b7c85"
             Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="969f20205d3e40d7bd825371bcfba995"/>
          <SubPoint Id="7c82da2b92834a249219a4f3aa5c61fb"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.4504049802279626" Q3="0.8928243689471344"/>
        <Stretch Factor="0.73"/>
      </Group>
      <Group Uuid="f6a95285572f41aea0c4ee7abf676b3f" X="0.4476743936538696"
             Y="-0.7267441749572754" Z="0.0" Name="G2" ColorCode="ff767881"
             Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="8f8f6c94797740e6b570c03ebee2593f"/>
          <SubPoint Id="39363b6688ed484284d6acd38d91c683"/>
          <SubPoint Id="b361000732634710b665746d1db684f6"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.0"/>
      </Group>
      <Group Uuid="f184aed5f97a4b7b92b9cd9263371c69" X="-0.6220930218696594"
             Y="0.6220930814743042" Z="0.08556149527430534" Name="G3" ColorCode="ff827b79"
             Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="2d82179b8143435ca13092e71f6b6832"/>
          <SubPoint Id="97f683deb4fa4fddbb61987e02677588"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.0"/>
      </Group>
      <Group Uuid="0f71f349f11c410bb7a3d8fbe20042fe" X="-0.7732558250427246"
             Y="-0.5058139562606812" Z="0.0" Name="G4" ColorCode="ff7d7673"
             Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="248680ff7d3b4a528068bcf51e7b0636"/>
          <SubPoint Id="6c2aa0a29f8842c4a6c9e8134c326206"/>
          <SubPoint Id="a4273a4125f54144a01683c304423560"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.0"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("Octagon Clockwise",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="af87a5aae3c54a65b7dcae7c7f1b2cff" X="-0.3802792500355717"
              Y="0.924871716505799" Z="0.0" Name="Src_1" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="55ca290e147d4a999b3773ce9d3c0815" X="0.380290227231409"
              Y="0.9248672029390399" Z="0.0" Name="Src_2" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="6e0b2b876f9b46f5846aac0023759ddd" X="0.9214877046078528"
              Y="0.3884075311532342" Z="0.0" Name="Src_3" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="43bcae55567b47f59c7a4fc471fb7e22" X="0.9262393130257575"
              Y="-0.3769359826357426" Z="0.0" Name="Src_4" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="896982abed6e46ebaa79db43d1b76c05" X="0.3850852908429511"
              Y="-0.9228809884142157" Z="0.0" Name="Src_5" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="c1381f22bb604452ad0aefd1ca6784b7" X="-0.3850941782487695"
              Y="-0.9228772799667921" Z="0.0" Name="Src_6" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="2c8ea4f426224fef8d9468ac492dd695" X="-0.9262768476342116"
              Y="-0.3768437362313821" Z="0.0" Name="Src_7" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="6b0c883c08cc44bc92c6ee2950539ad0" X="-0.9214493435352096"
              Y="0.3884985293389555" Z="0.0" Name="Src_8" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="dda19e6bd4324d63880c2a20cd4f7207" X="0.0" Y="-0.006750941276550293"
             Z="0.0" Name="Src" ColorCode="ff224e92" Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="af87a5aae3c54a65b7dcae7c7f1b2cff"/>
          <SubPoint Id="55ca290e147d4a999b3773ce9d3c0815"/>
          <SubPoint Id="6e0b2b876f9b46f5846aac0023759ddd"/>
          <SubPoint Id="43bcae55567b47f59c7a4fc471fb7e22"/>
          <SubPoint Id="896982abed6e46ebaa79db43d1b76c05"/>
          <SubPoint Id="c1381f22bb604452ad0aefd1ca6784b7"/>
          <SubPoint Id="2c8ea4f426224fef8d9468ac492dd695"/>
          <SubPoint Id="6b0c883c08cc44bc92c6ee2950539ad0"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.0"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("Octagon Pairwise",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="ec321ef91bcf406db26cf25e246cfdea" X="-0.3802792500355717"
              Y="0.924871716505799" Z="0.0" Name="Src_1" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="6e9762b503104fe399f0b03eebf1625a" X="0.380290227231409"
              Y="0.9248672029390399" Z="0.0" Name="Src_2" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="d0f4c73bc61e4b6ba6def528db412cad" X="-0.9238795325112866"
              Y="0.38268343236509" Z="0.0" Name="Src_3" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="a95bf4c46727433885ca56fd04d43d2e" X="0.9238795325112867"
              Y="0.3826834323650898" Z="0.0" Name="Src_4" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="f3d0a922c3ce46cfa30d04c8e6dd3d65" X="-0.9238795325112868"
              Y="-0.3826834323650895" Z="0.0" Name="Src_5" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="6f74381ee76f43efb32a1b27e7123f3b" X="0.9238795325112866"
              Y="-0.3826834323650897" Z="0.0" Name="Src_6" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="7b84d9b370504ad09abeb9135f54c7ea" X="-0.3826834323650892"
              Y="-0.9238795325112868" Z="0.0" Name="Src_7" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="49768e74bf0c456089d672024916edfc" X="0.3826834323650898"
              Y="-0.9238795325112866" Z="0.0" Name="Src_8" ColorCode="ff224e92"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="a6e0a8cc97074fde81ad32aa018fd054" X="0.0" Y="-0.006750941276550293"
             Z="0.0" Name="Src" ColorCode="ff224e92" Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="ec321ef91bcf406db26cf25e246cfdea"/>
          <SubPoint Id="6e9762b503104fe399f0b03eebf1625a"/>
          <SubPoint Id="d0f4c73bc61e4b6ba6def528db412cad"/>
          <SubPoint Id="a95bf4c46727433885ca56fd04d43d2e"/>
          <SubPoint Id="f3d0a922c3ce46cfa30d04c8e6dd3d65"/>
          <SubPoint Id="6f74381ee76f43efb32a1b27e7123f3b"/>
          <SubPoint Id="7b84d9b370504ad09abeb9135f54c7ea"/>
          <SubPoint Id="49768e74bf0c456089d672024916edfc"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.0"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("Quadro Stockhausen",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="397960bd6d764818997f58504e399c18" X="-0.4313109257555716"
              Y="0.4313109257555705" Z="0.0" Name="Src_1" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="0d957af4d70f4246a84249c3a3f2680c" X="0.4313109257555705"
              Y="0.4313109257555705" Z="0.0" Name="Src_2" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="15bb166235a640129d0d1184ce5c03ec" X="0.43131092575557"
              Y="-0.4313109257555698" Z="0.0" Name="Src_3" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="ae282f49b6e941cc8b4b2fbe4ee230a0" X="-0.4313109257555703"
              Y="-0.4313109257555705" Z="0.0" Name="Src_4" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="dc784286f3b04668b0a9b142d3f1cf5f" X="0.0" Y="-0.01201939582824707"
             Z="0.0" Name="Src" ColorCode="ff0042ff" Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="397960bd6d764818997f58504e399c18"/>
          <SubPoint Id="0d957af4d70f4246a84249c3a3f2680c"/>
          <SubPoint Id="15bb166235a640129d0d1184ce5c03ec"/>
          <SubPoint Id="ae282f49b6e941cc8b4b2fbe4ee230a0"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.3830754839857818" Q3="-0.9237170419392829"/>
        <Stretch Factor="1.620201700925827"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("Square",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="397960bd6d764818997f58504e399c18" X="-0.4313109257555716"
              Y="0.4313109257555705" Z="0.0" Name="Src_1" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="0d957af4d70f4246a84249c3a3f2680c" X="0.4313109257555705"
              Y="0.4313109257555705" Z="0.0" Name="Src_2" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="15bb166235a640129d0d1184ce5c03ec" X="0.43131092575557"
              Y="-0.4313109257555698" Z="0.0" Name="Src_3" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="ae282f49b6e941cc8b4b2fbe4ee230a0" X="-0.4313109257555703"
              Y="-0.4313109257555705" Z="0.0" Name="Src_4" ColorCode="ff0042ff"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="dc784286f3b04668b0a9b142d3f1cf5f" X="0.0" Y="-0.01201939582824707"
             Z="0.0" Name="Src" ColorCode="ff0042ff" Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="397960bd6d764818997f58504e399c18"/>
          <SubPoint Id="0d957af4d70f4246a84249c3a3f2680c"/>
          <SubPoint Id="15bb166235a640129d0d1184ce5c03ec"/>
          <SubPoint Id="ae282f49b6e941cc8b4b2fbe4ee230a0"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.620201700925827"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("Star 16",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="0.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="c33dc0957fa6495d89e43129e082cf44" X="-0.06145144525024403"
              Y="0.1619178130190702" Z="0.0" Name="1" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="9a8f027c8a664c328e83f16f53f9f7ef" X="0.07610571100349181"
              Y="0.1599313843373298" Z="0.0" Name="2" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="9baeedc182b74d9798d615618a5b9426" X="0.1631835035930922"
              Y="0.06877197173264316" Z="0.0" Name="3" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="cc0e3ccb69af43f0bc21db03cf04a0ed" X="0.1609352376341189"
              Y="-0.07387021477424507" Z="0.0" Name="4" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="5bf99bb6cb764528bd13f4e2fc01d491" X="0.07244927121201003"
              Y="-0.1585364953588268" Z="0.0" Name="5" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="48454f3282244ea48da6bfe46589049a" X="-0.0902710514536022"
              Y="-0.1501555893398727" Z="0.0" Name="6" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="5c494c8447e24825986885e289678b2b" X="-0.1671788230152166"
              Y="-0.05951996663705949" Z="0.0" Name="7" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="31e0c9a2daa64941befcee2f8a287997" X="-0.1679968603619226"
              Y="0.07308315092908155" Z="0.0" Name="8" ColorCode="ff000eff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="d8b05ffee9f14db793c54dcdd39b74b6" X="-0.005422406922032842"
              Y="0.2299553511289852" Z="0.0" Name="9" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="b315f605f5494477a688efaf861de387" X="0.162795711353334"
              Y="0.1615376012181994" Z="0.0" Name="10" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="ed642032ed0d4600938032e4c3ac0288" X="0.2301680017915773"
              Y="0.01084479788436187" Z="0.0" Name="11" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="0b0639817f134402953a6eafea75e8ef" X="0.159765583872114"
              Y="-0.1592516504211918" Z="0.0" Name="12" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="2bc6d3fa452347b3aeec2221de512aee" X="-0.01034858482752265"
              Y="-0.2299553511289851" Z="0.0" Name="13" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="0cc227f0e0c446e9ae8b900c82db5bb8" X="-0.1816854531752052"
              Y="-0.1343901877675197" Z="0.0" Name="14" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="99ca0e33d262416ba2f746a75cfcdee6" X="-0.2301680017915773"
              Y="0.0003189478597808702" Z="0.0" Name="15" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="fd33f4e593d74453943584b87e2cfe9e" X="-0.1658258757606497"
              Y="0.1609173550831758" Z="0.0" Name="16" ColorCode="ffff5600"
              Gain="1.0" Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="2bb7817ee5fd4b078a581e6f1a826330" X="0.006059885025024414"
             Y="0.003029942512512207" Z="0.0" Name="G3" ColorCode="ffff4500"
             Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="c33dc0957fa6495d89e43129e082cf44"/>
          <SubPoint Id="9a8f027c8a664c328e83f16f53f9f7ef"/>
          <SubPoint Id="9baeedc182b74d9798d615618a5b9426"/>
          <SubPoint Id="cc0e3ccb69af43f0bc21db03cf04a0ed"/>
          <SubPoint Id="5bf99bb6cb764528bd13f4e2fc01d491"/>
          <SubPoint Id="48454f3282244ea48da6bfe46589049a"/>
          <SubPoint Id="5c494c8447e24825986885e289678b2b"/>
          <SubPoint Id="31e0c9a2daa64941befcee2f8a287997"/>
          <SubPoint Id="d8b05ffee9f14db793c54dcdd39b74b6"/>
          <SubPoint Id="b315f605f5494477a688efaf861de387"/>
          <SubPoint Id="ed642032ed0d4600938032e4c3ac0288"/>
          <SubPoint Id="0b0639817f134402953a6eafea75e8ef"/>
          <SubPoint Id="2bc6d3fa452347b3aeec2221de512aee"/>
          <SubPoint Id="0cc227f0e0c446e9ae8b900c82db5bb8"/>
          <SubPoint Id="99ca0e33d262416ba2f746a75cfcdee6"/>
          <SubPoint Id="fd33f4e593d74453943584b87e2cfe9e"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.599319931268692"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
        
        buildDefaultPreset("Stereo",
R"(<?xml version="1.0" encoding="UTF-8"?>

<EncoderPreset>
  <AmbiSourceSet>
    <GroupMode Enable="1"/>
    <DistanceScaler Factor="1.0"/>
    <MasterGain Value="0.0"/>
    <Sources>
      <Source Uuid="c3f33b316b714930a27b92ff119b0337" X="-0.2668470973369521"
              Y="0.2723260158893507" Z="0.0" Name="1" ColorCode="ff0000ff"
              Gain="1.0" Mute="0" Enabled="1"/>
      <Source Uuid="6ecbf064dd594e989ef27bc474b02de6" X="0.2832618544940256"
              Y="0.277782935941627" Z="0.0" Name="2" ColorCode="ff0000ff" Gain="1.0"
              Mute="0" Enabled="1"/>
    </Sources>
    <Groups>
      <Group Uuid="85a8d020474a4294a7044a56d2a3e507" X="0.0" Y="0.0" Z="0.0"
             Name="G1" ColorCode="ffff4500" Gain="1.0" Mute="0" Enabled="1">
        <SubPoints>
          <SubPoint Id="c3f33b316b714930a27b92ff119b0337"/>
          <SubPoint Id="6ecbf064dd594e989ef27bc474b02de6"/>
        </SubPoints>
        <Rotation Q0="0.0" Q1="0.0" Q2="0.0" Q3="1.0"/>
        <Stretch Factor="1.0"/>
      </Group>
    </Groups>
  </AmbiSourceSet>
</EncoderPreset>
)"
        );
    }
    
    bool loadDefaultPreset(AudioParams* pAudioParams, AmbiSourceSet* pSourceSet, EncoderSettings* pEncoderSettings)
    {
        bool found = false;
        for(File f : presetFiles)
        {
            if(f.getFileNameWithoutExtension() == DEFAULT_PRESET_NAME)
            {
                loadFromXmlFile(f, pAudioParams, pSourceSet, pEncoderSettings);
                found = true;
                break;
            }
        }

        return found;
    }
    
private:
    ScalingInfo* pScalingInfo;
};
