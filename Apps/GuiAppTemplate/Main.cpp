#include "TrayIcon.h"
#include "Router.h"

namespace MIDIRouterApp
{
struct App
{
    State state;
    MIDIRouter router {state};
    DummyMenuBarModel dummyMenuBar;
    TrayIcon icon {state};
};

class GuiAppTemplateApplication : public juce::JUCEApplication
{
public:
    const String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }
    const String getApplicationVersion() override
    {
        return JUCE_APPLICATION_VERSION_STRING;
    }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override { app = std::make_unique<App>(); }

    void shutdown() override { app.reset(); }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const String& /*commandLine*/) override {}

private:
    std::unique_ptr<App> app;
};

} // namespace GuiApp

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MIDIRouterApp::GuiAppTemplateApplication)
