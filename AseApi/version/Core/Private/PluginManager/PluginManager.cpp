#include <API/Api.h>
#include <API/Commands.h>
#include <API/Primal/PrimalPlayerData.h>
#include <API/Primal/PrimalDinoData.h>
#include "Logger/Log.h"

using namespace ArkApi;

// Макрос для цвета сообщений
#define GREEN FLinearColor{0.f, 1.f, 0.f, 1.f}

// Команда для вывода количества динозавров в племени
void TribeDinoCountCommand(APlayerController* controller, FString* /*cmd*/, bool)
{
    if (!controller) return;

    auto shooter_controller = static_cast<AShooterPlayerController*>(controller);
    if (!shooter_controller) return;

    auto player_data = shooter_controller->GetPlayerData();
    if (!player_data)
    {
        GetApiUtils().SendServerMessage(controller, L"Ошибка получения данных игрока!", GREEN);
        return;
    }

    int64 tribe_id = player_data->TribeIdField();
    if (tribe_id == 0)
    {
        GetApiUtils().SendServerMessage(controller, L"Вы не состоите в племени!", GREEN);
        return;
    }

    int dino_count = 0;
    auto all_dinos = GetApiUtils().GetAllDinos(); // Возвращает список всех динозавров на сервере

    for (auto& dino : all_dinos)
    {
        if (!dino) continue;

        if (dino->TribeIdField() == tribe_id)
            dino_count++;
    }

    FString msg = L"Количество динозавров в племени: " + FString::FromInt(dino_count);
    GetApiUtils().SendServerMessage(controller, msg, GREEN);

    Log::GetLog()->info("Игрок {} запросил количество динозавров племени: {}",
        std::string(TCHAR_TO_UTF8(*shooter_controller->GetPlayerName())), dino_count);
}

// Инициализация плагина
extern "C" PLUGIN_API void PluginInit()
{
    Log::GetLog()->info("Plugin загружен");
    GetCommands().AddConsoleCommand("tribedino", TribeDinoCountCommand);
}

// Выгрузка плагина
extern "C" PLUGIN_API void PluginUnload()
{
    GetCommands().RemoveConsoleCommand("tribedino");
    Log::GetLog()->info("Plugin выгружен");
}
