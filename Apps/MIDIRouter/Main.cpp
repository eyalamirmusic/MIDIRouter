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
    GuiAppTemplateApplication() = default;

private:
    const String getApplicationName() override
    {
        return JUCE_APPLICATION_NAME_STRING;
    }
    const String getApplicationVersion() override
    {
        return JUCE_APPLICATION_VERSION_STRING;
    }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override { app.create(); }

    void shutdown() override { app.reset(); }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const String& /*commandLine*/) override {}

    OwningPointer<App> app;
};

} // namespace MIDIRouterApp

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MIDIRouterApp::GuiAppTemplateApplication)
