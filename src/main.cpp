#include "../include/main.hpp"

#include <dlfcn.h>

#include "../customui/customui.hpp"

int stop = 0;

CustomUI::TextObject Maul;

float grayScaleColor = 1.0f;

Vector2 posOffset = {-41.3f, -16.9f};

float fontSize = 3.6f;

bool leftHand;

bool noArrows;

void llog(const std::string &text)
{
  Logger::get().info("[QuestDarthMaul dev0002]" + text);
}

MAKE_HOOK_OFFSETLESS(Refresh, void, Il2CppObject *self)
{
  if (stop == 1)
  {
    Il2CppObject *nameText = il2cpp_utils::GetFieldValue(self, "_nameText").value();
    Il2CppObject *nameTextTransform;
    Il2CppObject *nameTextParent;

    nameTextTransform =
        il2cpp_utils::RunMethodUnsafe(nameText, "get_transform").value();
    nameTextParent =
        il2cpp_utils::RunMethodUnsafe(nameTextTransform, "GetParent").value();

    Maul.text = "/Darth Maul Mode";
    Maul.fontSize = fontSize;
    Maul.parentTransform = nameTextParent;
    Maul.sizeDelta = posOffset;
    Maul.color = {grayScaleColor, grayScaleColor, grayScaleColor,
                  grayScaleColor};
    Maul.create();
  }
  stop++;

  Refresh(self);
}

MAKE_HOOK_OFFSETLESS(SongStart,
                     void,
                     Il2CppObject *self,
                     Il2CppString *gameMode,
                     Il2CppObject *difficultyBeatmap,
                     Il2CppObject *overrideEnvironmentSettings,
                     Il2CppObject *overrideColorScheme,
                     Il2CppObject *gameplayModifiers,
                     Il2CppObject *playerSpecificSettings,
                     Il2CppObject *practiceSettings,
                     Il2CppString *backButtonText,
                     bool useTestNoteCutSoundEffects)
{

  noArrows = il2cpp_utils::RunMethodUnsafe(gameplayModifiers, "get_noArrows").value();
  leftHand = il2cpp_utils::RunMethodUnsafe(playerSpecificSettings, "get_leftHanded").value();

  SongStart(self, gameMode, difficultyBeatmap, overrideEnvironmentSettings,
            overrideColorScheme, gameplayModifiers, playerSpecificSettings,
            practiceSettings, backButtonText, useTestNoteCutSoundEffects);
  llog("SongStart Hook");
}

MAKE_HOOK_OFFSETLESS(PlayerController_Update, void, Il2CppObject *self)
{
  llog("Activate");
  PlayerController_Update(self);

  if (noArrows)
  {
    Il2CppObject *leftSaber =
        il2cpp_utils::GetFieldValue(self, "_leftSaber").value();
    Il2CppObject *rightSaber =
        il2cpp_utils::GetFieldValue(self, "_rightSaber").value();

    if (leftSaber != nullptr && rightSaber != nullptr)
    {
      Il2CppObject *leftSaberTransform = nullptr;
      Il2CppObject *rightSaberTransform = nullptr;

      Il2CppClass *componentsClass = il2cpp_utils::GetClassFromName("", "Saber");
      leftSaberTransform = il2cpp_utils::RunMethod(leftSaber,
                                                   il2cpp_functions::class_get_method_from_name(
                                                       componentsClass, "get_transform", 0))
                               .value();
      rightSaberTransform =
          il2cpp_utils::RunMethod(rightSaber,
                                  il2cpp_functions::class_get_method_from_name(
                                      componentsClass, "get_transform", 0))
              .value();

      if (leftSaberTransform != nullptr && rightSaberTransform != nullptr)
      {
        Il2CppClass *transformClass =
            il2cpp_utils::GetClassFromName("UnityEngine", "Transform");
        const MethodInfo *getMethod =
            il2cpp_functions::class_get_method_from_name(
                transformClass, "get_localPosition", 0);
        const MethodInfo *setMethod =
            il2cpp_functions::class_get_method_from_name(
                transformClass, "set_localPosition", 1);
        const MethodInfo *setRotate =
            il2cpp_functions::class_get_method_from_name(transformClass,
                                                         "Rotate", 1);
        const MethodInfo *setTranslate =
            il2cpp_functions::class_get_method_from_name(transformClass,
                                                         "Translate", 1);
        const MethodInfo *getLocalRotation =
            il2cpp_functions::class_get_method_from_name(
                transformClass, "get_localRotation", 0);
        const MethodInfo *setLocalRotation =
            il2cpp_functions::class_get_method_from_name(
                transformClass, "set_localRotation", 1);

        Vector3 rightSaberLocalPosition;
        Vector3 leftSaberLocalPosition;
        rightSaberLocalPosition = il2cpp_utils::RunMethod<Vector3>(rightSaberTransform, getMethod).value();
        leftSaberLocalPosition = il2cpp_utils::RunMethod<Vector3>(leftSaberTransform, getMethod).value();

        Quaternion rightSaberLocalRotation;
        Quaternion leftSaberLocalRotation;
        rightSaberLocalRotation = il2cpp_utils::RunMethod<Quaternion>(rightSaberTransform, getLocalRotation).value();
        leftSaberLocalRotation = il2cpp_utils::RunMethod<Quaternion>(leftSaberTransform, getLocalRotation).value();

        if (!leftHand)
        {
          il2cpp_utils::RunMethod(leftSaberTransform, setMethod, rightSaberLocalPosition);
          il2cpp_utils::RunMethod(leftSaberTransform, setLocalRotation, rightSaberLocalRotation);
          il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{0, 180, 0});
          il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{0, 0, 0.335});
        }
        else
        {
          il2cpp_utils::RunMethod(rightSaberTransform, setMethod, leftSaberLocalPosition);
          il2cpp_utils::RunMethod(rightSaberTransform, setLocalRotation, leftSaberLocalRotation);
          il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{0, 180, 0});
          il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{0, 0, 0.335});
        }
        llog("position set");
      }
    }
  }
}

MAKE_HOOK_OFFSETLESS(HapticFeedbackController_HitNote,
                     void,
                     Il2CppObject *self,
                     int node)
{
  if (noArrows)
  {
    if (!leftHand)
    {
      node = 5;
    }
    else
    {
      node = 4;
    }
  }
  HapticFeedbackController_HitNote(self, node);
}

extern "C" void load()
{
  INSTALL_HOOK_OFFSETLESS(Refresh, il2cpp_utils::FindMethodUnsafe("", "GameplayModifierToggle", "Start", 0));
  INSTALL_HOOK_OFFSETLESS(
      SongStart,
      il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 9));
  INSTALL_HOOK_OFFSETLESS(
      PlayerController_Update,
      il2cpp_utils::FindMethodUnsafe("", "SaberManager", "Update", 0));
  INSTALL_HOOK_OFFSETLESS(
      HapticFeedbackController_HitNote,
      il2cpp_utils::FindMethodUnsafe("", "NoteCutHapticEffect", "HitNote", 1));
  llog("Hooks installed.");
}