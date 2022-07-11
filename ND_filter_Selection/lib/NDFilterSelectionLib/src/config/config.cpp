#include "config.hpp"

Config::Config(const char *configName, const char *partitionName) : _configName(configName),
                                                                    _partitionName(partitionName) {}

Config::~Config() {}

void Config::begin()
{
    if (_preferences.begin(_configName))
    {
        log_i("Config %s opened\n", _configName);
        _preferences.end();
        log_d("Config initialized\n");
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
    }
    else
    {
        log_e("Config %s not opened\n", _configName);
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::ErrorConfig);
    }
    delay(100);
}

/**
 * @brief Writes a value to the config
 *
 * @tparam T Type of the value to write
 * @param key Key of the value to write
 * @param value Value to write
 * @return void
 * @see Takes a buffer as parameter to avoid the need to create a temporary variable
 */
template <typename T>
void Config::write(const char *key, T &buff)
{
    if (stateManager.getCurrentConfigState() == ProgramStates::DeviceStates::ConfigState_e::Reading)
    {
        log_e("Config %s is in reading state\n", _configName);
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::ErrorConfig);
        return;
    }

    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Writing);
    _preferences.begin(_configName);
    _preferences.putBytes(key, (T *)&buff, sizeof(buff));
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
}

template void Config::write<uint8_t>(const char *key, uint8_t &buff);
template void Config::write<uint16_t>(const char *, uint16_t &buff);
template void Config::write<uint32_t>(const char *key, uint32_t &buff);
template void Config::write<uint64_t>(const char *key, uint64_t &buff);
template void Config::write<int8_t>(const char *key, int8_t &buff);
template void Config::write<int16_t>(const char *key, int16_t &buff);
template void Config::write<int32_t>(const char *key, int32_t &buff);
template void Config::write<int64_t>(const char *key, int64_t &buff);
template void Config::write<bool>(const char *key, bool &buff);
template void Config::write<float_t>(const char *key, float_t &buff);
template void Config::write<double_t>(const char *key, double_t &buff);
template void Config::write<const char *>(const char *key, const char *&buff);
template void Config::write<String>(const char *key, String &buff);
template void Config::write<Config::LDR_t>(const char *key, Config::LDR_t &buff);

template <typename T>
void Config::write(const char *key, T *&buff)
{
    if (stateManager.getCurrentConfigState() == ProgramStates::DeviceStates::ConfigState_e::Reading)
    {
        log_e("Config %s is in reading state\n", _configName);
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::ErrorConfig);
        return;
    }

    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Writing);
    _preferences.begin(_configName);
    _preferences.putBytes(key, (T *)buff, sizeof(buff));
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
}

template void Config::write<uint8_t>(const char *key, uint8_t *&buff);
template void Config::write<uint16_t>(const char *key, uint16_t *&buff);
template void Config::write<uint32_t>(const char *key, uint32_t *&buff);
template void Config::write<uint64_t>(const char *key, uint64_t *&buff);
template void Config::write<int8_t>(const char *key, int8_t *&buff);
template void Config::write<int16_t>(const char *key, int16_t *&buff);
template void Config::write<int32_t>(const char *key, int32_t *&buff);
template void Config::write<int64_t>(const char *key, int64_t *&buff);
template void Config::write<bool>(const char *key, bool *&buff);
template void Config::write<float_t>(const char *key, float_t *&buff);
template void Config::write<double_t>(const char *key, double_t *&buff);
template void Config::write<const char *>(const char *key, const char **&buff);
template void Config::write<String>(const char *key, String *&buff);
template void Config::write<Config::LDR_t>(const char *key, Config::LDR_t *&buff);
template void Config::write<Config::LDR_t *>(const char *key, Config::LDR_t *&buff);

/**
 * @brief Reads a value from the config
 *
 * @tparam T Type of the value to read
 * @param key Key of the value to read
 * @param value Value to read
 * @return void
 * @see Takes a buffer as parameter to avoid the need to create a temporary variable
 */
template <typename T>
bool Config::read(const char *key, T &buff)
{
    if (stateManager.getCurrentConfigState() == ProgramStates::DeviceStates::ConfigState_e::Writing)
    {
        log_e("Config::read() - Config is being written and can not be accessed currently");
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::ErrorConfig);
        return false;
    }

    /* if (sizeof(buff) != _preferences.getBytes(key, (T *)&buff, sizeof(buff)))
    {
        log_e("Config::read() - Size of buffer does not match size of value");
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::ErrorConfig);
        return false;
    } */
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Reading);
    _preferences.begin(_configName, true);
    _preferences.getBytes(key, (T *)&buff, sizeof(T) * sizeof(buff));
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
    return true;
}

template bool Config::read<uint8_t>(const char *key, uint8_t &buff);
template bool Config::read<uint16_t>(const char *key, uint16_t &buff);
template bool Config::read<uint32_t>(const char *key, uint32_t &buff);
template bool Config::read<uint64_t>(const char *key, uint64_t &buff);
template bool Config::read<int8_t>(const char *key, int8_t &buff);
template bool Config::read<int16_t>(const char *key, int16_t &buff);
template bool Config::read<int32_t>(const char *key, int32_t &buff);
template bool Config::read<int64_t>(const char *key, int64_t &buff);
template bool Config::read<bool>(const char *key, bool &buff);
template bool Config::read<float_t>(const char *key, float_t &buff);
template bool Config::read<double_t>(const char *key, double_t &buff);
template bool Config::read<const char *>(const char *key, const char *&buff);
template bool Config::read<String>(const char *key, String &buff);
template bool Config::read<Config::LDR_t>(const char *key, Config::LDR_t &buff);

/**
 * @brief Reads a value from the config
 *
 * @tparam T Type of the value to read
 * @param key Key of the value to read
 * @param value Value to read
 * @return void
 * @see Takes a buffer as parameter to avoid the need to create a temporary variable
 */
template <typename T>
bool Config::read(const char *key, T *&buff)
{
    if (stateManager.getCurrentConfigState() == ProgramStates::DeviceStates::ConfigState_e::Writing)
    {
        log_e("Config::read() - Config is being written and can not be accessed currently");
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::ErrorConfig);
        return false;
    }

    /* if (sizeof(buff) != _preferences.getBytes(key, (T *)&buff, sizeof(buff)))
    {
        log_e("Config::read() - Size of buffer does not match size of value");
        stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::ErrorConfig);
        return false;
    } */
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Reading);
    _preferences.begin(_configName, true);
    _preferences.getBytes(key, (T *)&buff, sizeof(T) * sizeof(buff));
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
    return true;
}

template bool Config::read<uint8_t>(const char *key, uint8_t *&buff);
template bool Config::read<uint16_t>(const char *key, uint16_t *&buff);
template bool Config::read<uint32_t>(const char *key, uint32_t *&buff);
template bool Config::read<uint64_t>(const char *key, uint64_t *&buff);
template bool Config::read<int8_t>(const char *key, int8_t *&buff);
template bool Config::read<int16_t>(const char *key, int16_t *&buff);
template bool Config::read<int32_t>(const char *key, int32_t *&buff);
template bool Config::read<int64_t>(const char *key, int64_t *&buff);
template bool Config::read<bool>(const char *key, bool *&buff);
template bool Config::read<float_t>(const char *key, float_t *&buff);
template bool Config::read<double_t>(const char *key, double_t *&buff);
template bool Config::read<const char *>(const char *key, const char **&buff);
template bool Config::read<String>(const char *key, String *&buff);
template bool Config::read<Config::LDR_t>(const char *key, Config::LDR_t *&buff);
template bool Config::read<Config::LDR_t *>(const char *key, Config::LDR_t *&buff);

void Config::clear()
{
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Clearing);
    _preferences.begin(_configName);
    _preferences.clear();
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
}

void Config::remove(const char *key)
{
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Clearing);
    _preferences.begin(_configName);
    _preferences.remove(key);
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
}

size_t Config::getValueLength(const char *key)
{
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Reading);
    _preferences.begin(_configName);
    size_t length = _preferences.getBytesLength(key);
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
    return length;
}

//**********************************************************
//* Return value | Prefs Type | Data Type | Enumerated Value
//* 0            | Char       | int8_t    | PT_I8
//* 1            | UChar      | uint8_t   | PT_U8
//* 1            | Bool       | bool      | PT_U8
//* 2            | Short      | int16_t   | PT_I16
//* 3            | UShort     | uint16_t  | PT_U16
//* 4            | Int        | int32_t   | PT_I32
//* 4            | Long       | int32_t   | PT_I32
//* 5            | UInt       | int32_t   | PT_U32
//* 5            | ULong      | uint32_t  | PT_U32
//* 6            | Long64     | int64_t   | PT_I64
//* 7            | ULong64    | uint64_t  | PT_U64
//* 8            | String     | String    | PT_STR
//* 8            | String     | *char     | PT_STR
//* 9            | Float      | float_t   | PT_BLOB
//* 9            | Double     | double_t  | PT_BLOB
//* 10           | -          | -         | PT_INVALID
//************************************************************
byte Config::getType(const char *key)
{
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Reading);
    _preferences.begin(_configName);
    byte type = _preferences.getType(key);
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
    return type;
}

size_t Config::freeEntries()
{
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Reading);
    _preferences.begin(_configName);
    size_t freeEntries = _preferences.freeEntries();
    _preferences.end();
    stateManager.setState(ProgramStates::DeviceStates::ConfigState_e::Configured);
    return freeEntries;
}
