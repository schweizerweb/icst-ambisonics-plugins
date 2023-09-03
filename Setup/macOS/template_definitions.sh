#!/usr/bin/env bash

packageTemplate="
<dict>
	<key>CHILDREN</key>
    <array/>
    <key>DESCRIPTION</key>
    <array/>
    <key>OPTIONS</key>
    <dict>
        <key>HIDDEN</key>
        <false/>
        <key>STATE</key>
        <integer>1</integer>
    </dict>
    <key>PACKAGE_UUID</key>
    <string>__PACKAGE_UUID__</string>
    <key>TITLE</key>
    <array/>
    <key>TYPE</key>
    <integer>0</integer>
    <key>UUID</key>
    <string>__INSTALLER_UUID__</string>
</dict>
"

fileDefinitionTemplate="
<dict>
    <key>BUNDLE_CAN_DOWNGRADE</key>
    <false/>
    <key>CHILDREN</key>
    <array/>
    <key>GID</key>
    <integer>0</integer>
    <key>PATH</key>
    <string>__ABSOLUTE_FILEPATH__</string>
    <key>PATH_TYPE</key>
    <integer>0</integer>
    <key>PERMISSIONS</key>
    <integer>493</integer>
    <key>TYPE</key>
    <integer>3</integer>
    <key>UID</key>
    <integer>0</integer>
</dict>
"

fileSectionTemplateTemplates="
<dict>
    <key>DEFAULT_INSTALL_LOCATION</key>
    <string>/</string>
    <key>HIERARCHY</key>
    <dict>
        <key>CHILDREN</key>
        <array>
            <dict>
                <key>CHILDREN</key>
                <array>
                    <dict>
                        <key>CHILDREN</key>
                        <array>
                            <dict>
                                <key>CHILDREN</key>
                                <array>
                                <!--FileList-->
                                </array>
                                <key>GID</key>
                                <integer>0</integer>
                                <key>PATH</key>
                                <string>AmbiPluginsTemplatesTemp</string>
                                <key>PATH_TYPE</key>
                                <integer>2</integer>
                                <key>PERMISSIONS</key>
                                <integer>509</integer>
                                <key>TYPE</key>
                                <integer>2</integer>
                                <key>UID</key>
                                <integer>0</integer>
                            </dict>
                        </array>
                        <key>GID</key>
                        <integer>0</integer>
                        <key>PATH</key>
                        <string>Shared</string>
                        <key>PATH_TYPE</key>
                        <integer>0</integer>
                        <key>PERMISSIONS</key>
                        <integer>1023</integer>
                        <key>TYPE</key>
                        <integer>1</integer>
                        <key>UID</key>
                        <integer>0</integer>
                    </dict>
                </array>
                <key>GID</key>
                <integer>80</integer>
                <key>PATH</key>
                <string>Users</string>
                <key>PATH_TYPE</key>
                <integer>0</integer>
                <key>PERMISSIONS</key>
                <integer>493</integer>
                <key>TYPE</key>
                <integer>1</integer>
                <key>UID</key>
                <integer>0</integer>
            </dict>
        </array>
        <key>GID</key>
        <integer>0</integer>
        <key>PATH</key>
        <string>/</string>
        <key>PATH_TYPE</key>
        <integer>0</integer>
        <key>PERMISSIONS</key>
        <integer>493</integer>
        <key>TYPE</key>
        <integer>1</integer>
        <key>UID</key>
        <integer>0</integer>
    </dict>
    <key>PAYLOAD_TYPE</key>
    <integer>0</integer>
    <key>PRESERVE_EXTENDED_ATTRIBUTES</key>
    <false/>
    <key>SHOW_INVISIBLE</key>
    <false/>
    <key>SPLIT_FORKS</key>
    <true/>
    <key>TREAT_MISSING_FILES_AS_WARNING</key>
    <false/>
    <key>VERSION</key>
    <integer>5</integer>
</dict>
<key>PACKAGE_SCRIPTS</key>
<dict>
    <key>POSTINSTALL_PATH</key>
    <dict>
        <key>PATH</key>
        <string>__POSTINSTALL_SCRIPT__</string>
        <key>PATH_TYPE</key>
        <integer>0</integer>
    </dict>
    <key>PREINSTALL_PATH</key>
    <dict>
        <key>PATH_TYPE</key>
        <integer>0</integer>
    </dict>
    <key>RESOURCES</key>
    <array/>
</dict>
"

fileSectionTemplateBinaries="
<dict>
    <key>DEFAULT_INSTALL_LOCATION</key>
    <string>/</string>
    <key>HIERARCHY</key>
    <dict>
        <key>CHILDREN</key>
        <array>
            <dict>
                <key>CHILDREN</key>
                <array>
                    <dict>
                        <key>CHILDREN</key>
                        <array>
                            <dict>
                                <key>CHILDREN</key>
                                <array>
                                <!--FileList-->
                                </array>
                                <key>GID</key>
                                <integer>0</integer>
                                <key>PATH</key>
                                <string>Plug-Ins</string>
                                <key>PATH_TYPE</key>
                                <integer>2</integer>
                                <key>PERMISSIONS</key>
                                <integer>509</integer>
                                <key>TYPE</key>
                                <integer>2</integer>
                                <key>UID</key>
                                <integer>0</integer>
                            </dict>
                        </array>
                        <key>GID</key>
                        <integer>0</integer>
                        <key>PATH</key>
                        <string>Audio</string>
                        <key>PATH_TYPE</key>
                        <integer>2</integer>
                        <key>PERMISSIONS</key>
                        <integer>509</integer>
                        <key>TYPE</key>
                        <integer>2</integer>
                        <key>UID</key>
                        <integer>0</integer>
                    </dict>
                </array>
                <key>GID</key>
                <integer>0</integer>
                <key>PATH</key>
                <string>Library</string>
                <key>PATH_TYPE</key>
                <integer>0</integer>
                <key>PERMISSIONS</key>
                <integer>493</integer>
                <key>TYPE</key>
                <integer>1</integer>
                <key>UID</key>
                <integer>0</integer>
            </dict>
        </array>
        <key>GID</key>
        <integer>0</integer>
        <key>PATH</key>
        <string>/</string>
        <key>PATH_TYPE</key>
        <integer>0</integer>
        <key>PERMISSIONS</key>
        <integer>493</integer>
        <key>TYPE</key>
        <integer>1</integer>
        <key>UID</key>
        <integer>0</integer>
    </dict>
    <key>PAYLOAD_TYPE</key>
    <integer>0</integer>
    <key>PRESERVE_EXTENDED_ATTRIBUTES</key>
    <false/>
    <key>SHOW_INVISIBLE</key>
    <false/>
    <key>SPLIT_FORKS</key>
    <true/>
    <key>TREAT_MISSING_FILES_AS_WARNING</key>
    <false/>
    <key>VERSION</key>
    <integer>5</integer>
</dict>"
			
folderTemplate="
<dict>
    <key>CHILDREN</key>
    <array>
        <!--FileList-->
    </array>
    <key>GID</key>
    <integer>0</integer>
    <key>PATH</key>
    <string>__FILESECTION_FOLDERNAME__</string>
    <key>PATH_TYPE</key>
    <integer>2</integer>
    <key>PERMISSIONS</key>
    <integer>509</integer>
    <key>TYPE</key>
    <integer>2</integer>
    <key>UID</key>
    <integer>0</integer>
</dict>
"

mainFileSectionTemplate="
<dict>
    <key>MUST-CLOSE-APPLICATION-ITEMS</key>
    <array/>
    <key>MUST-CLOSE-APPLICATIONS</key>
    <false/>
    <key>PACKAGE_FILES</key>
    __FILE_SECTION__
    <key>PACKAGE_SETTINGS</key>
    <dict>
        <key>AUTHENTICATION</key>
        <integer>1</integer>
        <key>CONCLUSION_ACTION</key>
        <integer>0</integer>
        <key>FOLLOW_SYMBOLIC_LINKS</key>
        <false/>
        <key>IDENTIFIER</key>
        <string>com.icst.pkg.ICSTAmbiPlugins__PACKAGE_ID__</string>
        <key>LOCATION</key>
        <integer>0</integer>
        <key>NAME</key>
        <string>__PACKAGE_NAME__</string>
        <key>OVERWRITE_PERMISSIONS</key>
        <true/>
        <key>PAYLOAD_SIZE</key>
        <integer>-1</integer>
        <key>REFERENCE_PATH</key>
        <string></string>
        <key>RELOCATABLE</key>
        <false/>
        <key>USE_HFS+_COMPRESSION</key>
        <false/>
        <key>VERSION</key>
        <string>__PACKAGE_VERSION__</string>
    </dict>
    <key>TYPE</key>
    <integer>0</integer>
    <key>UUID</key>
    <string>__PACKAGE_UUID__</string>
</dict>
"
