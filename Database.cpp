#include "Database.h"
#include <QDir>
#include <iostream>
#include "Localization.h"
#include "GameAtlas.h"

Database::Database() :
    m_localization(nullptr),
    m_gameAtlas(nullptr)
{
    m_localization = new Localization();
    m_gameAtlas = new GameAtlas();
    m_gameAtlas->setLocalization(m_localization);
}

void Database::createDatabase(std::string _databasePath, std::string _tasksPath,
    std::string _initialCacheBlockPath, std::string _savePath)
{
    m_localization->createLocalizations();

    // Parse file. Search regions, maps, tasks. Add transcription.
    m_gameAtlas->createGameAtlasData(_tasksPath);

    // Parse save for upgrades list.
    // Parse initial.cache_block for upgrades middle code.
    // Find translation to middle code of upgrades.

    // Перенести эту функцию в createGameAtlasData?
    m_gameAtlas->createUpgradesData(_savePath, _initialCacheBlockPath);
    m_gameAtlas->createTrucksData();
    m_gameAtlas->connectUpgradesWithTrucks();


    m_gameAtlas->saveGameAtlasData(_databasePath);
    m_localization->saveLocalizationCache(_databasePath);
}

bool Database::loadDatabase(std::string _filename)
{
    bool localizationLoaded = m_localization->loadLocalizations(_filename);
    if (!localizationLoaded) {
        return false;
    }
    m_localization->setDefaultLanguage(Language::RUSSIAN);

    bool gameAtlasLoaded = m_gameAtlas->loadGameAtlas(_filename);
    if (!gameAtlasLoaded) {
        return false;
    }

    return true;
}

Localization* Database::localization()
{
    return m_localization;
}

GameAtlas* Database::gameAtlas()
{
    return m_gameAtlas;
}
