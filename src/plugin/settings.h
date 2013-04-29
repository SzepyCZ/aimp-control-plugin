// Copyright (c) 2013, Alexey Ivanov

#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <exception>
#include <set>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>

namespace ControlPlugin { namespace PluginSettings {

struct Settings
{
    struct HttpServer {
        std::string ip_to_bind; //!< Address of network interface to listen by server. Use "" to bind to all network interfaces. Example: "localhost", "192.168.0.1"
        std::string port; //!< port to listen. String type was chosen since it make easy IPv6 support.
        std::wstring document_root; //!< Path to directory.
        std::set<std::string> init_cookies; //! Cookies which server sends to client on page first load.
    } http_server;

    struct Logger {
        int severity_level;
        std::wstring directory; //<! path to directory to store logs.
        std::set<std::string> modules_to_log; //!< names of modules where logging is enabled.
    } logger;

    struct Misc {
        bool enable_track_upload;
    } misc;
};


//! Provides access to application settings. Saves/loads setting from file.
class Manager : boost::noncopyable
{
public:
    Manager();

    /*!
        \brief Loads settings from file.
        If error occurs, settings remain equal to default values.
    */
    void load(const boost::filesystem::wpath& filename); // throws ()

    /*!
        \brief Saves settings to file.
        \throw SaveError if saving fails.
    */
    void save(const boost::filesystem::wpath& filename) const; // throws SaveError

    const Settings& settings() const
        { return settings_; }

private:

    void setDefaultLoggerSettings();
    void setDefaultHttpServerSettings();

    Settings settings_;
};


/*!
    \brief Base class for exceptions generated by Manager functions.
    Used to be able to catch any Manager exception like this:
        try { ..error code.. } catch (PluginSettingsException&) { ..handle.. }
*/
class PluginSettingsException : public std::exception
{
public:
    explicit PluginSettingsException(const std::string& what_arg)
        :
        std::exception( what_arg.c_str() )
    {}
};

/*!
    Save file error exception.
*/
class SaveError : public PluginSettingsException
{
public:
    explicit SaveError(const std::string& what_arg)
        : PluginSettingsException(what_arg)
    {}
};

} } // namespace ControlPlugin::PluginSettings

#endif // #ifndef PLUGIN_SETTINGS_H
