LOCAL_PATH := $(call my-dir)
TARGET_ARCH_ABI := $(APP_ABI)

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Creating prebuilt for dependency: beatsaber-hook - version: 0.8.4
include $(CLEAR_VARS)
LOCAL_MODULE := beatsaber-hook_0_8_4
LOCAL_EXPORT_C_INCLUDES := extern/beatsaber-hook
LOCAL_SRC_FILES := extern/libbeatsaber-hook_0_8_4.so
LOCAL_CPP_FEATURES += exceptions
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: modloader - version: 1.0.4
include $(CLEAR_VARS)
LOCAL_MODULE := modloader
LOCAL_EXPORT_C_INCLUDES := extern/modloader
LOCAL_SRC_FILES := extern/libmodloader.so
LOCAL_CPP_FEATURES += exceptions
include $(PREBUILT_SHARED_LIBRARY)
# Creating prebuilt for dependency: customui - version: 0.1.4
include $(CLEAR_VARS)
LOCAL_MODULE := customui
LOCAL_EXPORT_C_INCLUDES := extern/customui
LOCAL_SRC_FILES := extern/libcustom-ui_0_1_4.so
LOCAL_CPP_FEATURES += exceptions
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := riku1227_quest_darth_maul
LOCAL_SRC_FILES += $(call rwildcard,src/,*.cpp)
LOCAL_SRC_FILES += $(call rwildcard,extern/beatsaber-hook/src/inline-hook/,*.cpp)
LOCAL_SRC_FILES += $(call rwildcard,extern/beatsaber-hook/src/inline-hook/,*.c)
LOCAL_SHARED_LIBRARIES += beatsaber-hook_0_8_4
LOCAL_SHARED_LIBRARIES += modloader
LOCAL_SHARED_LIBRARIES += customui
LOCAL_LDLIBS += -llog
LOCAL_CFLAGS += -DVERSION='"1.0.1"' -DID='"QuestDarthMaul"' -I"C:\Program Files\Unity\Hub\Editor\2019.3.15f1\Editor\Data\il2cpp\libil2cpp"
LOCAL_CPPFLAGS += -std=c++2a -frtti
LOCAL_C_INCLUDES += ./include ./src ./shared ./extern ./extern/beatsaber-hook/shared ./extern/modloader/shared ./extern/questui/shared ./extern/custom-types/shared ./extern/codegen/include
LOCAL_CPP_FEATURES += exceptions
include $(BUILD_SHARED_LIBRARY)