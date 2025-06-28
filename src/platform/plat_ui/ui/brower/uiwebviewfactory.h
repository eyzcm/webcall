#ifndef UIWEBVIEWFACTORY_H
#define UIWEBVIEWFACTORY_H


#include "ui/brower/UIWebEngineView.h"
#include "ui/brower/uiwebview.h"

class UIWebViewFactory {
public:
    static IWebView* CreateWebView(QWidget* parent, bool useWebEngine);
};

#endif // UIWEBVIEWFACTORY_H
