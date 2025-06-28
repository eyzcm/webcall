#include "uiwebviewfactory.h"
#include "UIWebEngineView.h"

IWebView* UIWebViewFactory::CreateWebView(QWidget* parent, bool useWebEngine)
{
        if (useWebEngine) {
            return new UIWebEngineView(parent);
        } else {
            //以后支持webkit,或特殊的UIWebEngineView
            return nullptr;//return new UIWebKitView(parent);
        }

}
